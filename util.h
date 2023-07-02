#ifndef LABRATORY_C_FINAL_PROJECT_UTIL_H
#define LABRATORY_C_FINAL_PROJECT_UTIL_H

#include "globals.h"

/**
 * This function converts a short type number to a string of base64
 * @param num an unsigned short number to convert
 * @return a string with the converted value in base64
 */
char *short_to_base64(unsigned short num);

/**
 * This function converts a negative short type number to an unsigned short in the method of two's complement
 * @param pos_num a negative number
 * @return an unsigned type number according to the two's complement method
 */
unsigned short twos_compliment(unsigned short pos_num);

/**
 * This function allocates new memory and handles the errors that might occur
 * @param object_size the amount of memory to allocate
 * @return a void pointer of the allocated memory, or NULL if the allocation failed
 */
void *handle_malloc(long object_size);

/**
 * This function saves a new name for a file. It deletes the content of the name after the '.' if one exists
 * and adds a new ending
 * @param file_name string of the beginning of the new file name
 * @param ending string of the ending of the new file name
 * @return a string with the new file name
 */
char *add_new_file(char *file_name, char *ending);

/**
 * This function creates an identical copy of a file
 * @param file_name_dest a string with the name of the new file
 * @param file_name_orig a string with the name of the original file
 * @return 1 if the copying process succeeded, 0 if not
 */
int copy_file(char *file_name_dest, char *file_name_orig);

/**
 * This function handles the process of closing correctly opened files and deallocation of file name strings
 * @param num_args the number of files and strings times 2 (each contains an identifier before it)
 * @param ... pairs of arguments with the file or string identifier ("%s" for string or "file" for a file)
 * and the string or file name
 */
void abrupt_close(int num_args, ...);
char *short_to_binary(unsigned short num);
int isFileExist(char file_name[]);

char *merge_str_names(char *str_name_1, char *str_name_2);

#endif