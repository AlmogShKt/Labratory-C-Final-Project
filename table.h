#ifndef TABLE_H
#define TABLE_H

#include "lexer.h"
#include "globals.h"


typedef struct label_address {
    int address;
    char *label_name;
    int assembly_line;
}label_address;

int insert_label_table(label_address **label_table, int lines, char *label, int IC, location am_file);
int check_each_label_once(label_address *label_table, int lines, char *file_name);

#endif