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
        {ERROR_CODE_2,  ".as files name is missing"},
        {ERROR_CODE_3,  "The file name is too long"},
        {ERROR_CODE_4,  "The provided file name is not exist"},
        {ERROR_CODE_5,  ".am file is missing"},
        {ERROR_CODE_6,  "The Line is too long"},
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
    printf("~~ERROR: ID:%d~~ | %s\n", error_code, errors[error_code].error_msg);
}


/**
 * Print errors while in the assembles files
 * @param error_code the error code
 * @param error_line the line in the assemble file the error was found
 */
void print_external_error(int error_code, line_data line) {
    /*todo: add file name */
    printf("~~ERROR: ID:%d~~ In %s at line:%d | there is error: %s\n", error_code, line.file_name, line.number,
           errors[error_code].error_msg);
}

/*todo: add another function to print erorr with the file name*/

