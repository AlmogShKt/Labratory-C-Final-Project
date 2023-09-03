#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../Header Files /lexer.h"
#include "../Header Files /globals.h"
#include "../Header Files /Errors.h"

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


int lines_too_long(char *file_name) {
    char str[BIG_NUMBER_CONST];
    FILE *fp;
    location am_file;
    int too_long;

    /* Open the file for reading */
    fp = fopen(file_name, "r");
    am_file.file_name = file_name;
    too_long = 0;

    /* Read lines from the file and check their lengths */
    while (fgets(str, BIG_NUMBER_CONST, fp) != NULL) {
        if (strlen(str) > MAX_LINE_LENGTH) {
        (am_file.line_num)++;

            /* Line is too long */
            print_external_error(ERROR_CODE_30, am_file);
            too_long = 1; /* Set the flag to indicate a line is too long */
        }
    }
    fclose(fp);
    return too_long;
}


int is_instr(char *str) {
    int i;

    /* Return 0 if the string is NULL */
    if (str == NULL) {
        return 0;
    }

    /* Iterate through the list of known instructions and compare the string with each instruction in the list */
    for (i = 0; i < INSTRUCTIONS_COUNT; i++) {
        if (strcmp(str, INSTUCTIONS[i]) == 0) {
            return 1; /* Return 1 if the string matches an instruction */
        }
    }
    return 0; /* Return 0 if the string is not a valid instruction */
}


int what_opcode(char *str) {
    int i;

    /* Return -1 if the string is NULL */
    if (str == NULL) {
        return -1;
    }

    /* Iterate through the list of known opcodes and compare the string with each opcode in the list */
    for (i = 0; i < OPCODES_COUNT; i++) {
        if (strcmp(str, OPCODES[i].opcode) == 0) {
            return i; /* Return the index of the matching opcode */
        }
    }
    return -1; /* Return -1 if the string does not match any known opcodes */
}


int what_reg(char *str) {
    int i;

    /* Return -1 if the string is NULL */
    if (str == NULL) {
        return -1;
    }

    /* Iterate through the list of known registers and compare the string with each register in the list */
    for (i = 0; i < REG_COUNT; i++) {
        if (strcmp(str, REGS[i]) == 0) {
            return i; /* Return the index of the matching register */
        }
    }
    return -1; /* Return -1 if the string does not match any known registers */
}


int legal_label_decl(char *str, int *error_code) {
    if (str == NULL) {
        return 0;
    }

    /* If str is in one of the following - the label name is not valid */
    if (strlen(str) > MAX_LABEL_LENGTH || !isalpha(*str) || what_reg(str) >= 0) {
        *error_code = ERROR_CODE_44; /* Illegal label declaration */
        return 0;
    }

    if (what_opcode(str) < 0) {
        while (*(++str) != '\0' && (isalpha(*str) || isdigit(*str))) { ;
        }
        if (*(str) == ':' && *(str + 1) == '\0') {
            *str = '\0';
            return 1;
        } else {
            *error_code = ERROR_CODE_44; /* Illegal label declaration */
        }
    }

    return 0;
}


int legal_label(char *str) {
    if (str == NULL) {
        return 0;
    }

    /* If str is in one of the following - the label name is not valid */
    if (isalpha(*str) && strlen(str) <= MAX_LABEL_LENGTH && (what_opcode(str) < 0) && \
        !is_instr(str)) {
        while (*(++str) != '\0' && *(str) != ' ' && (isalpha(*str) || isdigit(*str))) { ;
        }
    }
    /* If we reached this point and the string either terminates or we have a space
     * then it means that all the characters were legal and the label is legal */
    if (*str == '\0' || *str == ' ') {
        return 1;
    } else {
        return 0;
    }
}

int extra_text() {
    char *token;
    token = strtok(NULL, "\n");

    /* If a token exists beyond the expected content, there is extra text */
    if (token != NULL) {
        return 1;
    }
    return 0; /* No extra text found */
}

