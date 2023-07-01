#include <stdio.h>
#include "second_pass.h"
#include "table.h"
#include "Errors.h"
#include "util.h"

void free_all_memory(code_conv *code, label_address *label_table, other_table *entries, other_table *externs, \
    int code_count, int label_table_line, int entries_count, int externs_count){
     free_code(code,code_count);
     free_label_table(label_table,label_table_line);
     free_other_table(entries,entries_count);
     free_other_table(entries,entries_count);
}

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
    int externs_count, int entries_count, code_conv *code, code_conv *data, other_table *externs, other_table *entries){
    int error_found;
    error_found = 0;
    if(IC > IC_MAX){
        print_internal_error(ERROR_CODE_54);
        error_found = 1;
    }
    if(check_each_label_once(label_table,label_table_line,file_name) == 0){
        error_found = 1;
    }
    if(is_extern_defined(externs,externs_count,label_table,label_table_line,file_name) == 1){
        error_found = 1;
    }
    change_label_table_data_count(label_table, label_table_line, IC);

    reset_labels_address(label_table, label_table_line);
    if(merge_code(&code, data, IC, DC) == 0){
        error_found = 1;
    }
    replace_externs(code,externs,externs_count,IC+DC,file_name);
    if(error_replace_labels(code,label_table,label_table_line,IC,file_name)){
        error_found = 1;
    }
    /* print_binary_code(code,IC+DC); */
    if(error_found == 0){
        conv_code_base64(code,IC+DC,file_name,IC,DC);
        print_externs(code,IC+DC,externs,externs_count,file_name);
        print_entries(label_table,label_table_line,entries,entries_count,file_name);
    }
    free_all_memory(code,label_table,entries,externs,IC+DC,label_table_line,entries_count,externs_count);
    return error_found;
}

