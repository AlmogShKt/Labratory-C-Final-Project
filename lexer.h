#ifndef C_PROJECT_LEXER_H
#define C_PROJECT_LEXER_H

typedef struct op_code{
    char *opcode;
    int arg_num;
}op_code;

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
} inst_parts;

int lines_too_long(char *file_name);
int is_instr(char *str);
int what_opcode(char *str);
int what_reg(char *str);
int legal_label_decl(char *str);
int legal_label(char *str);
int extra_text();
int is_reg_or_label(char *str);
int is_num(char *str);
int is_reg_or_label_or_num(char *str);
void remove_asp(char **str);
int legal_arg(char *str, command_parts *command, int *error_code);
command_parts *read_command(char *str, int *error_code);
int capture_nums(char *str, inst_parts *inst, int *error_code);
inst_parts *read_instruction(char *str, int *error_code);
int inc_array(inst_parts **inst, int len);



#endif //C_PROJECT_LEXER_H