int is_reg_or_label(char *str) {
    /* Check if the string is a valid register using what_reg()
     * OR if it's a legal label using legal_label() */
    if ((what_reg(str) >= 0) || legal_label(str)) {
        return 1;
    }
    return 0; /* Neither a valid register nor a legal label */
}

int check_first_arg(char *str, char *ptr) {
    /* Calculate the length of the first argument substring */
    int first_arg_len = (int) (ptr - str);

    /* Extract the first argument substring */
    char first_arg[MAX_LINE_LENGTH];
    strncpy(first_arg, str, first_arg_len);
    first_arg[first_arg_len] = '\0';

    /* Check if the extracted first argument is a valid register, label, or number */
    return is_reg_or_label_or_num(first_arg);
}

int is_num(char *str) {
    char *ptr;

    /* Check if the string is not NULL */
    if (str != NULL) {
        strtol(str, &ptr, 10);

        /* Check if the conversion result is followed by a space or the entire string is converted */
        if (*ptr == '\0' || *ptr == ' ') {
            return 1; /* The string represents a numeric value */
        }
    }
    return 0; /* The string does not represent a numeric value */
}

int is_reg_or_label_or_num(char *str) {
    /* Check if the string is a valid register or label, and if it's a valid number */
    return (is_reg_or_label(str) || is_num(str));
}

int count_occurr(char *str, char ch) {
    int count;
    char *ptr;
    ptr = str;
    count = 0;

    /* Traverse the string until the end or until no more occurrences are found */
    while ((*ptr != '\0') && (ptr = strchr(ptr, ch)) != NULL) {
        count++;
        ptr++;
    }
    return count; /* Return the total count of occurrences */
}

int has_white_space(const char *str) {
    while (*str) {
        if (isspace(*str)) {
            return 1; /* Found a whitespace character */
        }
        str++;
    }
    return 0; /* No whitespace characters found */
}

void check_reg_error(int *error_code, char *str) {
    char temp_str1[MAX_LINE_LENGTH];
    strcpy(temp_str1, str);

    /* Check if any previous error has already occurred */
    if (error_code[0])
        return;

    /* Check if any previous error has already occurred */
    if (has_white_space(str))
        *error_code = ERROR_CODE_33;
    else if (atoi(strtok(temp_str1, "@r"))) {
        *error_code = ERROR_CODE_46; /* Illegal register name */
    } else
        *error_code = ERROR_CODE_33; /* Illegal argument */
}


