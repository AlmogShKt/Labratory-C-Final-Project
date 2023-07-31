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
        {ERROR_CODE_12, "endmcro with extra text"},
        {ERROR_CODE_13, "Macro has more than one definition"},
        {ERROR_CODE_14, "Failed to copy file during macros expansion"},
        {ERROR_CODE_15, "Macros expansion in an .as file failed"},
        {ERROR_CODE_16, "Macro call before declaration"},
        {ERROR_CODE_17, "Illegal name for a macro"},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {-1,            ""},
        {ERROR_CODE_30, "Line is too long"},
        {ERROR_CODE_31, "Illegal opcode"},
        {ERROR_CODE_32, "Line contains extra text"},
        {ERROR_CODE_33, "Illegal argument"},
        {ERROR_CODE_34, "Missing argument"},
        {ERROR_CODE_35, "Missing comma between argument in a command line with two arguments"},
        {ERROR_CODE_36, "Label not defined in the assembly file"},
        {ERROR_CODE_37, "Illegal label after .entry"},
        {ERROR_CODE_38, "Illegal comma near opcode"},
        {ERROR_CODE_39, "More commas than needed"},
        {ERROR_CODE_40, "Comma in the wrong place"},
        {ERROR_CODE_41, "Illegal char near opcode or label"},
        {ERROR_CODE_42, "Illegal char"},
        {ERROR_CODE_43, "Missing comma between numbers"},
        {ERROR_CODE_44, "Illegal label declaration"},
        {ERROR_CODE_45, "Missing ':' after label declaration"},
        {ERROR_CODE_46, "Illegal register name. Use only @r1-@r7"},
        {ERROR_CODE_47, "Illegal comma before opcode"},
        {ERROR_CODE_48, ""},
        {ERROR_CODE_49, "Data line without '. before directive"},
        {ERROR_CODE_50, "Instruction '.data' line contains non-number info"},
        {ERROR_CODE_51, "Comma after the last number in a '.data' line"},
        {ERROR_CODE_52, "Missing '\"' after '.string'"},
        {ERROR_CODE_53, "Extra text after the string end in '.string' line"},
        {ERROR_CODE_54, "IC too big for word CPU word length"},
        {ERROR_CODE_55, "Label definition repeats more than once"},
        {ERROR_CODE_56, "Label defined as .extern and defined in file"},
        {ERROR_CODE_57, "Input number in .data line is out of range"},
        {ERROR_CODE_58, "Illegal data line directive"},
        {ERROR_CODE_59, "Instruction '.data' line contains illegal chars or syntax error"},
        {ERROR_CODE_60, ""},

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
 * @param error_line the line in the assembly file the error was found
 */
void print_external_error(int error_code, location file) {
    printf("~~ERROR: ID:%d~~ in %s at line:%d | there is error: %s\n", error_code, \
    file.file_name, file.line_num, errors[error_code].error_msg);
}

/*todo: add another function to print erorr with the file name*/
