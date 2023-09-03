#ifndef C_PROJECT_LEXER_H
#define C_PROJECT_LEXER_H

/* Represents an operation code with its associated opcode and argument count */
typedef struct op_code {
    char *opcode;    /* The opcode corresponding to the operation */
    int arg_num;     /* The number of arguments for the operation */
} op_code;

/* Represents different parts of a command */
typedef struct command_parts {
    char *label;     /* The label associated with the command */
    int opcode;      /* The opcode of the command */
    char *source;    /* The source operand of the command */
    char *dest;      /* The destination operand of the command */
} command_parts;

/* Represents different parts of a data line */
typedef struct inst_parts {
    char *label;       /* The label associated with the instruction */
    short *nums;       /* An array of short integers representing instruction data */
    int len;           /* The number of data in *nums including '0' at the end of a string */
    char *arg_label;   /* The label associated with an argument, if applicable */
    int is_extern;     /* Indicates whether the instruction is marked as external */
} inst_parts;


/**
 * @brief Reads an entry or extern directive from a string.
 *
 * The function reads an entry or extern directive from a given string. It tokenizes the string, extracts
 * relevant information, and populates an inst_parts structure with the parsed data. The function handles
 * memory allocation for the inst_parts structure and returns a pointer to it. It also updates the error_code
 * in case of errors.
 *
 * @param str: The input string containing an entry or extern directive.
 * @param error_code: A pointer to an integer to hold the error code if an error occurs.
 * @return Returns a pointer to an inst_parts structure containing the parsed data, or NULL on error.
 */
inst_parts *read_entry_or_extern(char *str ,int *error_code);


/**
 * @brief This function checks whether any line in the provided file exceeds the maximum length limit.
 *
 * It scans through each line of the file, and if it encounters a line longer than `MAX_LINE_LENGTH`,
 * it prints an external error using `ERROR_CODE_30` and sets a flag indicating a long line was found.
 *
 * @param file_name A pointer to a character string representing the file name.
 *
 * @return Returns an integer. If any line in the file exceeds `MAX_LINE_LENGTH`, it returns 1.
 *         Otherwise, it returns 0 indicating all lines are within the limit.
 *
 */
int lines_too_long(char *file_name);


/**
 * @brief Checks if the provided string is an instruction.
 *
 * This function compares the provided string with a list of known instructions.
 * If a match is found, the function will return 1, indicating that the string is an instruction.
 * If no match is found, it returns 0.
 *
 * @param str A pointer to a character string representing the instruction to check.
 *
 * @return Returns an integer. If the string matches one of the known instructions, it returns 1.
 *         Otherwise, if the string does not match any instruction or is NULL, it returns 0.
 */
int is_instr(char *str);


/**
 * @brief This function returns the index of the opcode in the predefined opcode list.
 *
 * It compares the provided string with a list of known opcodes. If a match is found,
 * it returns the index of the matching opcode in the `OPCODES` array. If no match is
 * found or the string is NULL, it returns -1.
 *
 * @param str A pointer to a character string representing the opcode to check.
 *
 * @return Returns an integer representing the index of the opcode in the `OPCODES` array.
 *         If the string does not match any opcode or is NULL, it returns -1.
 **/
int what_opcode(char *str);


/**
 * @brief This function returns the index of the register in the predefined register list.
 *
 * It compares the provided string with a list of known registers. If a match is found,
 * it returns the index of the matching register in the `REGS` array. If no match is
 * found or the string is NULL, it returns -1.
 *
 * @param str A pointer to a character string representing the register to check.
 *
 * @return Returns an integer representing the index of the register in the `REGS` array.
 *         If the string does not match any register or is NULL, it returns -1.
**/
int what_reg(char *str);


