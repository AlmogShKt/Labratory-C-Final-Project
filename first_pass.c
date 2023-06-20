
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

int first_pass(line_data source, code_conv *code){
    code->line = IC_INIT_VALUE;
    code->binary_code = handle_malloc(sizeof(unsigned short));
    if(code->binary_code == NULL){
        return 0;
    }


    return 1;
}

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

short command_to_binary(command_parts *command){
    if(command->)
}

int exe_first_pass(char *file_name){
    int num_files, error_code, IC;
    command_parts *command;
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
    IC = IC_INIT_VALUE;
    while(fgets(str,MAX_LINE_LENGTH,fp) != NULL){
        //strcpy(line.data,str);
        (line.number)++;
        printf("%s ",str);
        command = read_command(str,&error_code);
        if(error_code == 0){
            printf("line %d: label - %s, opcode = %d, source - %s, dest - %s\n",
                   line.number, command->label, command->opcode, command->source, command->dest);
        }
        else {
            print_external_error(error_code,line);
        }

        if(command->label != NULL){
            insert_label_table(&label_table,label_table_line,command,IC);
        }

        free(command);
    }
    return 0;
}