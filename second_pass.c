#include <stdio.h>
#include "second_pass.h"
#include "first_pass.h"
#include "table.h"
#include "Errors.h"
#include "util.h"

int conv_code_base64(code_conv *code, int count, char *file_name){
    FILE *fp;
    fp = fopen(strcat(file_name,".ob"),"w");
    if(fp == NULL){
        print_internal_error(ERROR_CODE_7);
        return 0;
    }

}


int exe_second_pass(char *file_name, label_address *label_table, int IC, int DC, int label_table_line,\
    code_conv *code, code_conv *data){
    int error_found;
    error_found = 0;
    if(IC > IC_MAX){
        print_internal_error(ERROR_CODE_54);
        error_found = 1;
    }
    if(check_each_label_once(label_table,label_table_line,file_name) == 0){
        error_found = 1;
    }
    change_label_table_data_count(label_table, label_table_line, IC);

    reset_labels_address(label_table, label_table_line);
    if(error_replace_labels(code,label_table,label_table_line,IC,file_name)){
        error_found = 1;
    }
    if(merge_code(&code, data, IC, DC) == 0){
        error_found = 1;
    }
    print_binary_code(code,IC+DC);


}

