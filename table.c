#ifndef LABRATORY_C_FINAL_PROJECT_TABLE_H
#define LABRATORY_C_FINAL_PROJECT_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table.h"
#include "globals.h"
#include "Errors.h"
#include "code_conversion.h"

int insert_label_table(label_address **label_table, int lines, char *label, int counter, location am_file, int is_data){
    label_address *p_temp;
    p_temp = *label_table;
    *label_table = realloc(*label_table,lines * sizeof(label_address));
    if(*label_table == NULL){
        free(p_temp);
        return 0;
    }
    (*label_table+lines-1)->is_data = is_data;
    (*label_table+lines-1)->address = counter;
    (*label_table+lines-1)->assembly_line = am_file.line_num;
    (*label_table+lines-1)->label_name = malloc((strlen(label)+1)*sizeof(char));
    if((*label_table+lines-1)->label_name == NULL){
        return 0;
    }
    strcpy((*label_table+lines-1)->label_name,label);
    return 1;
}

int check_each_label_once(label_address *label_table, int table_lines, char *file_name){
    int i, j;
    for (i = 0; i < table_lines-1; i++){
        for (j = i+1; j < table_lines; j++){
            if(strcmp((label_table+i)->label_name,(label_table+j)->label_name) == 0){
                location am_file;
                am_file.file_name = file_name;
                am_file.line_num = (label_table+j)->assembly_line;
                print_external_error(ERROR_CODE_55,am_file);
                return 0;
            }
        }
    }
    return 1;
}

void change_label_table_data_count(label_address *label_table, int table_lines, int IC){
    int i;
    for (i = 0; i < table_lines; i++){
        if((label_table+i)->is_data){
            (label_table+i)->address += IC+1;
        }
    }
}

void reset_labels_address(label_address *label_table, int table_lines){
    int i;
    for (i = 0; i < table_lines; i++){
        (label_table+i)->address += IC_INIT_VALUE;
    }
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

#endif