int legal_arg(char *str, command_parts *command, int *error_code) {
    char *str1, *str2, *ptr;

    /* Check if the argument is empty */
    if (str == NULL && OPCODES[command->opcode].arg_num != 0) {
        *error_code = ERROR_CODE_34;
        return 0;
    }

    /* Handle opcodes with no arguments */
    if (OPCODES[command->opcode].arg_num == 0) {
        if (extra_text()) {
            *error_code = ERROR_CODE_32;
            return 0;
        } else {
            command->source = command->dest = NULL;
            return 1;
        }
    }

    /* Handle cases where the command has two arguments */
    if (OPCODES[command->opcode].arg_num == 2) {
        if (strstr(str, ",") == NULL) {
            *error_code = ERROR_CODE_35; /* Missing comma between arguments */
            return 0;
        } else if (count_occurr(str, ',') > 1) {
            *error_code = ERROR_CODE_39; /* Extra commas between arguments */
            return 0;
        } else {
            str1 = strtok(str, ",");
            if ((ptr = strchr(str1, ' '))) {
                if (check_first_arg(str1, ptr)) {
                    *error_code = ERROR_CODE_35; /* Invalid first argument format */
                    return 0;
                }
                *error_code = ERROR_CODE_33; /* Other argument-related error */
                return 0;
            }
            str2 = strtok(NULL, " \n");
            if (extra_text()) {
                *error_code = ERROR_CODE_32; /* Extra text after arguments */
                return 0;
            }
        }
    }
    /* Handle cases where the command has one argument */
    else if (OPCODES[command->opcode].arg_num == 1) {
        if (strchr(str, ' ')) {
            *error_code = ERROR_CODE_32; /* Extra text after argument */
            return 0;
        }
    }

    /* Validate and process the argument based on the opcode */
    switch (command->opcode) {
        /* Handle each opcode case separately */
        /* For each case, validate the source and destination arguments, if applicable. */
        /* If validation fails, set the error code and return 0. */
        /* If validation succeeds, assign the validated arguments to the command structure. */

        /* source addressing code is 1,3,5 and dest addressing code is 1,3,5 */
        case 1: {
            if (is_reg_or_label_or_num(str1) && is_reg_or_label_or_num(str2)) {
                command->source = str1;
                command->dest = str2;
            } else {
                if (str2 == NULL)
                    *error_code = ERROR_CODE_34; /* Missing argument */
                else if (what_reg(str1) == -1 || what_reg(str2) == -1)
                    *error_code = ERROR_CODE_46; /* Illegal register name */
                else
                    *error_code = ERROR_CODE_33; /* Illegal argument */
                return 0;
            }
            break;
        }

        /* source addressing code is 1,3,5 and dest addressing code is 3,5 */
        case 0:
        case 2:
        case 3: {
            if (is_reg_or_label_or_num(str1) && is_reg_or_label(str2)) {
                command->source = str1;
                command->dest = str2;
            } else {
                check_reg_error(error_code, str1);
                check_reg_error(error_code, str2);
                return 0;
            }
            break;
        }

        /* source addressing code is 3 and dest addressing code is 3,5 */
        case 6: {
            if (legal_label(str1) && is_reg_or_label(str2)) {
                command->source = str1;
                command->dest = str2;
            } else {
                *error_code = ERROR_CODE_33; /* Illegal argument */
                return 0;
            }
            break;
        }

        /* dest addressing code is 3,5 and no source */
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
                if (atoi(strtok(str, "@r")))
                    *error_code = ERROR_CODE_46; /* Illegal register name */
                else
                    *error_code = ERROR_CODE_33; /* Illegal argument */
                return 0;
            }
            break;
        }

        /* dest addressing code is 1,3,5 and no source */
        case 12: {
            if (is_reg_or_label_or_num(str)) {
                command->source = NULL;
                command->dest = str;
            } else {
                check_reg_error(error_code, str);
                return 0;
            }
            break;
        }
        /* case 14,15 are dealt with in previous stage */
        default: {
            *error_code = ERROR_CODE_33; /* Illegal argument */
            return 0;
        }
    }
    return 1;
}

int is_comma_after_directive(char *str, int *error_code) {
    if (strchr(str, ',')) {
        *error_code = ERROR_CODE_40; /* Comma after directive */
        return 1;
    }
    *error_code = ERROR_CODE_58; /* Illegal data line directive */
    return 0;
}


int inc_array(inst_parts **inst, int len) {
    short *ptr;
    ptr = (*inst)->nums; /* Backup for the case realloc fails */
    (*inst)->nums = realloc((*inst)->nums, (len + 1) * sizeof(short));
    if ((*inst)->nums == NULL) {
        print_internal_error(ERROR_CODE_1); /* Internal error: Memory allocation failed */
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
                return 0; /* Return false if comma found before any digit or after another comma*/
            }
            commaFlag = 1;
        } else {
            return 0; /* Return false for any other character */
        }
    }
    if (commaFlag) {
        return 0; /* Return false if string ends with a comma */
    }
    return 1; /*Return true if string is legal */
}


