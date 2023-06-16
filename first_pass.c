
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

int is_opcode(char *str){
    if((strcmp(str,"mov") && strcmp(str,"cmp") && strcmp(str,"add") && strcmp(str,"sub") && \
    strcmp(str,"lea") && strcmp(str,"not") && strcmp(str,"clr") && strcmp(str,"inc") && \
    strcmp(str,"dec") && strcmp(str,"jmp") && strcmp(str,"bne") && strcmp(str,"red") && \
    strcmp(str,"prn") && strcmp(str,"jsr") && strcmp(str,"rts") && strcmp(str,"stop")) != 0) {
        return 0;
    }
    return 1;
}

int is_reg(char *str){
    if(strcmp(str,"r0") && strcmp(str,"r1") && strcmp(str,"r2") && strcmp(str,"r3") && \
    strcmp(str,"r4") && strcmp(str,"r5") && strcmp(str,"r6") && strcmp(str,"r7") != 0){
        return 0;
    }
    return 1;
}


int legal_label(char *str){
    if(isalpha(*str) && strlen(str) <= MAX_LABEL_LENGTH && !is_reg(str) && !is_opcode(str)){
        while(*(++str) != '\0' && (isalpha(*str) || isdigit(*str))){
            ;
        }
        if(*str == ':' && *(++str) == '\0'){
            return 1;
        }
    }
    return 0;
}

int arg_count(char *opcode){
    if(strcmp(opcode,"rts") && strcmp(opcode,"stop") == 0){
        return 0;
    }
    if(strcmp(opcode,"not") && strcmp(opcode,"clr") && strcmp(opcode,"inc") && strcmp(opcode,"dec") && \
    strcmp(opcode,"jmp") && strcmp(opcode,"bne") && strcmp(opcode,"red") && strcmp(opcode,"prn") && \
    strcmp(opcode,"jsr") == 0){
        return 1;
    }
    if(strcmp(opcode,"mov") && strcmp(opcode,"cmp") && strcmp(opcode,"add") && strcmp(opcode,"sub") && \
    strcmp(opcode,"lea") == 0) {
        return 2;
    }
    return -1;
}

int extra_text(){
    char *token;
    token = strtok(NULL,"\n");
    if(token != NULL){
        return 1;
    }
    return 0;
}

command_parts *read_command(char *str, int *error_code){
    int args;
    char *token;
    command_parts *command = malloc(sizeof(command_parts));
    token = strtok(str," \n");
    /* there is a legal label in the line */
    if(legal_label(token)){
        command->label = token;
        token = strtok(NULL," \n");
        if(is_opcode(token)){
            command->opcode = token;
        }
        else {
            *error_code = ERROR_CODE_31;
            return NULL;
        }
        args = arg_count(command->opcode);
        switch(args){
            case 0: {
                if(extra_text()){
                    *error_code = ERROR_CODE_32;
                }
                break;
            }
            case 1: {

                break;
            }
            case 2: {

                break;
            }
            default: {
                return NULL;
            }
        }

    }
    /* command line with legal opcode without a label */
    else if(is_opcode(token)){
        command->label = NULL;
        command->opcode = token;
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