#include <stdio.h>
#include "util.h"
#include <stdlib.h>
#include "Errors.h"
#include <string.h>
#include <ctype.h>


char convert_to_base64(char binary_number[]) {
    char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int decimal = 0, i;
    char base64_number;
    for (i = 0; i < 6; i++) {
        decimal += (binary_number[i] - '0') << (5 - i);
    }

    base64_number = base64_table[decimal];
    printf("%c\n", base64_number);
    return base64_number;

}


void remove_extra_spaces_str(char str[]) {
    int i, j, blank;
    char str_temp[MAX_LINE_LENGTH];
    blank = 0;
    i = j = 0;
    while (*(str + i) != '\0') {
        /* coping content until first white-space is found */
        while (*(str + i) != '\0' && !is_space_or_tab(*(str + i))) {
            *(str_temp + j) = *(str + i);
            i++;
            j++;
        }
        if (*(str + i) == '\0') {
            *(str_temp + j) = *(str + i);
            strcpy(str, str_temp);
            return;
        }
        /* white-space is found, copy this one and skipping all other white-spaces */
        blank = 1;
        *(str_temp + j) = *(str + i);
        i++;
        j++;
        while (*(str + i) != '\0' && is_space_or_tab(*(str + i))) {
            i++;
        }
        if (*(str + i) == '\0') {
            *(str_temp + j) = *(str + i);
            strcpy(str, str_temp);
            return;
        }
        /* new non-white-spaces are found */
        blank = 0;
    }
}

int is_space_or_tab(char c) {
    return (isspace(c) && c != '\n');
}


/**
 * Trying to allocate memory and check is success
 * @param object_size the object size
 * @return a pointer to the address
 */
void *handle_malloc(long object_size) {
    void *object_ptr = malloc(object_size);
    if (object_ptr == NULL) {
        print_internal_error(ERROR_CODE_1);
        exit(1);
    }
    return object_ptr;

}


/**
 * @brief This function checks if the provided file is exist in the system
 * @param path the path to file
 * @return 0 if the file dose not exist , 1 if the file exist
 */
int isFileExist(char file_name[]) {
    FILE *file_ptr = fopen(file_name, "r");
    if (file_ptr == NULL) {
        /*File does not exist*/
        return 0;
    }
    fclose(file_ptr);
    return 1;
}

/**
 * Take 2 string and create 1 that contain both of them, str1+str2
 * @param str_name_1 the first string
 * @param str_name_2 the second string
 * @return new string
 */
char *merge_str_names(char *str_name_1, char *str_name_2) {
    char *full_str_name = (char *) malloc(strlen(str_name_1) + strlen(str_name_2) + 1);
    strcpy(full_str_name, str_name_1);
    strcat(full_str_name, str_name_2);
    return full_str_name;
}
