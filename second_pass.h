#ifndef C_PROJECT_SECOND_PASS_H
#define C_PROJECT_SECOND_PASS_H

#include "table.h"
#include "code_conversion.h"

int exe_second_pass(char *file_name, label_address *label_table, int IC, int DC, int label_table_line,\
    code_conv *code, code_conv *data);


#endif //C_PROJECT_SECOND_PASS_H
