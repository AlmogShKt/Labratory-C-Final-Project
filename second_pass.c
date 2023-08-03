#include <stdio.h>
#include "second_pass.h"
#include "table.h"
#include "Errors.h"
#include "util.h"

void free_all_memory(code_conv *code, label_address *label_table, other_table *entries, other_table *externs, \
    int code_count, int label_table_line, int entries_count, int externs_count) {
    free_code(code, code_count);
    free_label_table(label_table, label_table_line);
    free_other_table(entries, entries_count);
    free_other_table(externs, externs_count);
}

int conv_code_base64(code_conv *code, int count, char *file_name, int IC, int DC) {
    int i;
    FILE *fp;
    char *ob_file_name, *base64_char;

    /* Create the output file name with a '.ob' extension */
    ob_file_name = add_new_file(file_name, ".ob");

    /*Open the file and handle if any error occurred*/
    fp = fopen(ob_file_name, "w");
    if (fp == NULL) {
        print_internal_error(ERROR_CODE_7);
        return 0;
    }

    /* Write the values of IC and DC to the first line of the output file */
    fprintf(fp, "%d %d\n", IC + 1, DC);

    /* Convert each machine code to base64 and write it to the output file */
    for (i = 0; i <= count; i++) {
        base64_char = short_to_base64((code + i)->short_num);
        fprintf(fp, "%s\n", base64_char);
        free(base64_char);
    }

    /*Free Memory*/
    free(ob_file_name);

    /*Close file*/
    fclose(fp);

    return 1;
}


int exe_second_pass(char *file_name, label_address *label_table, int IC, int DC, int label_table_line, \
    int externs_count, int entries_count, code_conv *code, code_conv *data, other_table *externs, other_table *entries, \
    int error_found) {

    /* Check if the Instruction Counter (IC) exceeds the maximum value */
    if (IC > IC_MAX) {
        print_internal_error(ERROR_CODE_54);
        error_found = 1;
    }

    /* Check if each label in the label_table appears only once (no duplicate labels) */
    if (check_each_label_once(label_table, label_table_line, file_name) == 0) {
        error_found = 1;
    }

    /* Check if any extern label is defined in the assembly file (not allowed!) */
    if (is_extern_defined(externs, externs_count, label_table, label_table_line, file_name) == 1) {
        error_found = 1;
    }

    /* Change the data count in the label_table to the appropriate address */
    change_label_table_data_count(label_table, label_table_line, IC);

    /* Reset the addresses in the label_table to the appropriate addresses after merging the code and data */
    reset_labels_address(label_table, label_table_line);

    /* Merge the code and data arrays into a single array */
    if (merge_code(&code, data, IC, DC) == 0) {
        error_found = 1;
    }

    /* Replace the extern labels with the appropriate machine code (short_num = 1) in the code array */
    replace_externs(code, externs, externs_count, IC + DC, file_name);

    /* Replace the labels in the code array with their corresponding addresses in the label_table */
    if (replace_labels(code, label_table, label_table_line, IC, file_name) == 0) {
        error_found = 1;
    }
    /*! print_binary_code(code,IC+DC); */

    /* Convert the assembled code to base64 format and print it to the output file */
    if (error_found == 0) {
        conv_code_base64(code, IC + DC, file_name, IC, DC);

        /* Print the extern labels and their addresses to the '.ext' output file */
        print_externs(code, IC + DC, externs, externs_count, file_name);
        /* Print the entry labels and their addresses to the '.ent' output file */
        print_entries(label_table, label_table_line, entries, entries_count, file_name);
    }

    /* Free all the allocated memory and resources used during the second pass */
    free_all_memory(code, label_table, entries, externs, IC + DC, label_table_line, entries_count, externs_count);

    /* Return 1 if the second pass is executed successfully without errors, 0 otherwise */
    return !error_found;
}