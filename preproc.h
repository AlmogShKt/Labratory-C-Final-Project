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
 * @param file_name a string with the name of the source file
 * @param head a double pointer to head of a linked list where the macros are saved
 */
void add_mcros(char *file_name, node **head);

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
 * This function replaces all the macro calls in the source file
 * @param file_name a string with the name of the source file
 * @param head a pointer to head of a linked list where the macros were saved
 * @return a string to the name of a new file after the macros has been expanded
 */
char *replace_all_mcros(char file_name[], node *head);

/**
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
char *replace_mcro(char *str,node *mcro);

/**
 * This function checks if there's a macro call before it's definition in the source file
 * @param file_name a string with the name of the source file
 * @param head a pointer to head of a linked list where the macros were saved
 * @return 1 if there's a macro call before it's definition in the source file, 0 if not
 */
int mcro_call_before_decl(char file_name[], node *head);

/**
 * This function executes the macro expansion process
 * @param file_name a string with the name of the source file
 * @return 1 if the process successfully finished, 0 if not
 */
int mcro_exec(char file_name[]);

#endif