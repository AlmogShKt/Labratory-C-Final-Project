#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preproc.h"
#include "first_pass.h"
#include "second_pass.h"
#include "util.h"


int main(int argc, char *argv[]){
    char *as_file, *am_file;
    while(--argc > 0){
        as_file = add_new_file(argv[argc], ".as");
        if(mcro_exec(as_file) == 0){
            continue;
        }
        am_file = add_new_file(argv[argc], ".am");
        if(exe_first_pass(am_file)){
            continue;
        }
    }
    free(as_file);
    free(am_file);
    return 0;
}