#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Errors.h"
#include "globals.h"
#include "table.h"
#include "util.h"
#include "first_pass.h"
#include "lexer.h"
#include "code_conversion.h"
#include "second_pass.h"

/**
 * Executes the first pass of the assembly file parsing and processing.
 *
 * @param file_name The name of the assembly file to process.
 * @return An integer indicating whether an error occurred during the first pass.
 *         - If no error occurred, it returns 0.
 *         - If an error occurred, it returns a non-zero value.
 */
int exe_first_pass(char *file_name);

/**
 * Handles memory allocation for data structures used in the first pass.
 *
 * @param externs A pointer to the pointer of the externs table.
 * @param entries A pointer to the pointer of the entries table.
 * @param code A pointer to the pointer of the code_conv structure.
 * @param data A pointer to the pointer of the code_conv structure for data.
 * @return An integer indicating whether an error occurred during memory allocation.
 *         - If no error occurred, it returns 0.
 *         - If an error occurred, it returns a non-zero value.
 */
int handle_allocation(other_table **externs, other_table **entries, code_conv **code, code_conv **data);

#endif