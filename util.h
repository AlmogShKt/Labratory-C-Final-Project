#ifndef LABRATORY_C_FINAL_PROJECT_UTIL_H
#define LABRATORY_C_FINAL_PROJECT_UTIL_H

#define MAX_LINE_LENGTH 80

/* Moves the index to the next position in the string where the character is not a whitespace ot tab */
#define SKIP_WHITE_SPACE(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)));

char convert_to_base64(char binary_number[]);

void *handle_malloc(long object_size);
char *add_new_file(char *file_name, char *ending);
int copy_file(char *file_name_dest, char *file_name_orig);
void abrupt_close(int num_args, ...);

int isFileExist(char file_name[]);

char *merge_str_names(char *str_name_1, char *str_name_2);

#endif