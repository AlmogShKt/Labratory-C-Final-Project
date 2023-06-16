#include <stdio.h>
#include "first_pass.h"
#include "globals.h"
#include "first_pass.h"
#include "Errors.h"

int main(int argc, char *argv[]){
    int num_files, error_code;
    command_parts *command;
    char str[MAX_LINE_LENGTH];
    FILE *fp;
    num_files = argc-1;
    while(--argc > 0){
        if(!lines_too_long(argv[argc])){
            printf("lines are not too long\n");
        }
        else{
            printf("lines are too long\n");
        }

    }
    error_code = 0;
    fp = fopen(argv[num_files],"r");
    fgets(str,MAX_LINE_LENGTH,fp);
    //line_data line = {argv[num_files-1],0,str};
    command = read_command(str,&error_code);
    if(error_code){
        //print_external_error(ERROR_CODE_32, line);
    }


    return 0;
}

