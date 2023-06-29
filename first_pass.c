
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

int inc_mem(code_conv **code, int counter){
    code_conv *ptr;
    ptr = *code;
    *code = realloc(*code,(counter+1) * sizeof(code_conv));
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

int add_machine_code_line(code_conv **code, unsigned short num, char *str, int *IC, location am_file){
    char *bin_num;
    if(inc_mem(code,*IC) == 0){
        return 0;
    }
    (*code+*IC)->short_num = num;
    (*code+*IC)->assembly_line = am_file.line_num;
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
    bin_num = short_to_binary((*code + *IC)->short_num);
    printf("Assembly line %d, Code address %d binary code is: %s\n",\
        (*code + *IC)->assembly_line,*IC, bin_num);
    return 1;
}

int add_extra_machine_code_line(code_conv **code, command_parts *command, int *IC, int is_src, location am_file){
    unsigned short num;
    char *arg;
    arg = (is_src) ? command->source : command->dest;
    if(what_reg(arg) > 0 && (num = reg_to_short(command,is_src)) != DOUBLE_REGS_VALUE){
        (*IC)++;
        if(add_machine_code_line(code, num, NULL, IC, am_file) == 0){
            return 0;
        }
    }
    else if(legal_label(arg)) {
        (*IC)++;
        if(add_machine_code_line(code, RELOCATABLE_VALUE, arg, IC, am_file) == 0) {
            return 0;
        }
    }
    else if(is_num(arg)){
        (*IC)++;
        /* representing number in 2-11 bits, therefore pushing the number ARE_BITS bits to the left */
        if(add_machine_code_line(code, atoi(arg) << ARE_BITS, NULL, IC, am_file) == 0) {
            return 0;
        }
    }
    /* added successfully or had nothing to add */
    return 1;

}

int add_machine_code_data(code_conv **data, inst_parts *inst, int *DC, location am_file){
    int i;
    for (i = 0; i < inst->len; i++){
        char *bin_num;
        /* todo:
         * need to implement conde conversion for lines with .entry & .extern?
         * or just in label_table and output files
         *
         * */
        if(inc_mem(data,*DC) == 0){
            return 0;
        }
        (*data+*DC)->short_num = *(inst->nums+i);
        (*data+*DC)->label = NULL; /* a data line cannot include a label as an ARGUMENT */
        (*data+*DC)->assembly_line = am_file.line_num;
        bin_num = short_to_binary((*data + *DC)->short_num);
        printf("Assembly line %d, Code address %d binary code is: %s\n",\
        (*data + *DC)->assembly_line,*DC, bin_num);
        (*DC)++;
    }
    //(*DC)--;
    return 1;
}

int error_replace_labels(code_conv *code, label_address *label_table, int label_table_line, int IC_len, char *file_name){
    int i,j, found, error_found;
    error_found = 0;
    for (i = 0; i <= IC_len; i++){
        found = 0;
        if((code+i)->label != NULL){
            for (j = 0; j < label_table_line; j++){
                if(strcmp((code+i)->label,(label_table+j)->label_name) == 0){
                    (code+i)->short_num |= ((label_table+j)->address) << ARE_BITS;
                    found = 1;
                }
            }
            /* if label is not defined in the assembly file */
            if(!found){
                location am_file;
                am_file.file_name = file_name;
                am_file.line_num = (code+i)->assembly_line;
                print_external_error(ERROR_CODE_36,am_file);
                error_found = 1;
            }
        }
    }
    return error_found;
}

void print_binary_code(code_conv *code,int IC_len){
    int i;
    char *bin_num;
    for (i = 0; i <= IC_len; i++){
        bin_num = short_to_binary((code + i)->short_num);
        printf("Assembly line %d, Code address %d binary code is: %s\n",\
        (code + i)->assembly_line,+IC_INIT_VALUE+i, bin_num);
    }
}

int merge_code(code_conv **code, code_conv *data, int IC, int DC){
    int i;
    code_conv *ptr;
    ptr = *code;
    if(inc_mem(code,IC+DC) == 0){
        free(ptr);
        free(data);
        return 0;
    }
    /* coping the info from the data lines into the end of the command code lines */
    for(i = 0; i < DC; i++){
        (*code+IC+i+1)->label = (data+i)->label;
        (*code+IC+i+1)->assembly_line = (data+i)->assembly_line;
        (*code+IC+i+1)->short_num = (data+i)->short_num;
    }
    free(data); /* no need anymore for the code from the data */
    return 1;
}

int legal_entry(char *str){
    return 1;
}

int remove_label_table(label_address *label_table){
    return 1;
}


int exe_first_pass(char *file_name){
    int error_code, IC, DC, error_found, label_table_line;
    code_conv *code, *data;
    command_parts *command;
    inst_parts *inst;
    location am_file;
    char str[MAX_LINE_LENGTH];
    FILE *fp;
    label_address *label_table;
    error_code = 0;
    error_found = 0;
    if(lines_too_long(file_name)){
        error_found = 1;
    }
    fp = fopen(file_name,"r");
    am_file.file_name = file_name;
    am_file.line_num = 0;
    label_table_line = 0;
    label_table = NULL;
    /*
    code = handle_malloc(IC_INIT_VALUE * sizeof(code_conv));
    for (IC = 0; IC < IC_INIT_VALUE; IC++){
        (code+IC)->short_num = 0;
        (code+IC)->label = NULL;
    }
    IC--;
    */
    IC = -1;
    DC = 0;
    code = handle_malloc(sizeof(code_conv));
    data = handle_malloc(sizeof(code_conv));
    while(fgets(str,MAX_LINE_LENGTH,fp) != NULL && IC <= IC_MAX){
        (am_file.line_num)++;
        if(strcmp(str,"\n") == 0){
            continue;
        }
        printf("%s ",str);
        /* todo: change to is_inst(str). also in other places */
        if(strstr(str,".data") != NULL || strstr(str,".string") != NULL || \
            strstr(str,".entry") != NULL || strstr(str,".extern") != NULL){
            inst = read_instruction(str,&error_code);
            if(error_code == 0){
                printf("line %d: label - %s\n",am_file.line_num, inst->label);
                //DC++;
            }
            else {
                print_external_error(error_code,am_file);
                free(inst);
                error_found = 1;
            }
            if(inst->label != NULL){
                insert_label_table(&label_table,++label_table_line,inst->label,DC,am_file,1);
            }
            if(add_machine_code_data(&data, inst, &DC, am_file) == 0){
                free(inst->nums);
                free(inst);
                error_found = 1;
            }

            free(inst->nums);
            free(inst);
        }
        /*
        else if(strstr(str,".entry") != NULL){
            ddd
        }
        */
        else {
            command = read_command(str,&error_code);
            if(error_code == 0){
                printf("line %d: label - %s, opcode = %d, source - %s, dest - %s\n",
                       am_file.line_num, command->label, command->opcode, command->source, command->dest);
                IC++;
            }
            else {
                print_external_error(error_code,am_file);
                free(command);
                error_found = 1;
            }

            if(command->label != NULL){
                insert_label_table(&label_table,++label_table_line,command->label,IC,am_file,0);
            }
            if(add_machine_code_line(&code, command_to_short(command), NULL, &IC, am_file) == 0){
                free(command);
                error_found = 1;
            }
            if(add_extra_machine_code_line(&code,command,&IC,1,am_file) == 0 || \
            add_extra_machine_code_line(&code,command,&IC,0,am_file) == 0){
                free(command);
                error_found = 1;
            }
            free(command);
        }
    }
    if(IC > IC_MAX){
        print_internal_error(ERROR_CODE_54);
        error_found = 1;
    }
    if(check_each_label_once(label_table,label_table_line,file_name) == 0){
        error_found = 1;
    }
    change_label_table_data_count(label_table, label_table_line, IC);
    /*
    int i;
    for(i = 0; i < label_table_line; i++){
        printf("address: %d, label_name: %s, assembly_line: %d\n",\
        (label_table+i)->address,(label_table+i)->label_name,(label_table+i)->assembly_line);
    }
    */
    reset_labels_address(label_table, label_table_line);
    if(error_replace_labels(code,label_table,label_table_line,IC,file_name)){
        error_found = 1;
    }
    if(merge_code(&code, data, IC, DC) == 0){
        error_found = 1;
    }
    print_binary_code(code,IC+DC);

    return error_found;
}