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
} label_address;

typedef struct other_table {
    char *label_name;
    int assembly_line;
} other_table;

int insert_label_table(label_address **label_table, int lines, char *label, int counter, location am_file, int is_data);

int check_each_label_once(label_address *label_table, int lines, char *file_name);

void change_label_table_data_count(label_address *label_table, int table_lines, int IC);

void reset_labels_address(label_address *label_table, int table_lines);

int replace_labels(code_conv *code, label_address *label_table, int label_table_line, int IC_len, char *file_name);

int insert_other_labels(other_table **table, int count, inst_parts *inst, location am_file);

int is_extern_defined(other_table *externs, int externs_count, label_address *label_table, int label_table_line,
                      char *file_name);

void replace_externs(code_conv *code, other_table *externs, int externs_count, int count, char *file_name);

int print_externs(code_conv *code, int count, other_table *externs, int externs_count, char *file_name);

int print_entries(label_address *label_table, int label_table_line, other_table *entries, int entries_count,
                  char *file_name);

void free_other_table(other_table *table, int count);

void free_label_table(label_address *label_table, int label_table_line);

#endif