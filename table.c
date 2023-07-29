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

/**
 * @brief This function inserts other labels into the other_table.
 *
 * The function takes a pointer to the other_table (table), the count of labels (count), the inst_parts structure (inst),
 * and the location of the assembly file (am_file).
 * It updates the assembly line and label name for the new label entry in the table.
 * It then reallocates memory for the table to accommodate the new entry.
 *
 * @param table: Pointer to the other_table.
 * @param count: Count of labels.
 * @param inst: The inst_parts structure containing the label information.
 * @param am_file: The location of the assembly file.
 * @return Returns 1 if the label is inserted successfully, or 0 if memory allocation fails.
 */
int insert_other_labels(other_table **table, int count, inst_parts *inst, location am_file, int *error_code) {
    other_table *ptr;
    ptr = *table;
    (*table + count - 1)->assembly_line = am_file.line_num;
    (*table + count - 1)->label_name = handle_malloc(strlen(inst->arg_label + 1) * sizeof(char));
    if ((*table + count - 1)->label_name == NULL) {
        *error_code = ERROR_CODE_1;
        return 0;
    }
    strcpy((*table + count - 1)->label_name, inst->arg_label);

    *table = realloc(*table, (count + 1) * sizeof(other_table));
    if (table == NULL) {
        *error_code = ERROR_CODE_1;
        free(ptr);
        return 0;
    }
    return 1;
}

/**
 * @brief This function inserts a label entry into the label_table.
 *
 * The function takes a pointer to the label_table (label_table), the number of lines (lines),
 * the label name (label), the counter value (counter), the location of the assembly file (am_file),
 * and a flag indicating whether it is a data label (is_data_line_line).
 *
 * It reallocates memory for the label_table to accommodate the new label entry.
 * It updates the is_data_line, address, assembly_line, and label_name fields for the new label entry.
 *
 * @param label_table: Pointer to the label_table.
 * @param lines: Number of lines.
 * @param label: The label name.
 * @param counter: The counter value.
 * @param am_file: The location of the assembly file.
 * @param is_data_line: Flag indicating whether it is a data label.
 * @return Returns 1 if the label is inserted successfully, or 0 if memory allocation fails.
 */
int
insert_label_table(label_address **label_table, int lines, char *label, int counter, location am_file, int is_data_line, int *error_code) {
    label_address *p_temp;
    p_temp = *label_table;
    *label_table = realloc(*label_table, lines * sizeof(label_address));
    if (*label_table == NULL) {
        *error_code = ERROR_CODE_1;
        free(p_temp);
        return 0;
    }
    (*label_table + lines - 1)->is_data_line = is_data_line;
    (*label_table + lines - 1)->address = counter;
    (*label_table + lines - 1)->assembly_line = am_file.line_num;
    (*label_table + lines - 1)->label_name = malloc((strlen(label) + 1) * sizeof(char));
    if ((*label_table + lines - 1)->label_name == NULL) {
        *error_code = ERROR_CODE_1;
        return 0;
    }
    strcpy((*label_table + lines - 1)->label_name, label);
    return 1;
}

/**
 * @brief This function checks each label in the label_table to ensure uniqueness.
 *
 * The function takes a pointer to the label_table (label_table), the number of lines in the table (table_lines),
 * and the name of the assembly file (file_name).
 *
 * It iterates over each label entry in the label_table and compares it with the other entries to check for duplicates.
 * If a duplicate label is found, it prints an error message indicating the line number and file name where the duplicate label occurs.
 *
 * @param label_table: Pointer to the label_table.
 * @param table_lines: Number of lines in the label_table.
 * @param file_name: The name of the assembly file.
 * @return Returns 1 if all labels are unique, or 0 if a duplicate label is found.
 */
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

/**
 * @brief This function updates the addresses in the label_table for data lines.
 *
 * The function takes a pointer to the label_table (label_table), the number of lines in the table (table_lines),
 * and the instruction counter (IC).
 *
 * It iterates over each entry in the label_table and checks if the entry corresponds to a data line.
 * If it does, the address value in the entry is updated by adding the IC + 1.
 *
 * @param label_table: Pointer to the label_table.
 * @param table_lines: Number of lines in the label_table.
 * @param IC: The instruction counter.
 */
void change_label_table_data_count(label_address *label_table, int table_lines, int IC) {
    int i;
    for (i = 0; i < table_lines; i++) {
        if ((label_table + i)->is_data_line) {
            (label_table + i)->address += IC + 1;
        }
    }
}


/**
 * @brief This function resets the addresses in the label_table to their initial values.
 *
 * The function takes a pointer to the label_table (label_table) and the number of lines in the table (table_lines).
 *
 * It iterates over each entry in the label_table and resets the address value by adding the IC_INIT_VALUE.
 * IC_INIT_VALUE represents the initial value for the address.
 *
 * @param label_table: Pointer to the label_table.
 * @param table_lines: Number of lines in the label_table.
 */
void reset_labels_address(label_address *label_table, int table_lines) {
    int i;
    for (i = 0; i < table_lines; i++) {
        (label_table + i)->address += IC_INIT_VALUE;
    }
}


