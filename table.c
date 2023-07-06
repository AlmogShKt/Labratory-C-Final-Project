#ifndef LABRATORY_C_FINAL_PROJECT_TABLE_H
#define LABRATORY_C_FINAL_PROJECT_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table.h"
#include "globals.h"
#include "Errors.h"
#include "code_conversion.h"
#include "util.h"

int insert_other_labels(other_table **table, int count, inst_parts *inst, location am_file) {
    other_table *ptr;
    ptr = *table;
    (*table + count - 1)->assembly_line = am_file.line_num;
    (*table + count - 1)->label_name = handle_malloc(strlen(inst->arg_label + 1) * sizeof(char));
    if ((*table + count - 1)->label_name == NULL) {
        return 0;
    }
    strcpy((*table + count - 1)->label_name, inst->arg_label);

    *table = realloc(*table, (count + 1) * sizeof(other_table));
    if (table == NULL) {
        free(ptr);
        return 0;
    }
    return 1;
}

int
insert_label_table(label_address **label_table, int lines, char *label, int counter, location am_file, int is_data) {
    label_address *p_temp;
    p_temp = *label_table;
    *label_table = realloc(*label_table, lines * sizeof(label_address));
    if (*label_table == NULL) {
        free(p_temp);
        return 0;
    }
    (*label_table + lines - 1)->is_data = is_data;
    (*label_table + lines - 1)->address = counter;
    (*label_table + lines - 1)->assembly_line = am_file.line_num;
    (*label_table + lines - 1)->label_name = malloc((strlen(label) + 1) * sizeof(char));
    if ((*label_table + lines - 1)->label_name == NULL) {
        return 0;
    }
    strcpy((*label_table + lines - 1)->label_name, label);
    return 1;
}

int check_each_label_once(label_address *label_table, int table_lines, char *file_name) {
    int i, j;
    for (i = 0; i < table_lines - 1; i++) {
        for (j = i + 1; j < table_lines; j++) {
            if (strcmp((label_table + i)->label_name, (label_table + j)->label_name) == 0) {
                location am_file;
                am_file.file_name = file_name;
                am_file.line_num = (label_table + j)->assembly_line;
                print_external_error(ERROR_CODE_55, am_file);
                return 0;
            }
        }
    }
    return 1;
}

void change_label_table_data_count(label_address *label_table, int table_lines, int IC) {
    int i;
    for (i = 0; i < table_lines; i++) {
        if ((label_table + i)->is_data) {
            (label_table + i)->address += IC + 1;
        }
    }
}

void reset_labels_address(label_address *label_table, int table_lines) {
    int i;
    for (i = 0; i < table_lines; i++) {
        (label_table + i)->address += IC_INIT_VALUE;
    }
}

int replace_externs(code_conv *code, other_table *externs, int externs_count, int count, char *file_name) {
    int i, j, found, error_found;
    error_found = 0;
    for (i = 0; i <= count; i++) {
        found = 0;
        if ((code + i)->label != NULL) {
            for (j = 0; j < externs_count && found == 0; j++) {
                if (strcmp((code + i)->label, (externs + j)->label_name) == 0) {
                    /* by default all labels got value 2, now we know it is an extern, therefore 1 */
                    (code + i)->short_num -= 1;
                    found = 1;
                }
            }
        }
    }
}

int is_extern_defined(other_table *externs, int externs_count, label_address *label_table, int label_table_line,
                      char *file_name) {
    int i, j, found, extern_defined;
    extern_defined = 0;
    for (i = 0; i < externs_count; i++) {
        found = 0;
        for (j = 0; j < label_table_line && found == 0; j++) {
            /* if an extern is stored in the label_table (not allowed!) */
            if (strcmp((externs + i)->label_name, (label_table + j)->label_name) == 0) {
                extern_defined = 1;
                found = 1;
                location am_file;
                am_file.file_name = file_name;
                am_file.line_num = (label_table + j)->assembly_line;
                print_external_error(ERROR_CODE_56, am_file);
            }
        }
    }
    return extern_defined;
}

int replace_labels(code_conv *code, label_address *label_table, int label_table_line, int IC_len, char *file_name) {
    int i, j, found, error_found;
    error_found = 0;
    for (i = 0; i <= IC_len; i++) {
        found = 0;
        /* look for code lines that originated from a label from the first pass, */
        /* and not an extern label that we have already changed its machine code to '1' */
        if ((code + i)->label != NULL && (code + i)->short_num != 1) {
            for (j = 0; j < label_table_line && found == 0; j++) {
                if (strcmp((code + i)->label, (label_table + j)->label_name) == 0) {
                    (code + i)->short_num |= ((label_table + j)->address) << ARE_BITS;
                    found = 1;
                }
            }
            /* if label is not defined in the assembly file */
            if (!found) {
                location am_file;
                am_file.file_name = file_name;
                am_file.line_num = (code + i)->assembly_line;
                print_external_error(ERROR_CODE_36, am_file);
                error_found = 1;
            }
        }
    }
    return !error_found;
}

int print_externs(code_conv *code, int count, other_table *externs, int externs_count, char *file_name) {
    FILE *fp;
    int i, j, found, empty;
    char *temp;
    fp = fopen((temp = add_new_file(file_name, ".ext")), "w");
    empty = 1;
    if (fp == NULL) {
        print_internal_error(ERROR_CODE_7);
        return 0;
    }
    for (i = 0; i <= count; i++) {
        found = 0;
        if ((code + i)->label != NULL) {
            for (j = 0; j < externs_count && found == 0; j++) {
                if (strcmp((code + i)->label, (externs + j)->label_name) == 0) {
                    fprintf(fp, "%s\t%d\n", (externs + j)->label_name, IC_INIT_VALUE + i);
                    found = 1;
                    empty = 0;
                }
            }
        }
    }
    fclose(fp);
    if (empty) {
        remove(temp);
    }
    free(temp);
    return 1;
}

int print_entries(label_address *label_table, int label_table_line, other_table *entries, int entries_count,
                  char *file_name) {
    FILE *fp;
    int i, j, found, empty;
    char *temp;
    fp = fopen((temp = add_new_file(file_name, ".ent")), "w");
    empty = 1;
    if (fp == NULL) {
        print_internal_error(ERROR_CODE_7);
        return 0;
    }
    for (i = 0; i < label_table_line; i++) {
        found = 0;
        for (j = 0; j < entries_count && found == 0; j++) {
            if (strcmp((label_table + i)->label_name, (entries + j)->label_name) == 0) {
                found = 1;
                fprintf(fp, "%s\t%d\n", (entries + j)->label_name, (label_table + i)->address);
                empty = 0;
            }
        }
    }
    fclose(fp);
    if (empty) {
        remove(temp);
    }
    free(temp);
    return 1;
}

void free_label_table(label_address *label_table, int label_table_line) {
    int i;
    for (i = 0; i < label_table_line; i++) {
        free((label_table + i)->label_name);
    }
    free(label_table);
}

void free_other_table(other_table *table, int count) {
    int i;
    for (i = 0; i < count; i++) {
        free((table + i)->label_name);
    }
    free(table);
}

#endif