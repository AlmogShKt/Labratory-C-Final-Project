#include "globals.h"

int lines_too_long(char *file_name);
int is_instr(char *str);
int is_opcode(char *str);
int is_reg(char *str);
int legal_label(char *str);
command_parts *read_command(char *str, int *error_code);
int arg_count(char *opcode);
int extra_text();