int capture_nums(char *str, char *token_copy, inst_parts *inst, int *error_code) {
    char *token;
    int len, number;
    len = 0;

    /* Ensure a space follows any colon in the token_copy */
    if (!add_space_after_colon(&token_copy, error_code)) {
        return 0;
    }

    /* Extract token for numeric value validation */
    token = strtok(NULL, " \n");
    if (!is_string_legal(token)) {
        *error_code = ERROR_CODE_59; /* Instruction '.data' line contains illegal chars or syntax error */
        return 0;
    }
    strtok(token_copy, " \n");
    strtok(NULL, " \n");

    /* Process each token separated by commas */
    while ((token = strtok(NULL, ", \n")) != NULL) {
        if (is_num(token)) {
            number = (short) (atoi(token));
            if (number > MAX_NUM || number < MIN_NUM) {
                *error_code = ERROR_CODE_57; /* Input number in .data line is out of range */
                return 0;
            } else if (inc_array(&inst, ++len) == 0) {
                return 0;
            }
            *(inst->nums + len - 1) = (short) (atoi(token));
        } else {
            if (strcmp(token, "\n") == 0) {
                /* comma AFTER the last number */
                *error_code = ERROR_CODE_51; /* Comma after the last number in a '.data' line */
                return 0;
            }
            *error_code = ERROR_CODE_50; /* Instruction '.data' line contains non-number info */
            return 0;
        }
    }
    inst->len = len;
    return 1;
}

int capture_string(char *str, inst_parts *inst, int *error_code) {
    int flag;   /* If at least one char was saved */
    char *token;
    int len;
    len = inst->len = 0;

    /* Check for the opening double-quote */
    if (*(token = strtok(NULL, "\n")) != '"') {
        *error_code = ERROR_CODE_52; /* There was no opening double-quote */
        return 0;
    }
    token++;

    /* Check for the closing double-quote */
    if (strchr(token, '"') == NULL) {
        *error_code = ERROR_CODE_52; /* There was no closing double-quote */
        return 0;
    }
    flag = 0;
    while (*(token + len) != '"') {
        /* Save each character as a short integer in the instruction's nums array */
        if (inc_array(&inst, ++len) == 0) {
            return 0;
        }
        *(inst->nums + len - 1) = (short) (*(token + len - 1));
        flag = 1;
    }

    /* Check for extra text after the end of the string */
    if (!(*(token + len + 1) == '\0' || *(token + len + 1) == '\n')) {
        /* Extra text after the string end */
        *error_code = ERROR_CODE_53;
        if (flag == 1) {
            free(inst->nums);
        }
        return 0;
    }
    /* Add '\0' as the end of the string */
    if (inc_array(&inst, ++len) == 0) {
        return 0;
    }
    *(inst->nums + len - 1) = 0;
    inst->len = len;
    return 1;
}

inst_parts *read_entry_or_extern(char *str, int *error_code) {
    inst_parts *inst;
    char *ptr, *token;

    /* Find the first occurrence of '.' in the string */
    ptr = strchr(str, '.');
    token = strtok(ptr, " ");

    /* Allocate memory for the inst_parts structure */
    inst = handle_malloc(sizeof(inst_parts));
    if (inst == NULL) {
        *error_code = ERROR_CODE_1;
        return NULL;
    }

    /* Initialize fields of the inst_parts structure */
    inst->label = NULL;
    inst->nums = NULL;
    inst->is_extern = 0;
    inst->len = 0;

    /* Check if the token is ".extern" and set the is_extern flag */
    if (strcmp(token, ".extern") == 0) {
        inst->is_extern = 1;
    }

    /* Tokenize and handle the argument label */
    token = strtok(NULL, " \n");
    if (legal_label(token)) {
        inst->arg_label = token;
    } else {
        *error_code = ERROR_CODE_44;
    }

    /* Check for extra text after parsing */
    if (extra_text()) {
        *error_code = ERROR_CODE_32;
    }
    return inst; /* Return the inst_parts structure */
}

