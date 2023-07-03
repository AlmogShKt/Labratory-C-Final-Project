
/* Contains major function that are related to the first pass */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Errors.h"
#include "globals.h"
#include "table.h"
#include "util.h"
#include "first_pass.h"
#include "lexer.h"
#include "code_conversion.h"
#include "second_pass.h"

int exe_first_pass(char *file_name){
    int error_code, IC, DC, error_found, label_table_line, externs_count, entries_count;
    code_conv *code, *data;
    other_table *externs;
    other_table *entries;
    command_parts *command;
    inst_parts *inst;
    location am_file;
    char str[MAX_LINE_LENGTH];
    FILE *fp;
    label_address *label_table;
    error_code = error_found = 0;
    if(lines_too_long(file_name)){
        error_found = 1;
    }
    fp = fopen(file_name,"r");
    am_file.file_name = file_name;
    am_file.line_num = 0;
    label_table_line = externs_count = entries_count = 0;
    label_table = NULL;
    IC = -1;
    DC = 0;
    externs = handle_malloc(sizeof(other_table));
    if(externs == NULL){
        error_found = 1;
        return error_found;
    }
    entries = handle_malloc(sizeof(other_table));
    if(entries == NULL){
        error_found = 1;
        return error_found;
    }
    code = handle_malloc(sizeof(code_conv));
    if(code == NULL){
        error_found = 1;
        return error_found;
    }
    data = handle_malloc(sizeof(code_conv));
    if(code == NULL){
        error_found = 1;
        return error_found;
    }
    while(fgets(str,MAX_LINE_LENGTH,fp) != NULL && IC <= IC_MAX){
        (am_file.line_num)++;
        if(strcmp(str,"\n") == 0){
            continue;
        }
        if(strstr(str,".data") != NULL || strstr(str,".string") != NULL || \
            strstr(str,".entry") != NULL || strstr(str,".extern") != NULL){
            inst = read_instruction(str,&error_code);
            if(error_code != 0){
                print_external_error(error_code,am_file);
                free(inst);
                error_found = 1;
                continue;
            }
            /* .entry or .extern line (no numbers were captured */
            if(inst->nums == NULL){
                /* not a .extern line --> is a .entry line */
                if(inst->is_extern == 0){
                    if(insert_other_labels(&entries, ++entries_count, inst, am_file) == 0){
                        free(inst);
                        error_found = 1;
                        continue;
                    }
                }
                /* is a .extern line */
                else if(inst->is_extern == 1){
                    if(insert_other_labels(&externs, ++externs_count, inst, am_file) == 0){
                        free(inst);
                        error_found = 1;
                        continue;
                    }
                }
            }
            /* is not .entry nor .extern --> is .data or .string */
            else {
                if (inst->label != NULL) {
                    insert_label_table(&label_table, ++label_table_line, inst->label, DC, am_file, 1);
                }
                if (add_machine_code_data(&data, inst, &DC, am_file) == 0) {
                    free(inst->nums);
                    free(inst);
                    error_found = 1;
                    continue;
                }
            }
            free(inst->nums);
            free(inst);
        }
        else {
            command = read_command(str,&error_code);
            if(error_code == 0){
                /*
                 * printf("line %d: label - %s, opcode = %d, source - %s, dest - %s\n",
                       am_file.line_num, command->label, command->opcode, command->source, command->dest);
                */
                IC++;
            }
            else {
                print_external_error(error_code,am_file);
                free(command);
                error_found = 1;
                continue;
            }

            if(command->label != NULL){
                insert_label_table(&label_table,++label_table_line,command->label,IC,am_file,0);
            }
            if(add_machine_code_line(&code, command_to_short(command), NULL, &IC, am_file) == 0){
                free(command);
                error_found = 1;
                continue;
            }
            if(add_extra_machine_code_line(&code,command,&IC,1,am_file) == 0 || \
            add_extra_machine_code_line(&code,command,&IC,0,am_file) == 0){
                free(command);
                error_found = 1;
                continue;
            }
            free(command);
        }
    }
    if (exe_second_pass(file_name,label_table,IC,DC,label_table_line,externs_count,entries_count,\
        code,data,externs,entries,error_found) == 0){
        error_found = 1;
    }
    return error_found;
}