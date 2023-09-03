#ifndef LABRATORY_C_FINAL_PROJECT_TABLE_H
#define LABRATORY_C_FINAL_PROJECT_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Header Files /table.h"
#include "../Header Files /globals.h"
#include "../Header Files /Errors.h"
#include "../Header Files /code_conversion.h"
#include "../Header Files /util.h"


int insert_other_labels(other_table **table, int count, inst_parts *inst, location am_file, int *error_code) {
    other_table *ptr;
    int label_length;

    if(inst->arg_label == NULL)
        return 0;
    /* Set 'ptr' to the current table */
    ptr = *table;

    /* Update the assembly_line field of the last entry in the table */
    (*table + count - 1)->assembly_line = am_file.line_num;

    /* Calculate the length of the label_name */
    label_length = strlen(inst->arg_label) + 1;

    /* Allocate memory for the label_name and copy the label from 'inst' */
    (*table + count - 1)->label_name = handle_malloc(label_length * sizeof(char));
    if ((*table + count - 1)->label_name == NULL) {
        *error_code = ERROR_CODE_1;
        return 0;
    }
    /*Copy the data into the table*/
    strcpy((*table + count - 1)->label_name, inst->arg_label);

    /* Resize the table to accommodate the new entry */
    *table = realloc(*table, (count + 1) * sizeof(other_table));
    if (table == NULL) {
        /*Failed to allocate memory*/
        *error_code = ERROR_CODE_1;
        free(ptr);
        return 0;
    }

    /* Return 1 to indicate successful insertion */
    return 1;
}


int
insert_label_table(label_address **label_table, int lines, char *label, int counter, location am_file, int is_data_line,
                   int *error_code) {
    label_address *p_temp;

    p_temp = *label_table;

    /* Resize the label_table to accommodate the new entry */
    *label_table = realloc(*label_table, lines * sizeof(label_address));
    if (*label_table == NULL) {
        *error_code = ERROR_CODE_1;
        free(p_temp);
        return 0;
    }

    /* Set the values for the new entry in the label_table */
    (*label_table + lines - 1)->is_data_line = is_data_line;
    (*label_table + lines - 1)->address = counter;
    (*label_table + lines - 1)->assembly_line = am_file.line_num;

    /* Allocate memory for the label_name and copy the label from the input */
    (*label_table + lines - 1)->label_name = malloc((strlen(label) + 1) * sizeof(char));
    if ((*label_table + lines - 1)->label_name == NULL) {
        *error_code = ERROR_CODE_1;
        return 0;
    }
    strcpy((*label_table + lines - 1)->label_name, label);

    /* Return 1 to indicate successful insertion */
    return 1;
}