/**
 * @brief This function checks if the provided string is a valid label declaration.
 *
 * This function validates the given string as per the label declaration rules:
 * 1. The string must not be NULL.
 * 2. The first character of the label must be alphabetic.
 * 3. The total length of the label must be less than or equal to MAX_LABEL_LENGTH.
 * 4. The label must not be a register or an opcode (i.e., not found in the predefined list of opcodes or registers).
 * 5. After the first character, the label can contain alphabetic or numeric characters.
 * 6. The label must end with a ':' character, followed by a null character '\0'.
 *
 * If all these conditions are satisfied, it returns 1. Otherwise, it returns 0.
 *
 * @param str A pointer to a character string representing the label to validate.
 *
 * @return Returns an integer. If the string is a valid label declaration, it returns 1.
 *         Otherwise, it returns 0.
 */
int legal_label_decl(char *str, int * error_code);


/**
 * @brief This function checks if the provided string is a valid label.
 *
 * This function validates the given string as per the label rules:
 * 1. The string must not be NULL.
 * 2. The first character of the label must be alphabetic.
 * 3. The total length of the label must be less than or equal to MAX_LABEL_LENGTH.
 * 4. The label must not be an opcode or an instruction (i.e., not found in the predefined list of opcodes or instructions).
 * 5. After the first character, the label can contain alphabetic or numeric characters until a space character or a null character '\0' is found.
 *
 * If all these conditions are satisfied, it returns 1. Otherwise, it returns 0.
 *
 * @param str A pointer to a character string representing the label to validate.
 *
 * @return Returns an integer. If the string is a valid label, it returns 1.
 *         Otherwise, it returns 0.
 */
int legal_label(char *str);


/**
 * @brief This function checks if there is any extra text after a newline character in the token stream.
 *
 * This function uses the standard library function `strtok` with the newline character as the delimiter. If there is more
 * text after a newline character (i.e., another token), the function will return 1 indicating that extra text is present.
 * If there is no more text (i.e., no more tokens), it will return 0 indicating that no extra text is present.
 *
 * @return Returns an integer. If there is extra text after a newline character in the token stream, it returns 1.
 *         Otherwise, it returns 0.
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
 * @brief This function checks if the provided string is either a valid register or a valid label.
 *
 * This function uses the 'what_reg' function to check if the string is a valid register and 'legal_label' function to check
 * if the string is a valid label. If either of these conditions is satisfied, it returns 1. Otherwise, it returns 0.
 *
 * @param str A pointer to a character string that is to be validated as a register or a label.
 *
 * @return Returns an integer. If the string is a valid register or a valid label, it returns 1.
 *         Otherwise, it returns 0.
 */
int is_reg_or_label(char *str);


/**
 * @brief Checks for errors related to register usage in a string.
 *
 * The function checks for errors related to register usage in a given string. It verifies if the string contains any
 * whitespace characters using the has_white_space function. If whitespace characters are present, it sets the
 * error_code to ERROR_CODE_33, indicating an error due to whitespace in the register. It also checks if the string
 * is a valid register representation by attempting to convert it to an integer using atoi and checking if it's a non-zero
 * value, preceded by "@r". If this conversion is successful, it sets the error_code to ERROR_CODE_46, indicating an
 * error due to an invalid register representation. If none of these conditions are met, it sets the error_code to
 * ERROR_CODE_33.
 *
 * @param error_code: A pointer to an integer to hold the error code if an error occurs.
 * @param str: The input string to check for register-related errors.
 */
void check_reg_error(int *error_code, char *str);


/**
 * @brief This function checks if the provided string is a valid integer number.
 *
 * This function uses the 'strtol' function from the standard library to convert the string into a long integer.
 * If the entire string can be converted to a number (i.e., if the rest of the string after the conversion is
 * a null character or a space), it returns 1, indicating that the string is a valid integer number.
 * If the string cannot be fully converted to a number, the function returns 0.
 *
 * @param str A pointer to a character string that is to be checked for numeric validity.
 *
 * @return Returns an integer. If the string is a valid integer number, it returns 1. Otherwise, it returns 0.
 */