/**
 * @brief This function replaces the externs in the code with the appropriate values.
 *
 * The function takes the code array (code), the externs table (externs), the number of externs (externs_count),
 * the number of elements in the code array (count), and the file name (file_name).
 *
 * It iterates over each element in the code array and checks if it has a label.
 * If the label matches an entry in the externs table, it updates the short_num value in the code array
 * to indicate that the label is an extern (subtracting 1 from the default value of 2).
 *
 * @param code: Pointer to the code array.
 * @param externs: Pointer to the externs table.
 * @param externs_count: Number of externs in the externs table.
 * @param count: Number of elements in the code array.
 * @param file_name: Name of the file being processed.
 */
void replace_externs(code_conv *code, other_table *externs, int externs_count, int count, char *file_name) {
    int i, j, found;
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


/**
 * @brief This function checks if any externs are defined in the label table.
 *
 * The function takes the externs table (externs), the number of externs (externs_count),
 * the label table (label_table), the number of lines in the label table (label_table_line),
 * and the file name (file_name).
 *
 * It iterates over each extern in the externs table and checks if it is stored in the label table.
 * If an extern is found in the label table, it sets the extern_defined flag to 1, indicating that
 * an extern is defined, and prints an appropriate error message.
 *
 * @param externs: Pointer to the externs table.
 * @param externs_count: Number of externs in the externs table.
 * @param label_table: Pointer to the label table.
 * @param label_table_line: Number of lines in the label table.
 * @param file_name: Name of the file being processed.
 * @return 1 if an extern is defined in the label table, 0 otherwise.
 */
int is_extern_defined(other_table *externs, int externs_count, label_address *label_table, int label_table_line,
                      char *file_name) {
    int i, j, found, extern_defined;
    location am_file;
    extern_defined = 0;
    for (i = 0; i < externs_count; i++) {
        found = 0;
        for (j = 0; j < label_table_line && found == 0; j++) {
            /* if an extern is stored in the label_table (not allowed!) */
            if (strcmp((externs + i)->label_name, (label_table + j)->label_name) == 0) {
                extern_defined = 1;
                found = 1;
                am_file.file_name = file_name;
                am_file.line_num = (label_table + j)->assembly_line;
                print_external_error(ERROR_CODE_56, am_file);
            }
        }
    }
    return extern_defined;
}


/**
 * @brief This function replaces labels in the code with their corresponding addresses.
 *
 * The function takes the code table (code), the label table (label_table), the number of lines in the label table (label_table_line),
 * the length of the instruction code (IC_len), and the file name (file_name).
 *
 * It iterates over each code line in the code table and checks if it originated from a label from the first pass
 * and is not an extern label that has already been changed to '1' in the machine code. For such lines, it searches
 * the label table for the corresponding label name and replaces the machine code's operand field with the label's address.
 * If a label is not found in the label table, it prints an appropriate error message.
 *
 * @param code: Pointer to the code table.
 * @param label_table: Pointer to the label table.
 * @param label_table_line: Number of lines in the label table.
 * @param IC_len: Length of the instruction code.
 * @param file_name: Name of the file being processed.
 * @return 1 if all labels are successfully replaced, 0 if there are undefined labels.
 */
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

/**
 * @brief This function prints the external symbols to an external file.
 *
 * The function takes the code table (code), the number of lines in the code table (count),
 * the externs table (externs), the number of externs (externs_count), and the file name (file_name).
 *
 * It opens a new file with the extension ".ext" using the given file name, and iterates over each code line in the code table.
 * For each line that has a non-null label, it searches the externs table for a matching label name.
 * If a match is found, it writes the label name and the corresponding instruction counter value to the external file.
 * If the external file is empty, it is removed.
 *
 * @param code: Pointer to the code table.
 * @param count: Number of lines in the code table.
 * @param externs: Pointer to the externs table.
 * @param externs_count: Number of externs.
 * @param file_name: Name of the file being processed.
 * @return 1 if the external symbols are successfully printed, 0 if an error occurs.
 */
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


/**
 * @brief This function prints the entry symbols to an entry file.
 *
 * The function takes the label table (label_table), the number of lines in the label table (label_table_line),
 * the entries table (entries), the number of entries (entries_count), and the file name (file_name).
 *
 * It opens a new file with the extension ".ent" using the given file name, and iterates over each entry in the entries table.
 * For each entry, it searches the label table for a matching label name.
 * If a match is found, it writes the label name and the corresponding address to the entry file.
 * If the entry file is empty, it is removed.
 *
 * @param label_table: Pointer to the label table.
 * @param label_table_line: Number of lines in the label table.
 * @param entries: Pointer to the entries table.
 * @param entries_count: Number of entries.
 * @param file_name: Name of the file being processed.
 * @return 1 if the entry symbols are successfully printed, 0 if an error occurs.
 */
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


/**
 * Frees the memory allocated for the label_table structure.
 * @param label_table The label_table structure to be freed.
 * @param label_table_line The number of elements in the label_table.
 */
void free_label_table(label_address *label_table, int label_table_line) {
    int i;
    for (i = 0; i < label_table_line; i++) {
        free((label_table + i)->label_name);
    }
    free(label_table);
}


/**
 * Frees the memory allocated for the other_table structure.
 * @param table The other_table structure to be freed.
 * @param count The number of elements in the other_table.
 */
void free_other_table(other_table *table, int count) {
    int i;
    for (i = 0; i < count; i++) {
        free((table + i)->label_name);
    }
    free(table);
}

#endif