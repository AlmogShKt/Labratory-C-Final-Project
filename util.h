#ifndef LABRATORY_C_FINAL_PROJECT_UTIL_H
#define LABRATORY_C_FINAL_PROJECT_UTIL_H

#define MAX_LINE_LENGTH 80

char convert_to_base64(char binary_number[]);

void remove_extra_spaces_str(char str[]);

int is_space_or_tab(char c);

void *handle_malloc(long object_size);

int isFileExist(char file_name[]);

char *merge_str_names(char *str_name_1, char *str_name_2);


#endif