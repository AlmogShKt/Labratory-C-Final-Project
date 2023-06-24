
/* Contains major function that are related to the first pass */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Errors.h"
#include "globals.h"
#include "table.h"
#include "util.h"
#include "first_pass.h"
#include "lexer.h"

int inc_mem(code_conv **code, int IC){
    code_conv *ptr;
    ptr = *code;
    *code = realloc(*code,(IC+1) * sizeof(code_conv));
    if(*code == NULL){
        print_internal_error(ERROR_CODE_1);
        free(ptr);
        return 0;
    }
    return 1;
}

unsigned short command_to_short(command_parts *command){
    unsigned short n_src, n_op, n_dest;
    n_src = n_op = n_dest = 0;
    if(what_reg(command->source) >= 0){
        n_src = REG_ADDRESSING << SOURCE_BITS_SHIFT;
    }
    else if(legal_label(command->source)){
        n_src = (short)(LABEL_ADDRESSING << SOURCE_BITS_SHIFT);
    }
    else if(is_num(command->source)){
        n_src = (short)(DIRECT_ADDRESSING << SOURCE_BITS_SHIFT);
    }
    if(what_reg(command->dest) >= 0){
        n_dest = (short)(REG_ADDRESSING << DEST_BITS_SHIFT);
    }
    else if(legal_label(command->dest)){
        n_dest = (short)(LABEL_ADDRESSING << DEST_BITS_SHIFT);
    }
    else if(is_num(command->dest)){
        n_dest = (short)(DIRECT_ADDRESSING << DEST_BITS_SHIFT);
    }
    n_op = (short)(command->opcode) << OPCODE_BITS_SHIFT;
    return ((n_src | n_op) | n_dest);
}

unsigned short reg_to_short(command_parts *command, int reg_src){
    static int already_done;
    int reg1, reg2;
    unsigned short n_reg_src, n_reg_dest;
    n_reg_src = n_reg_dest = 0;
    if(reg_src){
        if((reg1 = what_reg(command->source)) >= 0) {
            n_reg_src = reg1 << SOURCE_BITS_SHIFT_REG;
        }
        if((reg2 = what_reg(command->dest)) >= 0) {
            n_reg_dest = reg2 << DEST_BITS_SHIFT_REG;
        }
        already_done = 1;
        return (n_reg_src | n_reg_dest);
    }
    else if(already_done == 0){
        if((reg2 = what_reg(command->dest)) >= 0) {
            n_reg_dest = reg2 << DEST_BITS_SHIFT_REG;
        }
        return n_reg_dest;
    }
    already_done = 0;
    return DOUBLE_REGS_VALUE;
}

int add_machine_code_line(code_conv **code, unsigned short num, char *str, int *IC){
    char *bin_num;
    if(inc_mem(code,*IC) == 0){
        return 0;
    }
    (*code+*IC)->short_num = num;
    if(str == NULL){
        (*code+*IC)->label = NULL;
    }
    else {
        (*code+*IC)->label = handle_malloc((strlen(str)+1) * sizeof(char));
        if((*code+*IC)->label == NULL){
            return 0;
        }
        strcpy((*code+*IC)->label,str);
    }
    // bin_num = short_to_binary((*code + *IC)->short_num);
    // printf("Address %d binary code is: %s\n", *IC, bin_num);
    return 1;
}

int add_extra_machine_code_line(code_conv **code, command_parts *command, int *IC, int is_src){
    unsigned short num;
    char *arg;
    arg = (is_src) ? command->source : command->dest;
    if(what_reg(arg) > 0 && (num = reg_to_short(command,is_src)) != DOUBLE_REGS_VALUE){
        (*IC)++;
        if(add_machine_code_line(code, num, NULL, IC) == 0){
            return 0;
        }
    }
    else if(legal_label(arg)) {
        (*IC)++;
        if(add_machine_code_line(code, RELOCATABLE_VALUE, arg, IC) == 0) {
            return 0;
        }
    }
    else if(is_num(arg)){
        (*IC)++;
        /* representing number in 2-11 bits, therefore pushing the number ARE_BITS bits to the left */
        if(add_machine_code_line(code, atoi(arg) << ARE_BITS, NULL, IC) == 0) {
            return 0;
        }
    }
    /* added successfully or had nothing to add */
    return 1;

}

