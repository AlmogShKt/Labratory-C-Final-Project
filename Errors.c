#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "table.h"
#include "util.h"
#include "Errors.h"

Error errors[] = {
        {ERROR_CODE_0,  "No Error"},
        {ERROR_CODE_1,  "Failed to dynamically allocate memory"},
        {ERROR_CODE_2,  ""},
        {ERROR_CODE_3,  ""},
        {ERROR_CODE_4,  ""},
        {ERROR_CODE_5,  ""},
        {ERROR_CODE_6,  ""},
        {ERROR_CODE_7,  ""},
        {ERROR_CODE_8,  ""},
        {ERROR_CODE_9,  ""},
        {ERROR_CODE_10, ""},
        {ERROR_CODE_11, ""}
};

/**
 * Prints error while the program
 * @param error_code the error code
 */
void print_internal_error(int error_code) {
    printf("~~ERROR~~:ID: %d | %s", error_code, errors[error_code].error_msg);
}


/**
 * Print errors while in the assembles files
 * @param error_code the error code
 * @param error_line the line in the assemble file the error was found
 */
void print_external_error(int error_code, int error_line) {
    printf("~~ERROR~~:ID: %d | in line %d there is error: %s", error_code, error_line, errors[error_code].error_msg);
}


