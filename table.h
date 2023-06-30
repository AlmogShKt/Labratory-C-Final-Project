#ifndef TABLE_H
#define TABLE_H

#include "lexer.h"
#include "globals.h"
#include "code_conversion.h"


typedef struct label_address {
    int address;
    char *label_name;
    int assembly_line;
    int is_data;
}label_address;

typedef struct extern_table {
    char *label_name;
    int assembly_line;
} extern_table;

int insert_label_table(label_address **label_table, int lines, char *label, int counter, location am_file, int is_data);
int check_each_label_once(label_address *label_table, int lines, char *file_name);
void change_label_table_data_count(label_address *label_table, int table_lines, int IC);
void reset_labels_address(label_address *label_table, int table_lines);
int error_replace_labels(code_conv *code, label_address *label_table, int label_table_line, int IC_len, char *file_name);
int insert_externs(extern_table **externs, int externs_count, inst_parts *inst, location am_file);
int check_extern_not_def(extern_table *externs,int externs_count,label_address *label_table, int label_table_lines,\
    char *file_name);

#endif