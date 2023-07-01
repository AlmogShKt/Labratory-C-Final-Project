#ifndef PREPROC
#define PREPROC

#include <stdio.h>
#include "data_strct.h"


char *copy_text(FILE *fp, fpos_t *pos, int length);
char *save_mcro_content(FILE *fp, fpos_t *pos, int *line_count);
void add_mcros(char *file_name, node **head);
int valid_mcro_decl(char *str, char **name, int line_count, char *file_name);
char *replace_all_mcros(char file_name[], node *head);
char *remove_mcros_decl(char file_name[]);
char *replace_mcro(char *str,node *mcro);
int mcro_call_before_decl(char file_name[], node *head);

/**
 * This function executes the macro replacement process
 * @param file_name string of the file name to work on
 * @return 1 if the process successfully finished, 0 if not
 */
int mcro_exec(char file_name[]);

#endif