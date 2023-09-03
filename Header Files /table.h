#ifndef TABLE_H
#define TABLE_H

#include "lexer.h"
#include "globals.h"
#include "code_conversion.h"

/**
 * Represents a label's address and associated information.
 *
 * This struct represents a label's address and its associated information in the assembly code.
 * It includes the address where the label is located, the name of the label, the assembly line where the label is defined,
 * and a flag 'is_data_line' to indicate if the label belongs to a data line.
 */
typedef struct label_address {
    int address;
    char *label_name;
    int assembly_line;
    int is_data_line;
} label_address;


/**
 * Represents other labels (non-instruction labels) and their assembly line information.
 *
 * This struct represents information about other labels (non-instruction labels) in the assembly code.
 * It includes the name of the label and the assembly line number where the label is used.
 */
typedef struct other_table {
    char *label_name;
    int assembly_line;
} other_table;


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
int insert_other_labels(other_table **table, int count, inst_parts *inst, location am_file, int *error_code);


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
insert_label_table(label_address **label_table, int lines, char *label, int counter, location am_file, int is_data_line,
                   int *error_code);

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
int check_each_label_once(label_address *label_table, int lines, char *file_name);


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
void change_label_table_data_count(label_address *label_table, int table_lines, int IC);


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
void reset_labels_address(label_address *label_table, int table_lines);

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
void replace_externs(code_conv *code, other_table *externs, int externs_count, int count, char *file_name);


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
                      char *file_name);

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
int replace_labels(code_conv *code, label_address *label_table, int label_table_line, int IC_len, char *file_name);

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
int print_externs(code_conv *code, int count, other_table *externs, int externs_count, char *file_name);


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
                  char *file_name);

/**
 * Frees the memory allocated for the other_table structure.
 * @param table The other_table structure to be freed.
 * @param count The number of elements in the other_table.
 */
void free_other_table(other_table *table, int count);

/**
 * Frees the memory allocated for the label_table structure.
 * @param label_table The label_table structure to be freed.
 * @param label_table_line The number of elements in the label_table.
 */
void free_label_table(label_address *label_table, int label_table_line);

#endif