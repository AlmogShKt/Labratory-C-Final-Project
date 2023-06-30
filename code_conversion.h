#ifndef C_PROJECT_CODE_CONVERSION_H
#define C_PROJECT_CODE_CONVERSION_H


#include "lexer.h"
#include "globals.h"

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
int add_machine_code_data(code_conv **code, inst_parts *inst, int *IC, location am_file);
int add_machine_code_line(code_conv **code, unsigned short num, char *str, int *IC, location am_file);
void print_binary_code(code_conv *code,int IC_len);
int merge_code(code_conv **code, code_conv *data, int IC, int DC);
int add_extra_machine_code_line(code_conv **code, command_parts *command, int *IC, int is_src, location am_file);
unsigned short reg_to_short(command_parts *command, int reg_src);
int add_extern_coding(code_conv **data,int *DC,location am_file);




#endif //C_PROJECT_CODE_CONVERSION_H
