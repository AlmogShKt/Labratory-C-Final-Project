#ifndef LABRATORY_C_FINAL_PROJECT_ERRORS_H
#define LABRATORY_C_FINAL_PROJECT_ERRORS_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Errors.h"
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
    /* enumeration skip for future macro related errors */
    ERROR_CODE_30 = 30,
    ERROR_CODE_31,
    ERROR_CODE_32,
    ERROR_CODE_33,
    ERROR_CODE_34,
    ERROR_CODE_35,
    ERROR_CODE_36,
    ERROR_CODE_37,
    ERROR_CODE_38,
    ERROR_CODE_39,
    ERROR_CODE_40,
    ERROR_CODE_41,
    ERROR_CODE_42,
    ERROR_CODE_43,
    ERROR_CODE_44,
    ERROR_CODE_45,
    ERROR_CODE_46,




} ERROR_CODES;

typedef enum EXIT_CODE {
    SUCCESS,
    FAILED
} EXIT_CODE;


void print_external_error(int error_code, line_data line);

void print_internal_error(int error_code);


#endif