#ifndef LABRATORY_C_FINAL_PROJECT_ERRORS_H
#define LABRATORY_C_FINAL_PROJECT_ERRORS_H



typedef struct Error{
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
    ERROR_CODE_11
} ERROR_CODES;

typedef enum EXIT_CODE {
    SUCCESS,
    FAILED
} EXIT_CODE;


void print_external_error(int error_code, int error_line);

void print_internal_error(int error_code);



#endif
