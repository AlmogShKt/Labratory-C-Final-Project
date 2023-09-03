#ifndef C_PROJECT_CODE_CONVERSION_H
#define C_PROJECT_CODE_CONVERSION_H


#include "lexer.h"
#include "globals.h"

#define SOURCE_BITS_SHIFT 9 /* number of shift left needed to reach the source operand LSB 9-11 */
#define OPCODE_BITS_SHIFT 5 /* number of shift left needed to reach the opcode LSB 5-8 */
#define DEST_BITS_SHIFT 2 /* number of shift left needed to reach the dest operand LSB 2-4 */

#define SOURCE_BITS_SHIFT_REG 7 /* number of shift left needed to reach the source register operand LSB 7-11 */
#define DEST_BITS_SHIFT_REG 2 /* number of shift left needed to reach the dest register operand LSB 2-6 */

/* A,R,E for addressing methods values */
#define DIRECT_ADDRESSING 1
#define LABEL_ADDRESSING 3
#define REG_ADDRESSING 5

/* value to indicate that both registers have been dealt with already to avoid dealing twice with dest register */
#define DOUBLE_REGS_VALUE 10000

/* Memory limit */
#define IC_MAX 1023

/**
 * @brief Represents a converted line of assembly code.
 *
 * This struct stores the converted information of an assembly line, including the
 * unsigned short numerical representation, an optional label associated with the line,
 * and the corresponding assembly line number.
 */
typedef struct code_conv {
    unsigned short short_num; /* The numerical representation of the converted line */
    char *label; /* An optional label associated with the line */
    int assembly_line; /* The corresponding assembly line number */
} code_conv;


/**
 * @brief This function increments the memory allocation for a code_conv array.
 *
 * The function takes a pointer to a code_conv array and increases its memory allocation
 * by reallocating the array with a larger size indicated by the counter parameter.
 * If the reallocation is successful, the function returns 1. If the reallocation fails,
 * an internal error is printed, the original memory is freed, and the function returns 0.
 *
 * @param code: A pointer to a pointer to the code_conv array.
 * @param counter: The desired new size of the array.
 * @return Returns 1 if the memory allocation was successful, and 0 otherwise.
 */
int inc_mem(code_conv **code, int IC);

/**
 * @brief This function converts a command_parts structure to an unsigned short representing the machine code.
 *
 * The function takes a command_parts structure and converts the source and destination operands,
 * as well as the opcode, into bit representations according to the addressing modes.
 * The converted values are then combined to form the final machine code, which is returned as an unsigned short.
 *
 * @param command: A pointer to the command_parts structure containing the command information.
 * @return Returns an unsigned short representing the machine code of the command.
 */
unsigned short command_to_short(command_parts *command);

/**
 * @brief This function adds machine code data to the data array.
 *
 * The function takes a data array, an inst_parts structure containing the data values (inst),
 * a data counter (DC), and a location structure representing the file position.
 * It iterates over the data values in the inst_parts structure and adds each value to the data array.
 * The DC is incremented accordingly.
 *
 * @param data: A pointer to a pointer to the code_conv data array.
 * @param inst: The inst_parts structure containing the data values.
 * @param DC: A pointer to the data counter (DC) variable.
 * @param am_file: A location structure representing the file position.
 * @return Returns 1 if the machine code data was added successfully, and 0 otherwise.
 */
int add_machine_code_data(code_conv **data, inst_parts *inst, int *DC, location am_file);

/**
 * @brief This function adds a machine code line to the code_conv array.
 *
 * The function takes a code_conv array, a machine code value (num), a string representing a label (str),
 * an instruction counter (IC), and a location structure representing the file position.
 * It allocates memory for the new code_conv element, sets its properties, and adds it to the array at the specified IC.
 * The IC is incremented accordingly.
 *
 * @param code: A pointer to a pointer to the code_conv array.
 * @param num: The machine code value to be added.
 * @param str: The label associated with the machine code line (can be NULL).
 * @param IC: A pointer to the instruction counter (IC) variable.
 * @param am_file: A location structure representing the file position.
 * @return Returns 1 if the machine code line was added successfully, and 0 otherwise.
 */
int add_machine_code_line(code_conv **code, unsigned short num, char *str, int *IC, location am_file);

/**
 * This function prints to stdout the machine words of the source file in binary representation
 * @param code a pointer to the struct that contains the machine words
 * @param IC_len the number of lines in the code struct
 */
void print_binary_code(code_conv *code, int IC_len);

/**
 * @brief This function merges the code and data arrays into a single code array.
 *
 * The function takes a pointer to a pointer to the code_conv code array (code),
 * a code_conv data array (data), the instruction counter (IC), and the data counter (DC).
 * It expands the code array to accommodate the data array and copies the data from the data array
 * into the code array starting from the IC position.
 *
 * @param code: A pointer to a pointer to the code_conv code array.
 * @param data: The code_conv data array to be merged.
 * @param IC: The instruction counter (IC).
 * @param DC: The data counter (DC).
 * @return Returns 1 if the merge was successful, and 0 otherwise.
 */
int merge_code(code_conv **code, code_conv *data, int IC, int DC);

/**
 * This function converts the optional second and third code words from a command line to machine
 * wards and stores them in the array of struct which contains the machine code of the command lines
 * @param code a double pointer to an array of struct which contains the machine code of the command lines
 * @param command a pointer to a command struct which contains the parsed information from the command line
 * @param IC the machine code counter of the commands from the source file
 * @param is_src 1 if the the converted information originates from the source part of the command, 0 if from the target
 * @param am_file a pointer to a struct which contains the source file name and line information
 * @return 1 if the conversion and store of the machine word succeeded, 0 if not
 */
int add_extra_machine_code_line(code_conv **code, command_parts *command, int *IC, int is_src, location am_file);

/**
 * @brief This function converts register operands to their corresponding bit representation in the machine code.
 *
 * The function takes a command_parts structure and an indicator for the source register (reg_src).
 * If reg_src is true, the function converts both the source and destination registers to their bit representation
 * and returns the combined value.
 * If reg_src is false and the already_done flag is not set, the function converts only the destination register
 * to its bit representation and returns the value.
 * The already_done flag is used to ensure that the function returns the correct value for double-register instructions.
 *
 * @param command: A pointer to the command_parts structure containing the command information.
 * @param reg_src: An indicator to determine if both source and destination registers should be converted.
 * @return Returns the bit representation of the register operand(s) in the machine code.
 */
unsigned short reg_to_short(command_parts *command, int reg_src);

/**
 * @brief This function frees the memory allocated for the code_conv code array.
 *
 * The function takes a pointer to the code_conv code array (code) and the count of elements in the array (code_count).
 * It iterates through the code array and frees the memory allocated for the label strings, if present.
 * Finally, it frees the memory allocated for the code array itself.
 *
 * @param code: The code_conv code array to be freed.
 * @param code_count: The count of elements in the code array.
 */
void free_code(code_conv *code, int code_count);


#endif
