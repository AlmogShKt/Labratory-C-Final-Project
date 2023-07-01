#ifndef LABRATORY_C_FINAL_PROJECT_UTIL_H
#define LABRATORY_C_FINAL_PROJECT_UTIL_H

#include "globals.h"

/* Moves the index to the next position in the string where the character is not a whitespace ot tab */
#define SKIP_WHITE_SPACE(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)));

// char convert_to_base64(char binary_number[]);
char *short_to_base64(unsigned short num);
unsigned short twos_compliment(unsigned short pos_num);
void *handle_malloc(long object_size);

/**
 * This function saves a new name for a file. It deletes the content of the name after the '.'
 * @param file_name string of the beginning of the new file name
 * @param ending string of the ending of the new file name
 * @return
 */
char *add_new_file(char *file_name, char *ending);
int copy_file(char *file_name_dest, char *file_name_orig);
void abrupt_close(int num_args, ...);
char *short_to_binary(unsigned short num);
int isFileExist(char file_name[]);

char *merge_str_names(char *str_name_1, char *str_name_2);

#endif