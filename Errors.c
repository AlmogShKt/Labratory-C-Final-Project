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
        {ERROR_CODE_7,  "Failed to open new file for writing"},
        {ERROR_CODE_8,  "Failed to open file for reading"},
        {ERROR_CODE_9,  "Error - macro without name definition"},
        {ERROR_CODE_10, "Error - extra text after macro name definition"},
        {ERROR_CODE_11, "Setting a position in a file failed"},
        {ERROR_CODE_12, "Error - macro without 'endmcro' definition at the end"},
        {ERROR_CODE_13, "Macro has more than one definition"},
        {ERROR_CODE_14, "Failed to copy file during macros expansion"},
        {ERROR_CODE_15, "Macros expansion in an .as file failed"},
        {ERROR_CODE_16, "Macro call before declaration"},
        {ERROR_CODE_17, ""},
        {ERROR_CODE_18, ""},

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
