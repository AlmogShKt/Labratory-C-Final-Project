#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "lexer.h"

#define SOURCE_BITS_SHIFT 9
#define OPCODE_BITS_SHIFT 5
#define DEST_BITS_SHIFT 2

#define DIRECT_ADDRESSING 1
#define LABEL_ADDRESSING 3
#define REG_ADDRESSING 5

unsigned short command_to_short(command_parts *command);
int exe_first_pass(char *file_name);
#endif