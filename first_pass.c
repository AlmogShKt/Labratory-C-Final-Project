
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

op_code OPCODES[] = {
        {"mov",2},
        {"cmp",2},
        {"add",2},
        {"sub",2},
        {"not",1},
        {"clr",1},
        {"lea",2},
        {"inc",1},
        {"dec",1},
        {"jmp",1},
        {"bne",1},
        {"red",1},
        {"prn",1},
        {"jsr",1},
        {"rts",0},
        {"stop",0}
};

char *REGS[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};

int lines_too_long(char *file_name){
    char str[SIZE];
    FILE *fp;
    line_data line = {file_name,0,str};
    int too_long;
    fp = fopen(file_name,"r");
    too_long = 0;
    while(fgets(str,SIZE,fp) != NULL){
        (line.number)++;
        if(strlen(str) > MAX_LINE_LENGTH){
            /* line is too long */
            print_external_error(ERROR_CODE_30, line);
            too_long = 1;
        }
    }
    return too_long;
}



int handle_first_pass(line_data current_line_struct, int *IC, int *DC, table symbol_table){
    int char_index;
    char current_symbol[MAX_LABEL_LENGTH];

    /*Initiate the char index to 0 */
    char_index = 0;

    /*!This case will be handle by Tamir in the pre-process*/
    /*Skip over not relevant chars*/
    SKIP_WHITE_SPACE(current_line_struct.data,char_index);




    return SUCCESS;
}
int is_instr(char *str){
    if(strstr(str,".data ") == NULL && strstr(str,".string ") == NULL \
     && strstr(str,".entry ") == NULL && strstr(str,".extern ") == NULL){
         return 0;
     }
    return 1;
}

int what_opcode(char *str){
    int i;
    for (i = 0; i < OPCODES_COUNT; i++) {
        if (strcmp(str, OPCODES[i].opcode) == 0) {
            return i;
        }
    }
    return -1;
}

int what_reg(char *str) {
    int i;
    for (i = 0; i < REG_COUNT; i++) {
        if (strcmp(str, REGS[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int legal_label_decl(char *str){
    if(isalpha(*str) && strlen(str) <= MAX_LABEL_LENGTH && (what_reg(str) < 0) && (what_opcode(str) < 0)){
        while(*(++str) != '\0' && (isalpha(*str) || isdigit(*str))){
            ;
        }
        if(*(str) == ':' && *(str+1) == '\0'){
            *str = '\0';
            return 1;
        }
    }
    return 0;
}



int legal_label(char *str){
    if(isalpha(*str) && strlen(str) <= MAX_LABEL_LENGTH && (what_reg(str) < 0) && (what_opcode(str) < 0)){
        while(*(++str) != '\0' && (isalpha(*str) || isdigit(*str))){
            ;
        }
    }
    if(*str == '\0'){
        return 1;
    }
    return 0;
}

int extra_text(){
    char *token;
    token = strtok(NULL,"\n");
    if(token != NULL){
        return 1;
    }
    return 0;
}

int is_reg_or_label(char *str){
    if((*str == '@' && what_reg(str+1) >= 0) || legal_label(str)){
        return 1;
    }
    return 0;
}

int is_reg_or_label_or_num(char *str){
    char *ptr;
    return (is_reg_or_label(str) || (strtol(str,&ptr,10) && (*ptr == '\0')));
}

void remove_asp(char **str){
    if(**str == '@'){
        (*str)++;
    }
}

int legal_arg(char *str, command_parts *command, int *error_code){
    char *str1, *str2;
    /* empty argument */
    if(str == NULL){
        *error_code = ERROR_CODE_34;
        return 0;
    }
    /* eliminating lines with two arguments and a missing comma */
    if(OPCODES[command->opcode].arg_num == 2) {
        if(strstr(str,",") == NULL) {
            *error_code = ERROR_CODE_35;
            return 0;
        }
        else {
            str1 = strtok(str,",");
            str2 = strtok(NULL,"\n");
            /* space before comma */
            if(*(str1+strlen(str1)-1) == ' '){
                /* eliminate the space at the end of str1 */
                *(str1+strlen(str1)-1) = '\0';
            }
            /* space after comma */
            else if(*(str2) == ' '){
                /* eliminate the space at the beginning of str2 */
                str2++;
            }
            /* no space at all near the comma */
        }
    }
    switch(command->opcode) {
        case 0: {
            if(is_reg_or_label_or_num(str1) && is_reg_or_label(str2)){
                remove_asp(&str1);
                remove_asp(&str2);
                command->source = str1;
                command->dest = str2;
            }
            break;
        }
        case 4: {
            if(is_reg_or_label(str)){
                command->source = NULL;
                remove_asp(&str);
                command->dest = str;
            }
            break;

        }


        default: {
            *error_code = ERROR_CODE_33;
            return 0;
        }
    }
    return 1;
}


command_parts *read_command(char *str, int *error_code){
    int args;
    char *token;
    command_parts *command = malloc(sizeof(command_parts));
    token = strtok(str," \n");
    /* there is a legal label in the line */
    if(legal_label_decl(token)) {
        command->label = token;
        token = strtok(NULL, " \n");
        if ((command->opcode = what_opcode(token)) != -1) { ;
        } else {
            *error_code = ERROR_CODE_31;
            return NULL;
        }
        if (OPCODES[command->opcode].arg_num == 0) {
            if (extra_text()) {
                *error_code = ERROR_CODE_32;
            } else {
                command->source = command->dest = NULL;
            }
        } else {
            legal_arg(strtok(NULL, "\n"), command, error_code);
        }
    }
    /* command line with legal opcode without a label */
    else if((command->opcode = what_opcode(token)) != -1){
        command->label = NULL;
    }
    /* command line without opcode */
    else {
        *error_code = ERROR_CODE_31;
        return NULL;
    }
    return command;
}

/*
int first_pass(line_data source, code_conv *code){
    code->line = IC_INIT_VALUE;
    code->binary_code = handle_malloc(sizeof(unsigned short));
    if(code->binary_code == NULL){
        return 0;
    }


    return 1;
}

*/


int incr_mem(code_conv *code){
    unsigned short *ptr;
    ptr = code->binary_code;
    code->binary_code = realloc(code->binary_code,code->line-IC_INIT_VALUE+1);
    if(code->binary_code == NULL){
        free(ptr);
        return 0;
    }
    return 1;
}

int exe_first_pass(char *file_name){
    int num_files, error_code;
    command_parts *command;
    char str[MAX_LINE_LENGTH];
    FILE *fp;
    error_code = 0;
    fp = fopen(file_name,"r");
    fgets(str,MAX_LINE_LENGTH,fp);
    command = read_command(str,&error_code);
    return 0;
}