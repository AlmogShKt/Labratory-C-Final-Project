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
    char file_name[MAX_LABEL_LENGTH], *full_file_name;


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

        /*Create the full file name*/
        full_file_name = merge_str_names(file_name, ".as");

        /*Check if the file is exist */
        if (isFileExist(full_file_name)) {
            strcpy(argv[file_index], (const char *) full_file_name);
            handle_file(full_file_name);
        } else {
            /*The file is not exist*/
            print_internal_error(ERROR_CODE_4);
        }
    }

    free(full_file_name);
    return SUCCESS;
}


int handle_file(char full_file_name[]) {
    FILE *file_ptr;
    long ic = IC_INIT_VALUE, dc = 0;
    table symbol_table = NULL;


    file_ptr = fopen(full_file_name, "r");


    /*Pre Process*/



    /*First Pass*/
    start_first_pass();

    /*Second Pass*/
    start_first_pass();
}

int start_first_pass() {


    return SUCCESS;
}

int start_second_pass() {


    return SUCCESS;
}



