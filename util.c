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


void remove_extra_spaces_str(char str[]){
    int i, j, blank;
    char str_temp[MAX_LINE_LENGTH];
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
        /* white-space is found, copy one space and skipping all other white-spaces */
        *(str_temp + j) = ' ';
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
        /* a new non-white-space is found */
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
//        print_internal_error(ERROR_CODE_1);
        exit(1);
    }
    return object_ptr;

}

char *add_new_file(char *file_name, char *ending){
    char *c, *new_file_name;
    new_file_name = malloc(MAX_LINE_LENGTH * sizeof(char));
    strcpy(new_file_name,file_name);
    c = strchr(new_file_name,'.');
    *c = '\0';
    strcat(new_file_name,ending);
    return new_file_name;
}

void copy_file(char *file_name_dest, char *file_name_orig){
    char str[MAX_LINE_LENGTH];
    FILE *fp, *fp_dest;
    fp = fopen(file_name_orig,"r");
    fp_dest = fopen(file_name_dest,"w");
    while(!feof(fp)){
        fgets(str,MAX_LINE_LENGTH,fp);
        if(feof(fp)){
            break;
        }
        fprintf(fp_dest,"%s",str);
    }
    fclose(fp);
    fclose(fp_dest);
}
