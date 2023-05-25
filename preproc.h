#include "data_strct.h"

char *save_mcro_content(FILE *fp, fpos_t *pos);
void add_mcros(FILE *fp, node **head);
int valid_mcro_decl(char *str, char **name);
char *replace_all_mcros(FILE *fp, char file_name[], node *head);
char *remove_mcros_decl(FILE *fp, char file_name[]);
char *replace_mcro(char *str,node *mcro);
void mcro_exec(char file_name[]);