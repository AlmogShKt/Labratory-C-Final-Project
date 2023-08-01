#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "code_conversion.h"
#include "Errors.h"
#include "globals.h"
#include "table.h"
#include "util.h"
#include "lexer.h"

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
int inc_mem(code_conv **code, int counter) {
    code_conv *ptr;
    ptr = *code;
    *code = realloc(*code, (counter + 1) * sizeof(code_conv));
    if (*code == NULL) {
        print_internal_error(ERROR_CODE_1);
        free(ptr);
        return 0;
    }
    return 1;
}

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
unsigned short command_to_short(command_parts *command) {
    unsigned short n_src, n_op, n_dest;
    n_src = n_op = n_dest = 0;
    if (what_reg(command->source) >= 0) {
        n_src = REG_ADDRESSING << SOURCE_BITS_SHIFT;
    } else if (legal_label(command->source)) {
        n_src = (short) (LABEL_ADDRESSING << SOURCE_BITS_SHIFT);
    } else if (is_num(command->source)) {
        n_src = (short) (DIRECT_ADDRESSING << SOURCE_BITS_SHIFT);
    }
    if (what_reg(command->dest) >= 0) {
        n_dest = (short) (REG_ADDRESSING << DEST_BITS_SHIFT);
    } else if (legal_label(command->dest)) {
        n_dest = (short) (LABEL_ADDRESSING << DEST_BITS_SHIFT);
    } else if (is_num(command->dest)) {
        n_dest = (short) (DIRECT_ADDRESSING << DEST_BITS_SHIFT);
    }
    n_op = (short) (command->opcode) << OPCODE_BITS_SHIFT;
    return ((n_src | n_op) | n_dest);
}

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
unsigned short reg_to_short(command_parts *command, int reg_src) {
    static int already_done;
    int reg1, reg2;
    unsigned short n_reg_src, n_reg_dest;
    n_reg_src = n_reg_dest = 0;
    if (reg_src) {
        if ((reg1 = what_reg(command->source)) >= 0) {
            n_reg_src = reg1 << SOURCE_BITS_SHIFT_REG;
        }
        if ((reg2 = what_reg(command->dest)) >= 0) {
            n_reg_dest = reg2 << DEST_BITS_SHIFT_REG;
        }
        already_done = 1;
        return (n_reg_src | n_reg_dest);
    } else if (already_done == 0) {
        if ((reg2 = what_reg(command->dest)) >= 0) {
            n_reg_dest = reg2 << DEST_BITS_SHIFT_REG;
        }
        return n_reg_dest;
    }
    already_done = 0;
    return DOUBLE_REGS_VALUE;
}


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
int add_machine_code_line(code_conv **code, unsigned short num, char *str, int *IC, location am_file) {
    if (inc_mem(code, *IC) == 0) {
        return 0;
    }
    (*code + *IC)->short_num = num;
    (*code + *IC)->assembly_line = am_file.line_num;
    if (str == NULL) {
        (*code + *IC)->label = NULL;
    } else {
        (*code + *IC)->label = handle_malloc((strlen(str) + 1) * sizeof(char));
        if ((*code + *IC)->label == NULL) {
            return 0;
        }
        strcpy((*code + *IC)->label, str);
    }
    return 1;
}


/**
 * @brief This function adds an extra machine code line to the code_conv array based on the command and operand.
 *
 * The function takes a code_conv array, a command_parts structure, an instruction counter (IC),
 * an indicator for the source operand (is_src), and a location structure representing the file position.
 * It determines the appropriate machine code value based on the operand type and adds it as an extra line to the array.
 * The IC is incremented accordingly.
 *
 * @param code: A pointer to a pointer to the code_conv array.
 * @param command: A pointer to the command_parts structure containing the command information.
 * @param IC: A pointer to the instruction counter (IC) variable.
 * @param is_src: An indicator to determine if the source operand should be processed.
 * @param am_file: A location structure representing the file position.
 * @return Returns 1 if the extra machine code line was added successfully or had nothing to add, and 0 otherwise.
 */
int add_extra_machine_code_line(code_conv **code, command_parts *command, int *IC, int is_src, location am_file) {
    unsigned short num;
    char *arg;
    arg = (is_src) ? command->source : command->dest;
    if (what_reg(arg) > 0 && (num = reg_to_short(command, is_src)) != DOUBLE_REGS_VALUE) {
        (*IC)++;
        if (add_machine_code_line(code, num, NULL, IC, am_file) == 0) {
            return 0;
        }
    } else if (legal_label(arg)) {
        (*IC)++;
        if (add_machine_code_line(code, RELOCATABLE_VALUE, arg, IC, am_file) == 0) {
            return 0;
        }
    } else if (is_num(arg)) {
        (*IC)++;
        /* representing number in 2-11 bits, therefore pushing the number ARE_BITS bits to the left */
        if (add_machine_code_line(code, atoi(arg) << ARE_BITS, NULL, IC, am_file) == 0) {
            return 0;
        }
    }
    /* added successfully or had nothing to add */
    return 1;

}


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
int add_machine_code_data(code_conv **data, inst_parts *inst, int *DC, location am_file) {
    int i;
    int inst_len;
    inst_len = inst->len;
    /*Check if data pointer is NULL before proceeding*/
    if (data == NULL || *data == NULL) {
        /* Handle the error appropriately */
        return 0;
    }

    for (i = 0; i < inst_len; i++) {
        if (inc_mem(data, *DC) == 0) {
            return 0;
        }
        (*data)[*DC].short_num = inst->nums[i]; // Updated this line
        (*data)[*DC].label = NULL; // Updated this line
        (*data)[*DC].assembly_line = am_file.line_num; // Updated this line
        (*DC)++;
    }
    return 1;
}
/*
void print_binary_code(code_conv *code,int IC_len){
    int i;
    char *bin_num;
    for (i = 0; i <= IC_len; i++){
        bin_num = short_to_binary((code + i)->short_num);
        printf("Assembly line %d, Code address %d binary code is: %s\n",\
        (code + i)->assembly_line,+IC_INIT_VALUE+i, bin_num);
    }
}
*/

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
int merge_code(code_conv **code, code_conv *data, int IC, int DC) {
    int i;
    code_conv *ptr;
    ptr = *code;
    if (inc_mem(code, IC + DC) == 0) {
        free(ptr);
        free(data);
        return 0;
    }
    /* coping the info from the data lines into the end of the command code lines */
    for (i = 0; i < DC; i++) {
        (*code + IC + i + 1)->label = (data + i)->label;
        (*code + IC + i + 1)->assembly_line = (data + i)->assembly_line;
        (*code + IC + i + 1)->short_num = (data + i)->short_num;
    }
    free(data); /* no need anymore for the code from the data */
    return 1;
}

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
void free_code(code_conv *code, int code_count) {
    int i;
    for (i = 0; i <= code_count; i++) {
        if ((code + i)->label != NULL) {
            free((code + i)->label);
        }
    }
    free(code);
}