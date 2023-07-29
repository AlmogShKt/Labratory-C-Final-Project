#ifndef C_PROJECT_LEXER_H
#define C_PROJECT_LEXER_H

typedef struct op_code {
    char *opcode;
    int arg_num;
} op_code;

typedef struct command_parts {
    char *label;
    int opcode;
    char *source;
    char *dest;
    char *extra;
} command_parts;

typedef struct inst_parts {
    char *label;
    short *nums;
    int len;    /* the number of data in *nums including '0' at the end of a string */
    char *arg_label;
    int is_extern;
} inst_parts;

inst_parts *read_entry_or_extern(char *str ,int *error_code);
/**
 * This function checks if a line in the source file is too long
 * @param file_name a string to the name of the source file
 * @return 1 if there's a line who is too long, 0 if all the lines are ok
 */
int lines_too_long(char *file_name);

/**
 * This function checks if the argument is ".data" or ".string" or ".extern" or ".entry"
 * @param str a string to check
 * @return 1 if yes, 0 if not
 */
int is_instr(char *str);

/**
 * This function returns the numerical value of the opcode
 * @param str a string that contains the name of the opcode
 * @return the numerical value of the opcode between 0-16, or -1 if opcode is incorrect or sting is empty
 */
int what_opcode(char *str);

/**
 * This function returns the numerical value of the register
 * @param str a string that contains the name of the register
 * @return the numerical value of the opcode between 0-7, or -1 if register is incorrect or sting is empty
 */
int what_reg(char *str);

/**
 * This function checks if the string contains a legal form of a label declaration and the label is legal
 * @param str a string that contains the input from the command or data line that might be a label
 * @return 1 if the the string corresponds to a legal declaration of a label and the label is legal, 0 if not
 */
int legal_label_decl(char *str, int * error_code);

int check_invalid_char(char *str, int *error_code);


/**
 * This function checks if the string is a legal label name
 * @param str a string that contains the input from the command or data line that might be a label
 * @return 1 if the string corresponds to a legal label name, 0 if not
 */
int legal_label(char *str);

/**
 * This function comes after strtok on a string, and checks if the string that from the point that strtok ended
 * has more characters or did it come to it's end
 * @return 1 if the string did not came to it's end, 0 if it did come to it's end
 */
int extra_text();



/**
 * @brief Adds a space character after the colon (:) in the given string.
 *
 * This function searches for the first occurrence of a colon (:) in the input string.
 * If a colon is found, it adds a space character immediately after the colon.
 *
 * @param str Pointer to the input string. The function modifies the string in-place.
 * @param error_code Pointer to an integer variable to store error codes.
 *                   If an error occurs during memory reallocation, the error_code will be set to ERROR_CODE_1.
 *
 * @return 1 on success (if a colon is found and space added), 0 if no colon is found,
 *         or the memory reallocation fails and error_code is set to ERROR_CODE_1.
 */
int add_space_after_colon(char **str, int *error_code);


/**
 * This function checks if a string is either a legal label or a legal register
 * @param str a string that contains the input from the command or data line that might be a label
 * @return 1 if the string is either a legal label or a legal register, 0 if not
 */
int is_reg_or_label(char *str);

void check_reg_error(int *error_code, char *str);


/**
 * This function checks if a string is a legal number
 * @param str
 * @return
 */
int is_num(char *str);

int is_reg_or_label_or_num(char *str);

int legal_arg(char *str, command_parts *command, int *error_code);

command_parts *read_command(char *str, int *error_code);

int capture_nums(char *str, char *token_copy, inst_parts *inst, int *error_code);

inst_parts *read_instruction(char *str, int *error_code);

int inc_array(inst_parts **inst, int len);

int opcode_err_check(char *str);

int count_occurr(char *str, char ch);


#endif
