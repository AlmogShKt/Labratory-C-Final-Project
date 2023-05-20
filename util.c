#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "util.h"
/*
char convert_to_base64(char binary_number[]) {
    char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int decimal = 0;
    char base64_number;
    for (int i = 0; i < 6; i++) {
        decimal += (binary_number[i] - '0') << (5 - i);
    }

    base64_number = base64_table[decimal];
    printf("%c\n", base64_number);
    return base64_number;

}
*/

void remove_extra_spaces_str(char str[]){
    int i, j, blank;
    char str_temp[MAX_LINE_LENGTH];
    blank = 0;
    i = j = 0;
    while(*(str+i) != '\0'){
        /* coping content until first white-space is found */
        while(*(str+i) != '\0' && !is_space_or_tab(*(str+i))){
            *(str_temp + j) = *(str+i);
            i++;
            j++;
        }
        if(*(str+i) == '\0'){
            *(str_temp + j) = *(str+i);
            strcpy(str,str_temp);
            return;
        }
        /* white-space is found, copy this one and skipping all other white-spaces */
        blank = 1;
        *(str_temp + j) = *(str+i);
        i++;
        j++;
        while(*(str+i) != '\0' && is_space_or_tab(*(str+i))){
            i++;
        }
        if(*(str+i) == '\0') {
            *(str_temp + j) = *(str + i);
            strcpy(str, str_temp);
            return;
        }
        /* new non-white-spaces are found */
        blank = 0;
    }
}

int is_space_or_tab(char c){
    return (isspace(c) && c != '\n');
}
