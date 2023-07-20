#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "globals.h"
#include "Errors.h"

/* Define the opcodes */
op_code OPCODES[] = {
        {"mov",  2},
        {"cmp",  2},
        {"add",  2},
        {"sub",  2},
        {"not",  1},
        {"clr",  1},
        {"lea",  2},
        {"inc",  1},
        {"dec",  1},
        {"jmp",  1},
        {"bne",  1},
        {"red",  1},
        {"prn",  1},
        {"jsr",  1},
        {"rts",  0},
        {"stop", 0}
};
/* Define the registers */
char *REGS[] = {"@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7"};

/* Define the instructions */
char *INSTUCTIONS[] = {".data", ".string", ".extern", ".entry"};

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
int lines_too_long(char *file_name) {
    char str[SIZE];
    FILE *fp;
    location am_file;
    int too_long;
    fp = fopen(file_name, "r");
    am_file.file_name = file_name;
    too_long = 0;
    while (fgets(str, SIZE, fp) != NULL) {
        (am_file.line_num)++;
        if (strlen(str) > MAX_LINE_LENGTH) {
            /* line is too long */
            print_external_error(ERROR_CODE_30, am_file);
            too_long = 1;
        }
    }
    fclose(fp);
    return too_long;
}


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
int is_instr(char *str) {
    int i;
    if (str == NULL) {
        return 0;
    }
    for (i = 0; i < INSTRUCTIONS_COUNT; i++) {
        if (strcmp(str, INSTUCTIONS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}


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
int what_opcode(char *str) {
    int i;
    if (str == NULL) {
        return -1;
    }
    for (i = 0; i < OPCODES_COUNT; i++) {
        if (strcmp(str, OPCODES[i].opcode) == 0) {
            return i;
        }
    }
    return -1;
}


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
int what_reg(char *str) {
    int i;
    if (str == NULL) {
        return -1;
    }
    for (i = 0; i < REG_COUNT; i++) {
        if (strcmp(str, REGS[i]) == 0) {
            return i;
        }
    }
    return -1;
}


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
int legal_label_decl(char *str) {
    if (str == NULL) {
        return 0;
    }

    if (isalpha(*str) && strlen(str) <= MAX_LABEL_LENGTH && (what_reg(str) < 0) && (what_opcode(str) < 0)) {
        while (*(++str) != '\0' && (isalpha(*str) || isdigit(*str))) { ;
        }
        if (*(str) == ':' && *(str + 1) == '\0') {
            *str = '\0';
            return 1;
        }
    }
    return 0;
}


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
int legal_label(char *str) {
    if (str == NULL) {
        return 0;
    }
    if (isalpha(*str) && strlen(str) <= MAX_LABEL_LENGTH && (what_opcode(str) < 0) && \
        !is_instr(str)) {
        while (*(++str) != '\0' && *(str) != ' ' && (isalpha(*str) || isdigit(*str))) { ;
        }
    }
    if (*str == '\0' || *str == ' ') {
        return 1;
    } else {
    }
    return 0;
}

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
int extra_text() {
    char *token;
    token = strtok(NULL, "\n");
    if (token != NULL) {
        return 1;
    }
    return 0;
}

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
int is_reg_or_label(char *str) {
    if ((what_reg(str) >= 0) || legal_label(str)) {
        return 1;
    }
    return 0;
}


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
int is_num(char *str) {
    char *ptr;
    if (str != NULL) {
        strtol(str, &ptr, 10);
        if (*ptr == '\0' || *ptr == ' ') {
            return 1;
        }
    }
    return 0;
}


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
int is_reg_or_label_or_num(char *str) {
    return (is_reg_or_label(str) || is_num(str));
}

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
int count_occurr(char *str, char ch) {
    int count;
    char *ptr;
    ptr = str;
    count = 0;
    while ((*ptr != '\0') && (ptr = strchr(ptr, ch)) != NULL) {
        count++;
        ptr++;
    }
    return count;
}

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
int legal_arg(char *str, command_parts *command, int *error_code) {
    char *str1, *str2;
    /* empty argument */
    if (str == NULL) {
        *error_code = ERROR_CODE_34;
        return 0;
    }
    /* eliminating lines with two arguments and a missing comma or extra commas*/
    if (OPCODES[command->opcode].arg_num == 2) {
        if (strstr(str, ",") == NULL) {
            *error_code = ERROR_CODE_35;
            return 0;
        } else if (count_occurr(str, ',') > 1) {
            *error_code = ERROR_CODE_39;
            return 0;
        } else {
            str1 = strtok(str, ",");
            if (strchr(str1, ' ')) {
                *error_code = ERROR_CODE_40;
                return 0;
            }
            /*! added white space */
            str2 = strtok(NULL, " \n");
        }
    }

    /* Validate the argument based on the opcode */
    switch (command->opcode) {
        /* For each case, validate the source and destination arguments, if applicable. */
        /* If validation fails, set the error code and return 0. */
        /* If validation succeeds, assign the validated arguments to the command structure. */

        /* Handle each opcode case separately. */
        case 1: {
            if (is_reg_or_label_or_num(str1) && is_reg_or_label_or_num(str2)) {
                command->source = str1;
                command->dest = str2;
            } else {
                *error_code = ERROR_CODE_33;

                return 0;
            }
            break;
        }
        case 0:
        case 2:
        case 3: {
            if (is_reg_or_label_or_num(str1) && is_reg_or_label(str2)) {
                command->source = str1;
                command->dest = str2;
            } else {
                printf("here 2\n");
                *error_code = ERROR_CODE_33;
                return 0;
            }
            break;
        }
        case 6: {
            if (legal_label(str1) && is_reg_or_label(str2)) {
                command->source = str1;
                command->dest = str2;
            } else {

                *error_code = ERROR_CODE_33;
                return 0;
            }
            break;
        }
        case 4:
        case 5:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 13: {
            if (is_reg_or_label(str)) {
                command->source = NULL;
                command->dest = str;
            } else {
                *error_code = ERROR_CODE_33;
                return 0;
            }
            break;
        }
        case 12: {
            if (is_reg_or_label_or_num(str)) {
                command->source = NULL;
                command->dest = str;
            } else {
                *error_code = ERROR_CODE_33;
                return 0;
            }
            break;
        }
            /* case 14,15 are dealt with in previous stage */
        default: {
            *error_code = ERROR_CODE_33;
            return 0;
        }
    }
    return 1;
}


int check_invalid_char(char *str) {
    int i;
    const char invalidChars[] = {'$', '#', '@', '^', '%', '!', ','};
    int numInvalidChars = sizeof(invalidChars) / sizeof(char);
    for (i = 0; i < numInvalidChars; i++) {
        char temp[2];
        temp[0] = invalidChars[i];
        temp[1] = '\0';
        if (strstr(str, temp)) {
            return 1; // Character found in the array
        }

    }
    return 0;
}

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
int inc_array(inst_parts **inst, int len) {
    short *ptr;
    ptr = (*inst)->nums;
    (*inst)->nums = realloc((*inst)->nums, (len + 1) * sizeof(short));
    if ((*inst)->nums == NULL) {
        print_internal_error(ERROR_CODE_1);
        free(ptr);
        return 0;
    }
    return 1;
}


int is_string_legal(const char *str) {
    int digitFlag = 0, commaFlag = 0, i;
    for (i = 0; str[i]; i++) {
        if (isdigit(str[i]) || str[i] == '-' || str[i] == ' ') {
            if (commaFlag) commaFlag = 0;
            if (isdigit(str[i])) digitFlag = 1;
        } else if (str[i] == ',') {
            if (!digitFlag || commaFlag) {
                return 0; // Return false if comma found before any digit or after another comma
            }
            commaFlag = 1;
        } else {
            return 0; // Return false for any other character
        }
    }
    if (commaFlag) {
        return 0; // Return false if string ends with a comma
    }
    return 1; // Return true if string is legal
}

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
int capture_nums(char *str, char *token_copy, inst_parts *inst, int *error_code) {
    char *token;
    int len;
    len = 0;

//    *error_code = ERROR_CODE_39;
//    return  0;

    token = strtok(NULL, " \n");
    if (!is_string_legal(token)) {
        *error_code = ERROR_CODE_39;
        return 0;
    }


    strtok(token_copy, " \n");
    strtok(NULL, " \n");

    while ((token = strtok(NULL, ",\n")) != NULL) {
        if (is_num(token)) {
            if (inc_array(&inst, ++len) == 0) {
                return 0;
            }
            *(inst->nums + len - 1) = (short) (atoi(token));
        } else {
            if (strcmp(token, "\n") == 0) {
                /* comma AFTER the last number */
                *error_code = ERROR_CODE_51;
                return 0;
            }
            *error_code = ERROR_CODE_50;
            return 0;
        }
    }
    inst->len = len;
    return 1;
}


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
int capture_string(char *str, inst_parts *inst, int *error_code) {
    int flag;   /* if at least one char was saved */
    char *token;
    int len;
    len = inst->len = 0;
    if (*(token = strtok(NULL, "\n")) != '"') {
        /* there was no first '"' */
        *error_code = ERROR_CODE_52;
        return 0;
    }
    token++;
    if (strchr(token, '"') == NULL) {
        /* there was no second '"' */
        *error_code = ERROR_CODE_52;
        return 0;
    }
    flag = 0;
    while (*(token + len) != '"') {
        if (inc_array(&inst, ++len) == 0) {
            return 0;
        }
        *(inst->nums + len - 1) = (short) (*(token + len - 1));
        flag = 1;
    }
    if (*(token + len + 1) != '\0' && *(token + len + 1) != ' ') {
        /* extra text after the string end */
        *error_code = ERROR_CODE_53;
        if (flag == 1) {
            free(inst->nums);
        }
        return 0;
    }
    /* add \0 sign as the end of the string */
    if (inc_array(&inst, ++len) == 0) {
        return 0;
    }
    *(inst->nums + len - 1) = 0;
    inst->len = len;
    return 1;
}

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
inst_parts *read_instruction(char *str, int *error_code) {
    inst_parts *inst;
    char *token;
    char *token_copy = strdup(str);
    *error_code = 0;
    if (strstr(str, ".") == NULL) {
        return 0;
    }
    token = strtok(str, " \n");
    inst = handle_malloc(sizeof(inst_parts));
    inst->label = NULL;
    inst->nums = NULL;
    if (legal_label_decl(token)) {
        inst->label = token;
        token = strtok(NULL, " \n");
    } else if (strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0 || \
            strcmp(token, ".entry") == 0 || strcmp(token, ".extern") == 0) {
        inst->label = NULL;
    }
    if (strcmp(token, ".data") == 0) {
        if (capture_nums(str, token_copy, inst, error_code) == 0) {
            free(inst);
            return 0;
        }
    } else if (strcmp(token, ".string") == 0) {
        if (capture_string(str, inst, error_code) == 0) {
            free(inst);
            return 0;
        }
    } else if (strcmp(token, ".entry") == 0) {
        token = strtok(NULL, " \n");
        if (legal_label(token)) {
            inst->label = NULL; /* ignore label in the beginning */
            inst->len = -1; /* not a data line at all */
            inst->arg_label = token;
            inst->nums = 0;
            inst->is_extern = 0;
        } else {
            *error_code = ERROR_CODE_37;
            free(inst);
            return 0;
        }
    } else if (strcmp(token, ".extern") == 0) {
        token = strtok(NULL, " \n");
        if (legal_label(token)) {
            inst->label = NULL; /* ignore label in the beginning */
            inst->len = -1; /* not a data line at all */
            inst->arg_label = token;
            inst->nums = 0;
            inst->is_extern = 1;
        }
    }
    return inst;
}

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
int opcode_err_check(char *str) {
    char *c;
    if ((c = strchr(str, ',')) != NULL) {
        return ERROR_CODE_38;
    }
    return ERROR_CODE_31;
}

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
command_parts *read_command(char *str, int *error_code) {
    char *token;
    int legal_label_name_st = 1, legal_opcode_name_st = 1;
    command_parts *command = handle_malloc(sizeof(command_parts));
    if (command == NULL) {
        return NULL;
    }
    token = strtok(str, " \n");
    *error_code = 0;
    /* there is a legal label in the line */
    if (legal_label_decl(token)) {
        command->label = token;
        token = strtok(NULL, " \n");
        if ((command->opcode = what_opcode(token)) != -1) { ;
        } else {
            *error_code = opcode_err_check(token);
            return NULL;
        }
        if (OPCODES[command->opcode].arg_num == 0) {
            if (extra_text()) {
                *error_code = ERROR_CODE_32;
            } else {
                command->source = command->dest = NULL;
            }
        } else {
            if (legal_arg(strtok(NULL, "\n"), command, error_code) == 0) {
                return NULL;
            }
        }
    }
        /* command line with legal opcode without a label */
    else {
        /*The label name is invalid*/
        legal_label_name_st = 0;
        if (check_invalid_char(token)) {
            *error_code = ERROR_CODE_42;
            return NULL;
        }
    }

    if ((command->opcode = what_opcode(token)) != -1 && !legal_label_name_st) {
        command->label = NULL;
        legal_arg(strtok(NULL, "\n"), command, error_code);
    } else {
        /*Invalid opcode name*/
        if (!legal_label_name_st)
            legal_opcode_name_st = 0;
    }
    /* invalid label name */
    if ((legal_label_name_st == 1) || (legal_opcode_name_st == 1)) {
        return command;

    } else if (legal_opcode_name_st == 0) {
        if (check_invalid_char(token))
            *error_code = ERROR_CODE_41;
        else
            *error_code = ERROR_CODE_31;
        return NULL;
    }
    return NULL;
}

