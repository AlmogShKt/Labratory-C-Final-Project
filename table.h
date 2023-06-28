#ifndef TABLE_H
#define TABLE_H

#include "lexer.h"
#include "globals.h"


typedef struct label_address {
    int address;
    char *label_name;
    int assembly_line;
    int is_data;
}label_address;

int insert_label_table(label_address **label_table, int lines, char *label, int IC, location am_file, int is_data);
int check_each_label_once(label_address *label_table, int lines, char *file_name);
void change_label_table_data_count(label_address *label_table, int table_lines, int IC);
void reset_labels_address(label_address *label_table, int table_lines);

#endif