#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Errors.h"
#include "globals.h"
#include "table.h"
#include "util.h"


int start_first_pass(int *ic, int *dc, char file_name[], table symbol_table);

int start_second_pass();

int handle_file(char file_name[]);


/**
 *
 * The main function is initiating the whole process. for each file name will be 3 runs.
 * 1: macros
 * 2: first pass
 * 3: second pass
 * */

int main(int argc, char *argv[]) {
    int file_index;
    /*? What is the max file name?*/
    char file_name[MAX_LABEL_LENGTH], *full_file_name;


    /*In case that .as files name was not provided in the arguments*/
    if (argc <= 1) {
        print_internal_error(ERROR_CODE_2);
        exit(FAILED);
    }

    /*For each file was provided*/
    for (file_index = 1; file_index < argc; file_index++) {
        /*Copy the file name to local variable*/
        strcpy(file_name, argv[file_index]);

        /*Check if the file name is too long*/
        if (strlen(file_name) > MAX_LABEL_LENGTH) {
            print_internal_error(ERROR_CODE_3);

            /*! Change exit to continue */
            exit(FAILED);
        }

        /*Create the full file name*/
        full_file_name = merge_str_names(file_name, AS_FILE_EXT);

        /*Check if the file is exist */
        if (isFileExist(full_file_name)) {
            handle_file(file_name);
        } else {
            /*The file is not exist*/
            printf("--%s--\n", full_file_name);
            print_internal_error(ERROR_CODE_4);
        }
    }

    free(full_file_name);
    return SUCCESS;
}


int handle_file(char file_name[]) {
    FILE *file_ptr;
    long ic = IC_INIT_VALUE, dc = 0;
    /*If error found, we cant process the file*/
    int error_found = FALSE;
    table symbol_table = NULL;
    machine_word a;





    /*Pre Process*/




    /*End of PRE*/



    /*First Pass*/
    start_first_pass((int *) &ic, (int *) &dc, file_name, (table) &symbol_table);


    return SUCCESS;
}


int start_first_pass(int *ic, int *dc, char file_name[], table symbol_table) {
    FILE *file_ptr = NULL;
    line_data current_line_struct;
    char *full_file_name, current_line_data[MAX_LINE_LENGTH];

    full_file_name = merge_str_names(file_name, AM_FILE_EXT);
    current_line_struct.file_name = full_file_name;

    if (isFileExist(full_file_name)) {
        file_ptr = fopen(full_file_name, "w");
        for (current_line_struct.number = 0;
             fgets(current_line_data, MAX_LINE_LENGTH, file_ptr) != NULL; current_line_struct.number++) {
            /* Handle the case of too long line, not all the char was get in to the buffer, so we cant handle this line - file */
            if (!feof(file_ptr) && !strchr(current_line_data, '\n')) {
                print_external_error(ERROR_CODE_6, current_line_struct);
                /*! Handle left overs*/

            } else {
                /* Handle first process */


                continue;
            }
        }
    } else {
        print_internal_error(ERROR_CODE_5);
    }


    return SUCCESS;
}


int start_second_pass() {


    return SUCCESS;
}