int check_each_label_once(label_address *label_table, int table_lines, char *file_name) {
    int i, j;

    /* Iterate through each label in the label_table */
    for (i = 0; i < table_lines - 1; i++) {
        for (j = i + 1; j < table_lines; j++) {
            /* Compare the names of the labels */
            if (strcmp((label_table + i)->label_name, (label_table + j)->label_name) == 0) {
                /* If a duplicate label is found, print an error message and return 0 */
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
        /*Increase by 1 the address of the data line counter*/
        if ((label_table + i)->is_data_line) {
            (label_table + i)->address += IC + 1;
        }
    }
}


void reset_labels_address(label_address *label_table, int table_lines) {
    int i;
    for (i = 0; i < table_lines; i++) {
        /*Set the address to IC Init value (100)*/
        (label_table + i)->address += IC_INIT_VALUE;
    }
}


void replace_externs(code_conv *code, other_table *externs, int externs_count, int count, char *file_name) {
    int i, j, found;

    /* Iterate through each entry in the 'code' array */
    for (i = 0; i <= count; i++) {
        found = 0;
        /* Check if the current code entry has a label */
        if ((code + i)->label != NULL) {

            /* Compare the label with each extern in the 'externs' array */
            for (j = 0; j < externs_count && found == 0; j++) {
                if (strcmp((code + i)->label, (externs + j)->label_name) == 0) {
                    /* by default all labels got value 2, now we know it is an extern, therefore 1 */
                    (code + i)->short_num -= 1;
                    /*Set 'found' to 1 to indicate a match was found*/
                    found = 1;
                }
            }
        }
    }
}


int is_extern_defined(other_table *externs, int externs_count, label_address *label_table, int label_table_line,
                      char *file_name) {
    int i, j, found, extern_defined;
    location am_file;
    extern_defined = 0;

    /* Iterate through each entry in the 'externs' array */
    for (i = 0; i < externs_count; i++) {
        found = 0;

        /* Compare the extern label with each entry in the 'label_table' array */
        for (j = 0; j < label_table_line && found == 0; j++) {

            /* if an extern is stored in the label_table (not allowed!) */
            if (strcmp((externs + i)->label_name, (label_table + j)->label_name) == 0) {
                extern_defined = 1;
                found = 1;
                am_file.file_name = file_name;
                am_file.line_num = (label_table + j)->assembly_line;
                /*Print the error*/
                print_external_error(ERROR_CODE_56, am_file);
            }
        }
    }
    return extern_defined;
}

int replace_labels(code_conv *code, label_address *label_table, int label_table_line, int IC_len, char *file_name) {
    int i, j, found, error_found;
    /*Set the flag as 0*/
    error_found = 0;

    /* Iterate through each entry in the 'code' array */
    for (i = 0; i <= IC_len; i++) {
        found = 0;

        /* Check if the current code entry has a label and its short_num is not 1 (extern label with machine code '1') */
        if ((code + i)->label != NULL && (code + i)->short_num != 1) {

            /* Compare the label with each entry in the 'label_table' array */
            for (j = 0; j < label_table_line && found == 0; j++) {
                if (strcmp((code + i)->label, (label_table + j)->label_name) == 0) {
                    /* Replace the short_num with the address of the label from the 'label_table' */
                    (code + i)->short_num |= ((label_table + j)->address) << ARE_BITS;

                    /*Set 'found' to 1 to indicate a match was found*/
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

    /* Open a new external file with the '.ext' extension */
    fp = fopen((temp = add_new_file(file_name, ".ext")), "w");

    /* Initialize the 'empty' flag to 1, assuming no extern labels are found initially */
    empty = 1;

    /* Check if the file was opened successfully */
    if (fp == NULL) {
        print_internal_error(ERROR_CODE_7);
        return 0;
    }

    /* Iterate through each entry in the 'code' array */
    for (i = 0; i <= count; i++) {
        found = 0;

        /* Check if the current code entry has a label */
        if ((code + i)->label != NULL) {
            /* Compare the label with each entry in the 'externs' array */
            for (j = 0; j < externs_count && found == 0; j++) {
                if (strcmp((code + i)->label, (externs + j)->label_name) == 0) {
                    fprintf(fp, "%s\t%d\n", (externs + j)->label_name, IC_INIT_VALUE + i);
                    /*Set 'found' to 1 to indicate a match was found*/
                    found = 1;
                    /* Set 'empty' to 0 to indicate at least one extern label is found*/
                    empty = 0;
                }
            }
        }
    }
    fclose(fp);

    /* If no extern labels are found, remove the temporary file and free the memory */
    if (empty) {
        remove(temp);
    }

    /* Free the memory allocated for the temporary file name */
    free(temp);

    /* Return 1 if extern labels are found and wrote to the external file, 0 if an error occurs during file creation */
    return 1;
}


int print_entries(label_address *label_table, int label_table_line, other_table *entries, int entries_count,
                  char *file_name) {
    FILE *fp;
    int i, j, found, empty;
    char *temp;

    /* Open a new entry file with the '.ent' extension and check if the operation success*/
    fp = fopen((temp = add_new_file(file_name, ".ent")), "w");
    if (fp == NULL) {
        /*Failed to open the file*/
        print_internal_error(ERROR_CODE_7);
        return 0;
    }
    /*Init the empty flag*/
    empty = 1;

    /* Iterate through each entry in the 'label_table' array */
    for (i = 0; i < label_table_line; i++) {
        found = 0;

        /* Compare the label with each entry in the 'entries' array */
        for (j = 0; j < entries_count && found == 0; j++) {
            if (strcmp((label_table + i)->label_name, (entries + j)->label_name) == 0) {
                /* Print(into the file) the label name and the corresponding address to the entry file */
                fprintf(fp, "%s\t%d\n", (entries + j)->label_name, (label_table + i)->address);

                /*Set 'found' to 1 to indicate a match was found*/
                found = 1;
                /* Set 'empty' to 0 to indicate at least one extern label is found*/
                empty = 0;
            }
        }
    }

    /* Close the entry file */
    fclose(fp);

    /* If no entry labels are found, remove the temporary file and free the memory */
    if (empty) {
        remove(temp);
    }

    /*Free the allocated memory*/
    free(temp);

    /* Return 1 if entry labels are found and printed to the entry file, 0 if an error occurs during file creation */
    return 1;
}


void free_label_table(label_address *label_table, int label_table_line) {
    int i;
    /* Iterate through each entry in the 'label_table' array and free the allocated memory*/
    for (i = 0; i < label_table_line; i++) {
        free((label_table + i)->label_name);
    }

    /* Free the memory allocated for the 'label_table' array itself */
    free(label_table);
}


void free_other_table(other_table *table, int count) {
    int i;
    /* Iterate through each entry in the 'other_table' array and free the allocated memory*/
    for (i = 0; i < count; i++) {
        free((table + i)->label_name);
    }

    /* Free the memory allocated for the 'other_table' array itself */
    free(table);
}

#endif