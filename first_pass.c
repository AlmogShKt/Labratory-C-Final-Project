
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
    int error_code, IC, DC, error_found, label_table_line, externs_count, entries_count;

    /* Declare pointers to different data types */
    code_conv *code, *data;
    other_table *externs;
    other_table *entries;
    command_parts *command;
    inst_parts *inst;
    location am_file;
    char str[MAX_LINE_LENGTH];

    /* Declare a file pointer */
    FILE *fp;

    /* Declare a table to store the labels found in the code */
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

        /* If the line contains a directive */
        if (strstr(str, ".data") != NULL || strstr(str, ".string") != NULL || \
            strstr(str, ".entry") != NULL || strstr(str, ".extern") != NULL) {

            /* Parse the instruction */
            inst = read_instruction(str, &error_code);

            if (error_code != 0) {
                print_external_error(error_code, am_file);
                free(inst);
                error_found = 1;
                continue;
            }

            /*
            If the 'nums' member of the 'inst' structure is NULL, it means the line does not contain any numbers.
            It's either an .extern or .entry directive.
            */
            if (inst->nums == NULL) {
                /* not a .extern line --> is a .entry line */
                if (inst->is_extern == 0) {
                    if (insert_other_labels(&entries, ++entries_count, inst, am_file) == 0) {
                        free(inst);
                        error_found = 1;
                        continue;
                    }
                }
                    /* is a .extern line */
                else if (inst->is_extern == 1) {
                    if (insert_other_labels(&externs, ++externs_count, inst, am_file) == 0) {
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
                /*
                Try to add the data/string to the 'data' machine code.
                If the addition fails (function returns 0), free the 'nums' member and the 'inst' structure, set the error flag, and continue to the next line.
                */
                if (add_machine_code_data(&data, inst, &DC, am_file) == 0) {
                    free(inst->nums);
                    free(inst);
                    error_found = 1;
                    continue;
                }
            }
            /*
            Free the 'nums' member and the 'inst' structure before processing the next line.
            */
            free(inst->nums);
            free(inst);
        } else {
            command = read_command(str, &error_code);
            /*
            If the command is parsed successfully (error_code is 0), increment the instruction counter.
            */
            if (error_code == 0) {
                /*!
                 * printf("line %d: label - %s, opcode = %d, source - %s, dest - %s\n",
                       am_file.line_num, command->label, command->opcode, command->source, command->dest);
                */
                IC++;
            } else {
                print_external_error(error_code, am_file);
                free(command);
                error_found = 1;
                continue;
            }
            /*
            If the 'label' member of the 'command' structure is not NULL, insert the label into the 'label_table'.
            */
            if (command->label != NULL) {
                insert_label_table(&label_table, ++label_table_line, command->label, IC, am_file, 0);
            }
            /*
           If the 'label' member of the 'command' structure is not NULL, insert the label into the 'label_table'.
           */
            if (add_machine_code_line(&code, command_to_short(command), NULL, &IC, am_file) == 0) {
                free(command);
                error_found = 1;
                continue;
            }
            /*
          Attempt to add the additional machine code lines related to the command into the 'code' structure.
          If the function fails for either of the additions (returns 0), free the 'command' structure, set the error flag, and continue to the next line.
          */
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
    if (exe_second_pass(file_name, label_table, IC, DC, label_table_line, externs_count, entries_count, \
        code, data, externs, entries, error_found) == 0) {
        error_found = 1;
    }

    fclose(fp);

    /* Return the error status */
    return error_found;
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