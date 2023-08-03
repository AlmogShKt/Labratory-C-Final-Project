#ifndef PREPROC
#define PREPROC

#include <stdio.h>
#include "data_strct.h"

/**
 * This function copies the content of a macro
 * @param fp file pointer to a source file
 * @param pos the position where the content of the macro begins
 * @param line_count a pointer to the line number from which we are reading the macro content in the source file
 * @return a string with the macro content
 */
char *save_mcro_content(FILE *fp, fpos_t *pos, int *line_count);

/**
 * This function reads a source file and saves all the macros in it
 * @param file_name A string with the name of the source file
 * @param head      A double pointer to head of a linked list where the macros are saved
 * @return Returns 1 if all macros are successfully added, 0 otherwise.
 */
int add_mcros(char *file_name, node **head);

/**
 * This function checks of the name of a macro is valid according to the rules
 * @param str a string with the line content read from the source file
 * @param name a double pointer to a string with the name of the macro read from the source file
 * @param line_count the line number from which we are reading the macro content in the source file
 * @param file_name a string with the name of the source file
 * @return 1 if the macro declaration is valid, 0 if it isn't
 */
int valid_mcro_decl(char *str, char **name, int line_count, char *file_name);

/**
 * Replaces all occurrences of macros in the input file and creates a new modified file.
 * This function replaces all the macro calls in the source file
 * @param file_name a string with the name of the source file
 * @param head a pointer to head of a linked list where the macros were saved
 * @return a string to the name of a new file after the macros has been expanded
 */
char *replace_all_mcros(char file_name[], node *head);

/**
 * Removes macro declarations from the given file and creates a new file without them.
 * This function deletes the lines where the macros in a source file were defined
 * @param file_name a string with the name of the source file
 * @return a string to the name of a new file after the macros definition were deleted
 */
char *remove_mcros_decl(char file_name[]);

/**
 * This function replaces a call to a macro in a line with it's definition and saves the line
 * with the expanded macro to a string
 * @param str a string with the content of the line where a macro call is present
 * @param mcro a node in a lined list where the macro was saved.
 * @return a string with the line including the expanded macro
 */
char *replace_mcro(char *str, node *mcro);

/**
 * Checks for macro calls before their declarations in the input file.
 * This function checks if there's a macro call before it's definition in the source file
 * @param file_name a string with the name of the source file
 * @param head a pointer to head of a linked list where the macros were saved
 * @return 1 if there's a macro call before it's definition in the source file, 0 if not
 */
int mcro_call_before_decl(char file_name[], node *head);

/**
 * Executes the macro expansion process for the input file.
 *
 * This function performs the macro expansion process on the input file, which includes several steps:
 * 1. Removing unnecessary white spaces in the input file and saving the result in a new temp file.
 * 2. Scanning and saving all the macros in the input file in a linked list of macros.
 * 3. Ensuring that there is no macro call before its declaration in the input file.
 * 4. Removing the declaration of the macros from the input file and saving the result in a new temp file.
 * 5. Replacing all macro calls with their definitions as saved in the linked list and saving the result in a new file.
 * 6. Cleaning up temporary files and freeing memory.
 *
 * @param file_name The name of the source file to perform macro expansion on.
 * @return Returns 1 if the macro expansion process is successful, 0 otherwise.
 */
int mcro_exec(char file_name[]);

#endif