int is_num(char *str);


/**
 * @brief This function checks if the provided string is a valid register, label, or numeric string.
 *
 * This function uses the 'is_reg_or_label' function to check if the string is a valid register or label
 * and 'is_num' function to check if the string is a valid numeric string. If the string satisfies any
 * of these conditions, it returns 1. Otherwise, it returns 0.
 *
 * @param str A pointer to a character string that is to be validated as a register, label, or a numeric string.
 *
 * @return Returns an integer. If the string is a valid register, label, or numeric string, it returns 1.
 *         Otherwise, it returns 0.
 */
int is_reg_or_label_or_num(char *str);


/**
 * @brief This function checks the legality of an argument for a given command.
 *
 * This function validates an argument string based on the opcode of the given command.
 * It sets an error code if any rule is violated.
 * If the argument is legal, the function also assigns the correct argument (source or destination) to the command.
 *
 * @param str A pointer to a null-terminated string that contains the argument to be validated.
 * @param command A pointer to a command_parts structure for which the argument is to be validated.
 * @param error_code A pointer to an integer in which the function will store an error code if the argument is not legal.
 *
 * @return Returns 1 if the argument is legal and correctly assigned, 0 otherwise.
 *         If the function returns 0, an error code is set in the integer pointed by the 'error_code' argument.
 */
int legal_arg(char *str, command_parts *command, int *error_code);

/**
 * @brief This function reads a command from the input, parses it, and stores it in a command_parts structure.
 *
 * The function tokenizes the input string and allocates a command_parts structure.
 * It checks for a valid label declaration in the line, and if present, stores it in the structure.
 * It then checks for a valid opcode and stores it in the structure.
 * Based on the opcode, it determines the number of arguments expected and validates the arguments accordingly.
 * The function returns a pointer to the command_parts structure if the command was successfully parsed, and NULL otherwise.
 *
 * @param str: The input string containing the command to be parsed.
 * @param error_code: A pointer to an integer where the function can store error codes in case of failure.
 * @return Returns a pointer to the command_parts structure if the command was successfully parsed, and NULL otherwise.
 */
command_parts *read_command(char *str, int *error_code);


/**
 * @brief This function captures numbers from a string and stores them into the nums array of inst_parts structure.
 *
 * The function uses strtok to tokenize the input string, checking each token to see if it's a valid number.
 * If a token is a valid number, the function increases the size of the nums array in the inst_parts structure
 * and adds the number to this array. If the token isn't a valid number, an error code is set and the function returns 0.
 *
 * @param str: The input string from which numbers are to be extracted.
 * @param inst: A pointer to an inst_parts structure where the numbers are to be stored.
 * @param error_code: A pointer to an integer where the function can store error codes in case of failure.
 * @return Returns 1 if all tokens are valid numbers and have been successfully added to the nums array,
 *         and 0 if a token isn't a valid number or if memory reallocation for the nums array fails.
 */
int capture_nums(char *str, char *token_copy, inst_parts *inst, int *error_code);


/**
 * @brief This function reads an instruction from the input, parses it and stores it in an inst_parts structure.
 *
 * The function checks for a valid instruction starting with ".", and then tokenizes the input string.
 * It allocates an inst_parts structure, and based on the instruction type, it captures numbers or a string from the instruction,
 * or sets the necessary fields for ".entry" and ".extern" instructions. It also validates the instruction and sets an error code if necessary.
 *
 * @param str: The input string containing the instruction to be parsed.
 * @param error_code: A pointer to an integer where the function can store error codes in case of failure.
 * @return Returns a pointer to the inst_parts structure if the instruction was successfully parsed, and 0 otherwise.
 */
inst_parts *read_instruction(char *str, int *error_code);


