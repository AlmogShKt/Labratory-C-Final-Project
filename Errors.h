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
    /* macro related errors or general errors */
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
    /* enumeration skip for future macro related errors */
    /* reading command related errors */
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
    /* enumeration skip for future command reading related errors */
    /* reading instruction .data or .string lines related errors */
    ERROR_CODE_50 = 50,
    ERROR_CODE_51,
    ERROR_CODE_52,
    ERROR_CODE_53,
    ERROR_CODE_54,
    ERROR_CODE_55,
    ERROR_CODE_56,
    ERROR_CODE_57,
    ERROR_CODE_58,
    ERROR_CODE_59,
    ERROR_CODE_60,
    ERROR_CODE_61,
    ERROR_CODE_62,
    ERROR_CODE_63,
    ERROR_CODE_64,
    ERROR_CODE_65,
    ERROR_CODE_66

} ERROR_CODES;

/**
 * This function prints to stdout an error that is related to an error in the source file
 * with error number and the error location in the source file
 * @param error_code error number
 * @param file a pointer to a struct which contains the source file name and line information
 */
void print_external_error(int error_code, location file);

/**
 * This function prints to stdout an error that is a result of an internal process and NOT an error in the source file
 * @param error_code error number
 */
void print_internal_error(int error_code);

#endif