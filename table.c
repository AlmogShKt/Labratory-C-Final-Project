#ifndef LABRATORY_C_FINAL_PROJECT_TABLE_H
#define LABRATORY_C_FINAL_PROJECT_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table.h"
#include "globals.h"
#include "lexer.h"
#include "Errors.h"

int insert_label_table(label_address **label_table, int lines, char *label, int IC, location am_file){
    label_address *p_temp;
    p_temp = *label_table;
    *label_table = realloc(*label_table,lines * sizeof(label_address));
    if(*label_table == NULL){
        free(p_temp);
        return 0;
    }
    (*label_table+lines-1)->address = IC;
    (*label_table+lines-1)->assembly_line = am_file.line_num;
    (*label_table+lines-1)->label_name = malloc((strlen(label)+1)*sizeof(char));
    if((*label_table+lines-1)->label_name == NULL){
        return 0;
    }
    strcpy((*label_table+lines-1)->label_name,label);
    return 1;
}

int check_each_label_once(label_address *label_table, int lines, char *file_name){
    int i, j;
    for (i = 0; i < lines-1; i++){
        for (j = i+1; j < lines; j++){
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

#endif