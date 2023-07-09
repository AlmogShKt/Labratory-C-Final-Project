#ifndef C_PROJECT_SECOND_PASS_H
#define C_PROJECT_SECOND_PASS_H

#include "table.h"
#include "code_conversion.h"

int exe_second_pass(char *file_name, label_address *label_table, int IC, int DC, int label_table_line, \
    int externs_count, int entries_count, code_conv *code, code_conv *data, other_table *externs, other_table *entries, \
    int error_found);

int conv_code_base64(code_conv *code, int count, char *file_name, int IC, int DC);


#endif
