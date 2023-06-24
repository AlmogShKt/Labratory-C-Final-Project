#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "util.h"
#include "Errors.h"

void rev_str(char *str){

}

unsigned short twos_compliment(unsigned short pos_num){
    return ~pos_num+1;
}

char *short_to_binary(unsigned short num){
    char *res, *ptr;
    int i;
    if(num < 0){
        num = twos_compliment(num);
    }
    res = handle_malloc((WORD_LEN+1) * sizeof(char));
    if(res == NULL){
        print_internal_error(ERROR_CODE_1);
        return NULL;
    }
    ptr = res;
    for(i = 0; i < WORD_LEN; i++){
        *(ptr+i) = '0';
    }
    *(ptr+WORD_LEN) = '\0';
    ptr = res + WORD_LEN-1;
    while (num != 0){
        *ptr = (num % 2) + '0';
        num /= 2;
        if(num != 0){
            ptr--;
        }
    }
    return res;
}


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

/**
 * Trying to allocate memory and check is success
 * @param object_size the object size
 * @return a pointer to the address
 */
void *handle_malloc(long object_size) {
    void *object_ptr = malloc(object_size);
    if (object_ptr == NULL) {
        print_internal_error(ERROR_CODE_1);
    }
    return object_ptr;
}

char *add_new_file(char *file_name, char *ending){
    char *c, *new_file_name;
    new_file_name = handle_malloc(MAX_LINE_LENGTH * sizeof(char));
    strcpy(new_file_name,file_name);
    c = strchr(new_file_name,'.');
    *c = '\0';
    strcat(new_file_name,ending);
    return new_file_name;
}

int copy_file(char *file_name_dest, char *file_name_orig){
    char str[MAX_LINE_LENGTH];
    FILE *fp, *fp_dest;
    fp = fopen(file_name_orig,"r");
    if(fp == NULL){
        print_internal_error(ERROR_CODE_8);
        return 0;
    }
    fp_dest = fopen(file_name_dest,"w");
    if(fp_dest == NULL){
        print_internal_error(ERROR_CODE_7);
        fclose(fp);
        return 0;
    }
    while(!feof(fp)){
        fgets(str,MAX_LINE_LENGTH,fp);
        if(feof(fp)){
            break;
        }
        fprintf(fp_dest,"%s",str);
    }
    fclose(fp);
    fclose(fp_dest);
    return 1;
}


void abrupt_close(int num_args, ...){
    int i;
    char *str;
    FILE *fp;
    va_list args;
    va_start(args,num_args);
    for (i = 0; i < num_args; i++){
        /* next argument is a string whose allocated memory needs to be freed*/
        if(strcmp(va_arg(args,char*),"%s") == 0){
            i++;
            str = va_arg(args,char*);
            free(str);
        }
        /* next argument is a file pointer that needs to be closed */
        else {
            fp = va_arg(args,FILE*);
            fclose(fp);
        }
    }
    va_end(args);
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