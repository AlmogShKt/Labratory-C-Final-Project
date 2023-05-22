#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Errors.h"
#include "data_strct.h"
#include "globals.h"
#include "table.h"
#include "util.h"


int start_first_pass();

int start_second_pass();

int handle_file(char file_name[]);


/**
 *
 * The main is initiating the whole process. for each file name will be 3 runs.
 * 1: macro
 * 2: first pass
 * 3: second pass
 * */
int main(int argc, char *argv[]) {
    int file_index;
    /*? What is the max file name?*/
    char file_name[MAX_LABEL_LENGTH];
    FILE *file_ptr;

    /*In case that .as file name was not provided in the arguments*/
    if (argc <= 1) {
        print_internal_error(ERROR_CODE_2);
        exit(FAILED);
    }

    /*For each file was provided*/
    for (file_index = 1; file_index < argc; file_index++) {
        /*Copy the file name to local variable*/
        strcpy(argv[file_index], (const char *) file_name);

        /*Check if the file name is too long*/
        if (strlen(file_name) > MAX_LABEL_LENGTH) {
            print_internal_error(ERROR_CODE_3);
            exit(FAILED);
        }

        /*Check if the file is exist */
        if (isFileExist(file_name)) {
            strcpy(argv[file_index], (const char *) file_name);
            file_ptr = fopen(file_name, "r");
            handle_file(file_name);
        } else {
            /*The file is not exist*/
            print_internal_error(ERROR_CODE_4);
        }
    }

    return SUCCESS;
}

int start_first_pass() {


    return 0;
}