int add_machine_code_data(code_conv **code, inst_parts *inst, int *IC){
    int i;
    char *bin_num;
    for (i = 0; i < inst->len; i++){
        if(inc_mem(code,*IC) == 0){
            return 0;
        }
        (*code+*IC)->short_num = *(inst->nums+i);
        (*code+*IC)->label = NULL;
        // bin_num = short_to_binary((*code + *IC)->short_num);
        // printf("Address %d binary code is: %s\n", *IC, bin_num);
        (*IC)++;
    }
    (*IC)--;
    return 1;
}

void replace_labels(code_conv *code, label_address *label_table, int label_table_line, int IC_len){
    int i,j;
    for (i = IC_INIT_VALUE; i <= IC_len; i++){
        if((code+i)->label != NULL){
            for (j = 0; j < label_table_line; j++){
                if(strcmp((code+i)->label,(label_table+j)->label_name) == 0){
                    (code+i)->short_num |= ((label_table+j)->address) << ARE_BITS;
                }
            }
        }
    }
}

void print_binary_code(code_conv *code,int IC_len){
    int i;
    char *bin_num;
    for (i = IC_INIT_VALUE; i <= IC_len; i++){
        bin_num = short_to_binary((code + i)->short_num);
        printf("Address %d binary code is: %s\n", i, bin_num);
    }
}

int remove_label_table(label_address *label_table){
    return 1;
}

int exe_first_pass(char *file_name){
    int num_files, error_code, IC;
    code_conv *code;
    command_parts *command;
    inst_parts *inst;
    line_data line;
    char str[MAX_LINE_LENGTH];
    FILE *fp;
    label_address *label_table;
    int label_table_line;
    error_code = 0;
    fp = fopen(file_name,"r");
    line.file_name = file_name;
    line.number = 0;
    label_table_line = 0;
    label_table = NULL;
    code = handle_malloc(IC_INIT_VALUE * sizeof(code_conv));
    for (IC = 0; IC < IC_INIT_VALUE; IC++){
        (code+IC)->short_num = 0;
        (code+IC)->label = NULL;
    }
    IC--;
    while(fgets(str,MAX_LINE_LENGTH,fp) != NULL){
        (line.number)++;
        /* skipping empty line */
        if(strcmp(str,"\n") == 0){
            continue;
        }
        //strcpy(line.data,str);
        printf("%s ",str);
        if(strstr(str,".data") != 0 || strstr(str,".string") != 0){
            inst = read_instruction(str,&error_code);
            if(error_code == 0){
                printf("line %d: label - %s\n",line.number, inst->label);
                IC++;
            }
            else {
                print_external_error(error_code,line);
                free(inst);
                return 0;
            }
            if(inst->label != NULL){
                insert_label_table(&label_table,++label_table_line,inst->label,IC);
            }
            if(add_machine_code_data(&code, inst, &IC) == 0){
                free(inst->nums);
                free(inst);
                return 0;
            }
            free(inst->nums);
            free(inst);
        }
        else {
            command = read_command(str,&error_code);
            if(error_code == 0){
                printf("line %d: label - %s, opcode = %d, source - %s, dest - %s\n",
                       line.number, command->label, command->opcode, command->source, command->dest);
                IC++;
            }
            else {
                print_external_error(error_code,line);
                free(command);
                return 0;
            }

            if(command->label != NULL){
                insert_label_table(&label_table,++label_table_line,command->label,IC);
            }
            if(add_machine_code_line(&code, command_to_short(command), NULL, &IC) == 0){
                free(command);
                return 0;
            }
            if(add_extra_machine_code_line(&code,command,&IC,1) == 0 || \
            add_extra_machine_code_line(&code,command,&IC,0) == 0){
                free(command);
                return 0;
            }
            free(command);
        }
    }
    replace_labels(code,label_table,label_table_line,IC);
    print_binary_code(code,IC);
    return 1;
}