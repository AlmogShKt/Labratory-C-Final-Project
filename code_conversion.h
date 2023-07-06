#ifndef C_PROJECT_CODE_CONVERSION_H
#define C_PROJECT_CODE_CONVERSION_H


#include "lexer.h"
#include "globals.h"

#define SOURCE_BITS_SHIFT 9
#define OPCODE_BITS_SHIFT 5
#define DEST_BITS_SHIFT 2

#define SOURCE_BITS_SHIFT_REG 7
#define DEST_BITS_SHIFT_REG 2

#define DIRECT_ADDRESSING 1
#define LABEL_ADDRESSING 3
#define REG_ADDRESSING 5

#define DOUBLE_REGS_VALUE 10000

#define IC_MAX 1023

typedef struct code_conv {
    unsigned short short_num;
    char *label;
    int assembly_line;
} code_conv;


/**
 * This function increases the allocated memory to an array of struct which contains the machine code
 * @param code a double pointer to an array of struct which contains the machine code
 * @param IC the machine code counter of the commands from the source file
 * @return 1 if the new memory allocation succeeded, 0 if not
 */
int inc_mem(code_conv **code, int IC);

/**
 * This function converts a command line to a the first machine word
 * @param command a pointer to a command struct which contains the parsed information from the command line
 * @return the machine word
 */
unsigned short command_to_short(command_parts *command);

/**
 * This function converts a data line to a machine word and saves it in the code array
 * @param data a double pointer to an array of struct which contains the machine code of the data lines
 * @param inst a pointer to a inst struct which contains the parsed information from a data line
 * @param DC a pointer to the machine code counter of the data lines from the source file
 * @param am_file a pointer to a struct which contains the source file name and line information
 * @return 1 if the conversion and store of the machine word succeeded, 0 if not
 */
int add_machine_code_data(code_conv **data, inst_parts *inst, int *DC, location am_file);

/**
 * This function converts a command line to a machine word and saves it in the code array
 * @param code a double pointer to an array of struct which contains the machine code of the command lines
 * @param num the command line converted to a number
 * @param str a string to an optional label in the beginning of the command line in the source file
 * @param IC a pointer to the machine code counter of the command lines from the source file
 * @param am_file a pointer to a struct which contains the source file name and line information
 * @return 1 if the conversion and store of the machine word succeeded, 0 if not
 */
int add_machine_code_line(code_conv **code, unsigned short num, char *str, int *IC, location am_file);

/**
 * This function prints to stdout the machine words of the source file in binary representation
 * @param code a pointer to the struct that contains the machine words
 * @param IC_len the number of lines in the code struct
 */
void print_binary_code(code_conv *code, int IC_len);

/**
 * This function merges the two structs of the machine words originated from the command lines
 * and afterwards the machine words originated from the data lines
 * @param code a double pointer to an array of struct which contains the machine code of the command lines
 * @param data a double pointer to an array of struct which contains the machine code of the data lines
 * @param IC the machine code counter of the commands from the source file
 * @param DC the machine code counter of the data from the source file
 * @return 1 if the merge succeeded, 0 if not
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
 * This function converts 1 or 2 register/s in a command line to a machine word
 * @param command a pointer to a command struct which contains the parsed information from the command line
 * @param reg_src 1 if the register is a source, 0 if the register is a destination
 * @return the machine word of the converted 1 or 2 register/s
 */
unsigned short reg_to_short(command_parts *command, int reg_src);

/**
 * This function frees the allocated memory of an array of struct which contains the machine code
 * of the command lines
 * @param code a pointer to an array of struct which contains the machine code of the command lines
 * @param code_count the number of code word lines
 */
void free_code(code_conv *code, int code_count);


#endif //C_PROJECT_CODE_CONVERSION_H
