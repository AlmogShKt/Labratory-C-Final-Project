#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "globals.h"
#include "Errors.h"

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

char *INSTUCTIONS[] = {"data","string","extern","entry"};

int lines_too_long(char *file_name){
    char str[SIZE];
    FILE *fp;
    location am_file;
    int too_long;
    fp = fopen(file_name,"r");
    am_file.file_name = file_name;
    too_long = 0;
    while(fgets(str,SIZE,fp) != NULL){
        (am_file.line_num)++;
        if(strlen(str) > MAX_LINE_LENGTH){
            /* line is too long */
            print_external_error(ERROR_CODE_30, am_file);
            too_long = 1;
        }
    }
    return too_long;
}

int is_instr(char *str){
    if(str == NULL){
        return 0;
    }
    int i;
    for (i = 0; i < INSTRUCTIONS_COUNT; i++) {
        if (strcmp(str, INSTUCTIONS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int what_opcode(char *str){
    if(str == NULL){
        return -1;
    }
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
    if(str == NULL){
        return -1;
    }
    for (i = 0; i < REG_COUNT; i++) {
        if (strcmp(str, REGS[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int legal_label_decl(char *str){
    if(str == NULL){
        return 0;
    }
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
    if(str == NULL){
        return 0;
    }
    if(isalpha(*str) && strlen(str) <= MAX_LABEL_LENGTH && (what_reg(str) < 0) && (what_opcode(str) < 0) && \
        !is_instr(str)){
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

int is_num(char *str){
    char *ptr;
    return (strtol(str,&ptr,10) && (*ptr == '\0'));
}

int is_reg_or_label_or_num(char *str){
    char *ptr;
    return (is_reg_or_label(str) || is_num(str));
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
        case 1: {
            if(is_reg_or_label_or_num(str1) && is_reg_or_label_or_num(str2)){
                remove_asp(&str1);
                remove_asp(&str2);
                command->source = str1;
                command->dest = str2;
            }
            else {
                *error_code = ERROR_CODE_33;
                return 0;
            }
            break;
        }
        case 0:
        case 2:
        case 3: {
            if(is_reg_or_label_or_num(str1) && is_reg_or_label(str2)){
                remove_asp(&str1);
                remove_asp(&str2);
                command->source = str1;
                command->dest = str2;
            }
            else {
                *error_code = ERROR_CODE_33;
                return 0;
            }
            break;
        }
        case 6: {
            if(legal_label(str1) && is_reg_or_label(str2)){
                remove_asp(&str1);
                remove_asp(&str2);
                command->source = str1;
                command->dest = str2;
            }
            else {
                *error_code = ERROR_CODE_33;
                return 0;
            }
            break;
        }
        case 4:
        case 5:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 13: {
            if(is_reg_or_label(str)){
                command->source = NULL;
                remove_asp(&str);
                command->dest = str;
            }
            else {
                *error_code = ERROR_CODE_33;
                return 0;
            }
            break;
        }
        case 12: {
            if(is_reg_or_label_or_num(str)){
                command->source = NULL;
                remove_asp(&str);
                command->dest = str;
            }
            else {
                *error_code = ERROR_CODE_33;
                return 0;
            }
            break;
        }
            /* case 14,15 are dealt with in previous stage */
        default: {
            *error_code = ERROR_CODE_33;
            return 0;
        }
    }
    return 1;
}

int inc_array(inst_parts **inst, int len){
    short *ptr;
    ptr = (*inst)->nums;
    (*inst)->nums = realloc((*inst)->nums,(len+1) * sizeof(short));
    if((*inst)->nums == NULL){
        print_internal_error(ERROR_CODE_1);
        free(ptr);
        return 0;
    }
    return 1;
}

int capture_nums(char *str, inst_parts *inst, int *error_code){
    char *token;
    int len;
    len = 0;
    while((token = strtok(NULL,",\n")) != NULL){
        if(is_num(token)){
            if(inc_array(&inst,++len) == 0){
                return 0;
            }
            *(inst->nums+len-1) = (short)(atoi(token));
        }
        else {
            if(strcmp(token,"\n") == 0){
                /* comma AFTER the last number */
                *error_code = ERROR_CODE_51;
                return 0;
            }
            *error_code = ERROR_CODE_50;
            return 0;
        }
    }
    inst->len = len;
    return 1;
}

int capture_string(char *str, inst_parts *inst, int *error_code){
    int flag;   /* if at least one char was saved */
    char *token;
    int len;
    len = inst->len = 0;
    if(*(token = strtok(NULL,"\n")) != '"'){
        /* there was no first '"' */
        *error_code = ERROR_CODE_52;
        return 0;
    }
    token++;
    if(strchr(token,'"') == NULL){
        /* there was no second '"' */
        *error_code = ERROR_CODE_52;
        return 0;
    }
    flag = 0;
    while(*(token+len) != '"'){
        if(inc_array(&inst,++len) == 0){
            return 0;
        }
        *(inst->nums+len-1) = (short)(*(token+len-1));
        flag = 1;
    }
    if(*(token+len+1) != '\0' && *(token+len+1) != ' '){
        /* extra text after the string end */
        *error_code = ERROR_CODE_53;
        if(flag == 1){
            free(inst->nums);
        }
        return 0;
    }
    /* add \0 sign as the end of the string */
    if(inc_array(&inst,++len) == 0){
        return 0;
    }
    *(inst->nums+len-1) = 0;
    inst->len = len;
    return 1;
}

inst_parts *read_instruction(char *str, int *error_code){
    int num_count;
    inst_parts *inst;
    char *token;
    *error_code = 0;
    if(strstr(str,".") == NULL){
        return 0;
    }
    token = strtok(str," \n");
    inst = handle_malloc(sizeof(inst_parts));
    inst->label = NULL;
    inst->nums = NULL;
    num_count = 0;
    if(legal_label_decl(token)){
        inst->label = token;
        token = strtok(NULL," \n");
    }
    else if(strcmp(token,".data") == 0 || strcmp(token,".string") == 0 || \
            strcmp(token,".entry") == 0 || strcmp(token,".extern") == 0) {
        inst->label = NULL;
    }
    if(strcmp(token,".data") == 0){
        if(capture_nums(str,inst,error_code) == 0){
            free(inst);
            return 0;
        }
    }
    else if (strcmp(token,".string") == 0){
        if(capture_string(str,inst,error_code) == 0){
            free(inst);
            return 0;
        }
    }
    else if(strcmp(token,".entry") == 0){
        token = strtok(NULL," \n");
        if(legal_label(token)){
            inst->label = NULL; /* ignore label in the beginning */
            inst->len = -1; /* not a data line at all */
            inst->arg_label = token;
            inst->nums = 0;
            inst->is_extern = 0;
        }
        else {
            *error_code = ERROR_CODE_37;
            free(inst);
            return 0;
        }
    }
    else if(strcmp(token,".extern") == 0) {
        token = strtok(NULL, " \n");
        if (legal_label(token)) {
            inst->label = NULL; /* ignore label in the beginning */
            inst->len = -1; /* not a data line at all */
            inst->arg_label = token;
            inst->nums = 0;
            inst->is_extern = 1;
        }
    }
    return inst;
}

command_parts *read_command(char *str, int *error_code){
    int args;
    char *token;
    command_parts *command = handle_malloc(sizeof(command_parts));
    if(command == NULL){
        return NULL;
    }
    token = strtok(str," \n");
    *error_code = 0;
    /* there is a legal label in the line */
    if(legal_label_decl(token)) {
        command->label = token;
        token = strtok(NULL, " \n");
        if ((command->opcode = what_opcode(token)) != -1) {
            ;
        }
        else {
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
        legal_arg(strtok(NULL, "\n"), command, error_code);
    }
        /* command line without opcode */
    else {
        *error_code = ERROR_CODE_31;
        return NULL;
    }
    return command;
}