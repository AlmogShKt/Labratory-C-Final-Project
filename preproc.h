#include "data_strct.h"

char *remove_extra_spaces_file(FILE *fp, char file_name[]);
char *copy_text(FILE *fp, fpos_t *pos, int length);
char *save_mcro_content(FILE *fp, fpos_t *pos);
void add_mcros(FILE *fp, node **head);
int valid_mcro_decl(char *str, char **name);
char *replace_mcros(FILE *fp, char file_name[], node *head);
char *remove_mcros_decl(FILE *fp, char file_name[]);
char *replace_mcro(char *str,node *mcro);