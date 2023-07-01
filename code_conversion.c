//
// Created by sheme on 30/06/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "code_conversion.h"
#include "Errors.h"
#include "globals.h"
#include "table.h"
#include "util.h"
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
    /*
    printf("Assembly line %d, Code address %d binary code is: %s\n",\
        (*code + *IC)->assembly_line,*IC, bin_num);
    */
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
        if(inc_mem(data,*DC) == 0){
            return 0;
        }
        (*data+*DC)->short_num = *(inst->nums+i);
        (*data+*DC)->label = NULL; /* a data line cannot include a label as an ARGUMENT */
        (*data+*DC)->assembly_line = am_file.line_num;
        bin_num = short_to_binary((*data + *DC)->short_num);
        /*
        printf("Assembly line %d, Code address %d binary code is: %s\n",\
        (*data + *DC)->assembly_line,*DC, bin_num);
        */
        (*DC)++;
    }
    //(*DC)--;
    return 1;
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

void free_code(code_conv *code,int code_count){
    int i;
    for (i = 0; i <= code_count; i++){
        free((code+i)->label);
    }
    free(code);
}

int remove_label_table(label_address *label_table){
    return 1;
}
