#ifndef LABRATORY_C_FINAL_PROJECT_UTIL_H
#define LABRATORY_C_FINAL_PROJECT_UTIL_H

char convert_to_base64(char binary_number[]);
void *handle_malloc(long object_size);
char *add_new_file(char *file_name, char *ending);
int copy_file(char *file_name_dest, char *file_name_orig);
void abrupt_close(int num_args, ...);

#endif