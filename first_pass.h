#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "lexer.h"
#include "globals.h"
#include "table.h"

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

int inc_mem(code_conv **code, int IC);
unsigned short command_to_short(command_parts *command);
int exe_first_pass(char *file_name);
int add_machine_code_data(code_conv **code, inst_parts *inst, int *IC, location am_file);
int add_machine_code_line(code_conv **code, unsigned short num, char *str, int *IC, location am_file);
void print_binary_code(code_conv *code,int IC_len);
int error_replace_labels(code_conv *code, label_address *label_table, int label_table_line, int IC_len, char *file_name);
#endif