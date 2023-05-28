#include <stdio.h>
#include "data_strct.h"

char *copy_text(FILE *fp, fpos_t *pos, int length);
char *save_mcro_content(FILE *fp, fpos_t *pos);
void add_mcros(char *file_name, node **head);
int valid_mcro_decl(char *str, char **name);
char *replace_all_mcros(char file_name[], node *head);
char *remove_mcros_decl(char file_name[]);
char *replace_mcro(char *str,node *mcro);
void mcro_exec(char file_name[]);