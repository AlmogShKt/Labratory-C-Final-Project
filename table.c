 /* Implements a basic table ("dictionary") data structure. sorted by value, ascending. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "util.h"

/*   */

/*  ddd */

void search_symbol(symbol_table current_table, char *symbol) {
    return;
}

void set_new_entry(char *symbol, int symbol_addrs, symbol_type sym_typ, table *new_entry) {
    /*Create new symbol to prevent aliasing*/
    char *new_symbol;
    new_symbol = (char *) handle_malloc(sizeof(symbol) + 1);
    strcpy(new_symbol, symbol);

    strcpy((char *) (*new_entry)->symbol, new_symbol);
    (*new_entry)->symbol_type = sym_typ;
    (*new_entry)->symbol_address = symbol_addrs;
}

void add_item_to_table(char *symbol, int symbol_addrs, symbol_type sym_typ, table *current_table) {
    table new_entry, current_entry, prev_entry;
    char *new_symbol = NULL;
    /*! use symbol */
    new_entry = (table) handle_malloc(sizeof(symbol_table));
    set_new_entry(new_symbol, symbol_addrs, sym_typ, &new_entry);

    /*Check if there is records in the table, if not create the head or if the address is need to be before what already exit*/
    if (current_table == NULL || (*current_table)->symbol_address < symbol_addrs) {
        /* If the table is NULL or the symbol address of the current table is less than new symbol address */
        /* Set the next_entry of new_entry to the current_table */
        new_entry->next_entry = (table *) (*current_table);
        /* Set the current_table to point to new_entry */
        (*current_table) = new_entry;
    }

    /* Move to the next entry */
    current_entry = (table) (*current_table)->next_entry;
    /* Keep track of the previous entry */
    prev_entry = (*current_table);
    while (current_table != NULL && current_entry->symbol_address > symbol_addrs) {
        /* While the current_table is not NULL and the symbol_address of the current_entry is greater than symbol_addrs */
        /* Update the prev_entry to the current_entry */
        prev_entry = current_entry;
        current_entry = (table) current_entry->next_entry;
    }

    /*Then set the new entry in the right place (sorted by the address value)*/
    new_entry->next_entry = (table *) current_entry;
    prev_entry->next_entry = (table *) new_entry;




    return;
}



