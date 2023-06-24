#ifndef TABLE_H
#define TABLE_H

#include "lexer.h"


typedef struct label_address {
    int address;
    char *label_name;
}label_address;

int insert_label_table(label_address **label_table, int lines, char *label, int IC);

#endif