inst_parts *read_instruction(char *str, int *error_code) {
    inst_parts *inst;
    char *token;
    char token_copy[MAX_LINE_LENGTH];
    strcpy(token_copy, str);

    /* Check for presence of a dot in the string */
    if (strstr(str, ".") == NULL) {
        return 0;
    }

    /* Add a space after colon and handle error if needed */
    if (!add_space_after_colon(&str, error_code)) {
        return NULL;
    }

    /* Tokenize the string */
    token = strtok(str, " \n");

    /* Allocate memory for the inst_parts structure */
    inst = handle_malloc(sizeof(inst_parts));
    if (inst == NULL) {
        *error_code = ERROR_CODE_1;
        return NULL;
    }

    /* Initialize fields of the inst_parts structure */
    inst->label = NULL;
    inst->nums = NULL;

    /* Handle different cases based on the token */
    if (legal_label_decl(token, error_code)) {
        inst->label = token;
        token = strtok(NULL, " \n");
    } else if (strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0 || \
            strcmp(token, ".entry") == 0 || strcmp(token, ".extern") == 0) {
        inst->label = NULL;
    }

    /* Check directive cases */
    if (strcmp(token, ".data") == 0) {
        capture_nums(str, token_copy, inst, error_code);
    } else if (strcmp(token, ".string") == 0) {
        capture_string(str, inst, error_code);
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
    } else {
        is_comma_after_directive(token, error_code);
    }

    return inst; /* Return the inst_parts structure */
}

int opcode_err_check(char *str) {
    char *c;

    /* Check if a comma is present in the string */
    if ((c = strchr(str, ',')) != NULL) {
        return ERROR_CODE_38; /* Comma-related opcode error */
    }
    return ERROR_CODE_31; /* Illegal opcode */
}

int add_space_after_colon(char **str, int *error_code) {
    char *colon_ptr = NULL;

    /* Find the position of the first colon in the string */
    colon_ptr = strchr(*str, ':');

    /* If no colon is found, return without making changes */
    if (!colon_ptr) {
        return 1;
    }

    else {
        /* If a colon exists, add a space after the colon
         * Check if the string is already at its maximum length */
        if (strlen(*str) == MAX_LINE_LENGTH) {
            char *temp_ptr = *str;

            /* Reallocate memory to accommodate one more character */
            *str = realloc(*str, MAX_LINE_LENGTH + 1);
            if (*str == NULL) {
                *error_code = ERROR_CODE_1;
                free(temp_ptr);
                return 0;
            }
        }

        /* Move the content after the colon by one position to add a space */
        colon_ptr = strchr(*str, ':');
        colon_ptr++; /* Move to the character after the colon */
        memmove(colon_ptr + 1, colon_ptr, strlen(colon_ptr) + 1);
        *colon_ptr = ' '; /* Add a space after the colon */

        return 1; /* Successful modification or reallocation */
    }
}

command_parts *read_command(char *str, int *error_code) {
    char *token;
    int flag_visited = 0;

    /* Allocate memory for the command_parts structure */
    command_parts *command = handle_malloc(sizeof(command_parts));
    if (command == NULL) {
        return command;
    }

    /* Add a space after colon and handle error if needed */
    if (!add_space_after_colon(&str, error_code))
        return command;

    /* Tokenize the string */
    token = strtok(str, " \n");

    /* Check for a legal label declaration */
    if (legal_label_decl(token, error_code)) {
        flag_visited = 1;
        command->label = token;
      
        /* Get the opcode and check if it's valid */
        token = strtok(NULL, " \n");
        if ((command->opcode = what_opcode(token)) != -1) { ;
        } else {
            *error_code = opcode_err_check(token);
            command->opcode = -1;
            return command;
        }

        /* Handle arguments based on the opcode's argument count */
        if (OPCODES[command->opcode].arg_num == 0) {
            if (extra_text()) {
                *error_code = ERROR_CODE_32;
            } else {
                command->source = command->dest = NULL;
            }
        } else {
            if (legal_arg(strtok(NULL, "\n"), command, error_code) == 0) {
                return command;
            }
        }
    }

    /* Command line with a legal opcode without a label */
    else {
        if (*error_code)
            return command;
    }

    /* Handle the case where no legal_label_decl was visited */
    if (!flag_visited) {
        if ((command->opcode = what_opcode(token)) != -1) {
            command->label = NULL;
            legal_arg(strtok(NULL, "\n"), command, error_code);
            return command;
        } else {
            *error_code = opcode_err_check(token);
            command->opcode = -1;
            return command;
        }
    } else {
        return command;
    }
}

