#ifndef LABRATORY_C_FINAL_PROJECT_ERRORS_H
#define LABRATORY_C_FINAL_PROJECT_ERRORS_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "table.h"
#include "util.h"

typedef struct Error {
    int error_id;
    char *error_msg;
} Error;

typedef enum ERROR_CODES {
    ERROR_CODE_0 = 0,
    ERROR_CODE_1,
    ERROR_CODE_2,
    ERROR_CODE_3,
    ERROR_CODE_4,
    ERROR_CODE_5,
    ERROR_CODE_6,
    ERROR_CODE_7,
    ERROR_CODE_8,
    ERROR_CODE_9,
    ERROR_CODE_10,
    ERROR_CODE_11,
    ERROR_CODE_12,
    ERROR_CODE_13,
    ERROR_CODE_14,
    ERROR_CODE_15,
    ERROR_CODE_16,
    ERROR_CODE_17,
    ERROR_CODE_18,
    ERROR_CODE_19,
    ERROR_CODE_20,
    ERROR_CODE_21,
    ERROR_CODE_22,
    ERROR_CODE_23,
    ERROR_CODE_24,
    ERROR_CODE_25,
    ERROR_CODE_26,
    ERROR_CODE_27,
    ERROR_CODE_28,
    ERROR_CODE_29

} ERROR_CODES;

typedef enum EXIT_CODE {
    SUCCESS,
    FAILED
} EXIT_CODE;


void print_external_error(int error_code, line_data line);

void print_internal_error(int error_code);


#endif