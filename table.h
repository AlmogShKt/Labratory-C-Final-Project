#ifndef LABRATORY_C_FINAL_PROJECT_TABLE_H
#define LABRATORY_C_FINAL_PROJECT_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"

/*__________*/
/*Symbols table*/
#define MAX_SYMBOL_LENGTH 31

/** pointer to table entry is just a table. */
typedef struct symbol_table_entry *table;

/*Define the symbol table entries*/
typedef struct symbol_table_entry {
    /*The Symbol - char - up to 30*/
    char *symbol[MAX_SYMBOL_LENGTH];
    int symbol_address;
    symbol_type symbol_type;
    table *next_entry;
} symbol_table;


#endif
