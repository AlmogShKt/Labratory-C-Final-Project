#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preproc.h"
#include "first_pass.h"
#include "second_pass.h"
#include "util.h"
#include "unistd.h"
#include "check_output.h"


int main(int argc, char *argv[]) {
    char *as_file, *am_file;
    chdir("/Users/almogshtaigmann/CLionProjects/mmn14/Labratory-C-Final-Project/Tests/AsemblerFiles-Test/To-Test");

    while (--argc > 0) {
        printf("Start pre-proc\n");
        as_file = add_new_file(argv[argc], ".as");
        if (!mcro_exec(as_file)) {
            continue;
        }
        printf("Start first pass\n");
        am_file = add_new_file(argv[argc], ".am");
        if (exe_first_pass(am_file)) {
            continue;
        }
        printf("end\n");
        free(as_file);
        free(am_file);
        /* compare output */
        if(chech_output(argv[argc]) == 1){
            printf("%s file output correct\n",argv[argc]);
        }
        else {
            printf("%s file WRONG output\n",argv[argc]);
        }
    }
    return 0;
}