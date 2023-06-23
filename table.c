#ifndef LABRATORY_C_FINAL_PROJECT_TABLE_H
#define LABRATORY_C_FINAL_PROJECT_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table.h"
#include "globals.h"
#include "lexer.h"

int insert_label_table(label_address **label_table, int lines, command_parts *command, int IC){
    label_address *p_temp;
    p_temp = *label_table;
    *label_table = realloc(*label_table,lines * sizeof(label_address));
    if(*label_table == NULL){
        free(p_temp);
        return 0;
    }
    (*label_table+lines-1)->address = IC;
    (*label_table+lines-1)->label_name = malloc((strlen(command->label)+1)*sizeof(char));
    if((*label_table+lines-1)->label_name == NULL){
        return 0;
    }
    strcpy((*label_table+lines-1)->label_name,command->label);
    return 1;
}


#endif