
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

/**
 * Executes the first pass of the assembly file parsing and processing.
 *
 * @param file_name The name of the assembly file to process.
 * @return An integer indicating whether an error occurred during the first pass.
 *         - If no error occurred, it returns 0.
 *         - If an error occurred, it returns a non-zero value.
 */
int exe_first_pass(char *file_name) {
    int error_code, IC, DC, error_found, label_table_line, externs_count, entries_count, inst_created = 1;

    /* Declare pointers to different data types */
    code_conv *code, *data;
    other_table *externs;
    other_table *entries;
    command_parts *command;
    inst_parts *inst;
    location am_file;
    char str[MAX_LINE_LENGTH];
    char str_copy[MAX_LINE_LENGTH], str_copy_token[MAX_LINE_LENGTH], *token;


    /* Declare a file pointer */
    FILE *fp;
    label_address *label_table;

    /* Initialize the error codes */
    error_code = error_found = 0;

    /* Check if any line in the file exceeds the maximum length */
    if (lines_too_long(file_name)) {
        error_found = 1;
    }

    fp = fopen(file_name, "r");
    am_file.file_name = file_name;
    am_file.line_num = 0;

    label_table_line = externs_count = entries_count = 0;
    label_table = NULL;

    /* Initialize the instruction counter and data counter */
    IC = -1;
    DC = 0;

    /* Handle the memory allocation for externs, entries, code, and data */
    error_found = handle_allocation(&externs, &entries, &code, &data);

    /* Start reading the file line by line until end of file or maximum instruction count */
    while (fgets(str, MAX_LINE_LENGTH, fp) != NULL && IC + DC <= IC_MAX - IC_INIT_VALUE) {
        (am_file.line_num)++;
        if (strcmp(str, "\n") == 0) {
            continue;
        }

        if (strchr(str, '.')) {
            /*Copy the str*/
            strcpy(str_copy, str);
            if (strstr(str_copy, ".entry") || strstr(str_copy, ".extern")) {
                inst = read_entry_or_extern(str_copy, &error_code);
                /*If the 'nums' member of the 'inst' structure is NULL, it means the line does not contain any numbers.
                It's either an .extern or .entry directive.
                not a .extern line --> is a .entry line */
                if (inst->is_extern == 0)
                    insert_other_labels(&entries, ++entries_count, inst, am_file, &error_code);

                    /* is a .extern line */
                else if (inst->is_extern == 1)
                    insert_other_labels(&externs, ++externs_count, inst, am_file, &error_code);

            } else if (strstr(str_copy, ".data") != NULL || strstr(str_copy, ".string") != NULL) {
                /* Parse the instruction */
                inst = read_instruction(str_copy, &error_code);
                if (inst->label != NULL) {
                    insert_label_table(&label_table, ++label_table_line, inst->label, DC, am_file, 1, &error_code);
                }
            } else {
                /*Illegal data line directive - must be .data or .string or .extern or . entry */
                inst_created = 0;
                error_code = ERROR_CODE_58;
            }

            if (error_code != 0) {
                print_external_error(error_code, am_file);
                if (inst_created)
                    free(inst);
                error_found = 1;
                continue;
            } else {
                /* is not .entry nor .extern --> is .data or .string
                /ry to add the data/string to the 'data' machine code.
                If the addition fails (function returns 0), free the 'nums' member and the 'inst' structure,
                set the error flag, and continue to the next line.*/
                if (inst_created) {
                    if (add_machine_code_data(&data, inst, &DC, am_file) == 0) {
                        error_found = 1;
                        continue;
                    }
                }
            }
            /* Free the 'nums' member and the 'inst' structure before processing the next line. */
            if (inst_created) {
                if (inst->nums)
                    free(inst->nums);
                free(inst);
            }
        } else {
            command = read_command(str, &error_code);
            /*
            If the command is parsed successfully (error_code is 0), increment the instruction counter.

            If the 'label' member of the 'command' structure is not NULL, insert the label into the 'label_table'.
            */
            if (error_code == 0) {
                IC++;
            } else {
                print_external_error(error_code, am_file);
                free(command);
                error_found = 1;
                continue;
            }
            /*If the 'label' member of the 'command' structure is not NULL, insert the label into the 'label_table'.*/
            if (command != NULL && command->label != NULL) {
                insert_label_table(&label_table, ++label_table_line, command->label, IC, am_file, 0, &error_code);
            }
            if (add_machine_code_line(&code, command_to_short(command), NULL, &IC, am_file) == 0) {
                free(command);
                error_found = 1;
                continue;
            }
            /*Attempt to add the additional machine code lines related to the command into the 'code' structure.
            If the function fails for either of the additions (returns 0), free the 'command' structure, set the error flag, and continue to the next line.*/
            if (add_extra_machine_code_line(&code, command, &IC, 1, am_file) == 0 || \
            add_extra_machine_code_line(&code, command, &IC, 0, am_file) == 0) {
                free(command);
                error_found = 1;
                continue;
            }
            free(command);
        }
    }

/* Execute the second pass. If it fails, set error_found to 1 */
    if (exe_second_pass(file_name, label_table, IC, DC, label_table_line, externs_count, entries_count, code, data,
                        externs, entries, error_found) == 0) {
        error_found = 1;
    }

    fclose(fp);

/* Return the error status */
    return
            error_found;
}

/**
 * Handles memory allocation for data structures used in the first pass.
 *
 * @param externs A pointer to the pointer of the externs table.
 * @param entries A pointer to the pointer of the entries table.
 * @param code A pointer to the pointer of the code_conv structure.
 * @param data A pointer to the pointer of the code_conv structure for data.
 * @return An integer indicating whether an error occurred during memory allocation.
 *         - If no error occurred, it returns 0.
 *         - If an error occurred, it returns a non-zero value.
 */
int handle_allocation(other_table **externs, other_table **entries, code_conv **code, code_conv **data) {
    int error_found = 0;

    /* Allocate memory for 'externs' using the size of 'other_table' */
    *externs = handle_malloc(sizeof(other_table));
    if (*externs == NULL) {
        error_found = 1;
        return error_found;
    }

    /* Allocate memory for 'entries' using the size of 'other_table' */
    *entries = handle_malloc(sizeof(other_table));
    if (*entries == NULL) {
        error_found = 1;
        return error_found;
    }

    /* Allocate memory for 'code' using the size of 'code_conv' */
    *code = handle_malloc(sizeof(code_conv));
    if (*code == NULL) {
        error_found = 1;
        return error_found;
    }

    /* Allocate memory for 'data' using the size of 'code_conv' */
    *data = handle_malloc(sizeof(code_conv));
    if (*code == NULL) {
        error_found = 1;
        return error_found;
    }

    /* If the program gets to this point, then all memory allocations were successful. */
    /* Return the error flag (should be 0 at this point). */
    return error_found;
}