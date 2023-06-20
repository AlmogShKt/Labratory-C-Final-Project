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

int lines_too_long(char *file_name);
int is_instr(char *str);
int what_opcode(char *str);
int what_reg(char *str);
int legal_label_decl(char *str);
int legal_label(char *str);
int extra_text();
int is_reg_or_label(char *str);
int is_reg_or_label_or_num(char *str);
void remove_asp(char **str);
int legal_arg(char *str, command_parts *command, int *error_code);
command_parts *read_command(char *str, int *error_code);



#endif //C_PROJECT_LEXER_H
