#include <stdio.h>
#include "second_pass.h"
#include "first_pass.h"
#include "table.h"
#include "Errors.h"
#include "util.h"

int conv_code_base64(code_conv *code, int count, char *file_name,int IC, int DC){
    int i;
    FILE *fp;
    fp = fopen(add_new_file(file_name,".ob"),"w");
    if(fp == NULL){
        print_internal_error(ERROR_CODE_7);
        return 0;
    }
    fprintf(fp,"%d %d\n",IC+1,DC);
    for (i = 0; i <= count; i++){
        fprintf(fp,"%s\n", short_to_base64((code+i)->short_num));
    }
    return 1;
}


int error_exe_second_pass(char *file_name, label_address *label_table, int IC, int DC, int label_table_line,\
    int externs_count, code_conv *code, code_conv *data, extern_table *externs){
    int error_found;
    error_found = 0;
    if(IC > IC_MAX){
        print_internal_error(ERROR_CODE_54);
        error_found = 1;
    }
    if(check_each_label_once(label_table,label_table_line,file_name) == 0){
        error_found = 1;
    }
    if(check_extern_not_def(externs,externs_count,label_table,label_table_line,file_name) == 0){
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
    if(conv_code_base64(code,IC+DC,file_name,IC,DC) == 0){
        error_found = 1;
    }
    return error_found;
}

