#include <stdio.h>
#include <stdlib.h>
#include "../Header Files /preproc.h"
#include "../Header Files /first_pass.h"
#include "../Header Files /util.h"

/**
 * This is the main function that processes the input files.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return Returns 0 on successful completion.
 */
int main(int argc, char *argv[]) {
    char *as_file, *am_file;
    while (--argc > 0) {
        /* Generate a new file with the ".as" extension by adding it to the input filename.*/
        printf("Start pre-proc\n");
        as_file = add_new_file(argv[argc], ".as");

        /*Execute the macro preprocessor on the ".as" file.*/
        if (!mcro_exec(as_file)) {
            /*If it failed, move to the next file.*/
            continue;
        }

        printf("Start first pass\n");
        /* Generate a new file with the ".am" extension by adding it to the input filename.*/
        am_file = add_new_file(argv[argc], ".am");
        /*Execute the first pass, and then the second on the ".am" file.*/
        if (exe_first_pass(am_file)) {
            /*If it failed, move to the next file.*/
            continue;
        }

        /*Free allocated memory*/
        free(as_file);
        free(am_file);
    }
    printf("end\n");
    return 0;
}