/**
 * @brief This function increments the size of the array (*inst)->nums.
 *
 * This function is used to increment the size of the nums array in the inst_parts structure
 * by reallocating memory and handling any memory allocation errors.
 *
 * @param inst: A double pointer to an inst_parts structure, the nums array of which is to be incremented.
 * @param len: The new length for the nums array.
 * @return Returns 1 if the memory reallocation is successful, and 0 if it fails.
 *         In the case of failure, an internal error is printed, and the previously allocated memory is freed.
 */
int inc_array(inst_parts **inst, int len);


/**
 * @brief This function checks for errors related to opcode parsing.
 *
 * The function checks if there is a comma (',') in the input string, which indicates an error
 * since opcodes should not contain commas. If a comma is found, it returns ERROR_CODE_38.
 * If no comma is found, it returns ERROR_CODE_31, indicating a general opcode error.
 *
 * @param str: The input string containing the opcode to be checked.
 * @return Returns the corresponding error code based on the presence of a comma in the opcode string.
 */
int opcode_err_check(char *str);


/**
 * @brief This function counts the occurrence of a character in a given string.
 *
 * This function takes a string and a character as arguments and iterates through the string
 * to count the number of times the character occurs in the string.
 *
 * @param str A pointer to a null-terminated character string in which the character's occurrence is to be counted.
 * @param ch  The character whose occurrences need to be counted in the given string.
 *
 * @return Returns an integer representing the count of the specified character in the given string.
 */
int count_occurr(char *str, char ch);


/**
 * @brief Checks if a comma appears after a directive in a string.
 *
 * The function checks whether a comma appears after a directive in a given string. If a comma is found after the
 * directive, the appropriate error code is set, and the function returns 1. If a comma is not found but other
 * extraneous characters are present, a different error code is set, and the function returns 0.
 *
 * @param str: The input string to be checked for a comma after a directive.
 * @param error_code: A pointer to an integer to hold the error code if an error occurs.
 * @return Returns 1 if a comma appears after a directive, 0 if extraneous characters appear, and sets the appropriate error code.
 */
int is_comma_after_directive(char *str, int *error_code);


/**
 * @brief Checks if a given string represents a legal sequence of integers separated by commas.
 *
 * The function iterates through the characters of the input string 'str' and checks if it forms a legal sequence
 * of integers. The sequence can include negative integers and spaces between numbers. The function returns 1 if the
 * string is a legal sequence, and 0 otherwise.
 *
 * @param str: The input string to be checked.
 * @return Returns 1 if the string represents a legal sequence of integers, and 0 otherwise.
 */
int is_string_legal(const char *str);


/**
 * @brief Checks the validity of the first argument in a given string.
 *
 * This function extracts the first argument from the input string and checks
 * whether it is a valid register, label, or number using the `is_reg_or_label_or_num` function.
 *
 * @param str The input string containing the complete argument list.
 * @param ptr A pointer to the character in `str` where the first argument ends.
 * @return Returns the result of the `is_reg_or_label_or_num` function, which will be:
 *         - 1 if the first argument is a valid register.
 *         - 2 if the first argument is a valid label.
 *         - 3 if the first argument is a valid number.
 *         - 0 if the first argument is not a valid register, label, or number.
 */
int check_first_arg(char *str, char *ptr);


/**
 * @brief This function captures a string from the input and stores it as an array of shorts in the inst_parts structure.
 *
 * The function tokenizes the input string and checks the token for validity (must be enclosed in quotes).
 * The characters inside the quotes are stored as short values in the nums array of the inst_parts structure.
 * If there's text outside the quotes, an error code is set and the function returns 0.
 *
 * @param str: The input string from which a string is to be extracted.
 * @param inst: A pointer to an inst_parts structure where the string characters are to be stored as short values.
 * @param error_code: A pointer to an integer where the function can store error codes in case of failure.
 * @return Returns 1 if the string was successfully captured and stored in the nums array, and 0 otherwise.
 */
int capture_string(char *str, inst_parts *inst, int *error_code);

#endif
