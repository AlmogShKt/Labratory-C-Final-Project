#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "preproc.h"
#include "util.h"
#include "globals.h"
#include "handle_text.h"
#include "Errors.h"

char *remove_extra_spaces_file(char file_name[]){
    char *new_file_name;
    char str[MAX_LINE_LENGTH];
    FILE *fp, *fp_temp;
    fp = fopen(file_name,"r");
    if(fp == NULL){
        print_internal_error(ERROR_CODE_2);
        return NULL;
    }
    new_file_name = add_new_file(file_name,".t01");
    if(new_file_name == NULL){
        abrupt_close(2,"file",fp);
        return NULL;
    }
    fp_temp = fopen(new_file_name,"w");
    if(fp_temp == NULL){
        abrupt_close(4,"file",fp,"%s",new_file_name);
        print_internal_error(ERROR_CODE_7);
        return NULL;
    }
    while(fgets(str,MAX_LINE_LENGTH,fp) != NULL){
        if(*str == ';'){
            *str = '\n';
            *(str+1) = '\0';
        }
        else {
            remove_extra_spaces_str(str);
        }
        fprintf(fp_temp,"%s",str);
    }
    putc('\n',fp_temp);
    fclose(fp);
    fclose(fp_temp);
    return new_file_name;
}

char *copy_text(FILE *fp, fpos_t *pos, int length){
    /* the function assumes that pos + length < end. this was checked by save_mcro_content function */
    int i;
    char *str;
    if(fsetpos(fp,pos) != 0){
        printf("fsetpos in copy_text failed\n");
        return NULL;
    }
    str = handle_malloc((length+1) * sizeof(char));
    for(i = 0; i < length; i++){
        *(str+i) = getc(fp);
    }
    *(str+i) = '\0';
    fgetpos(fp,pos);
    return str;
}

void remove_extra_spaces_str(char str[]){
    int i, j;
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
            remove_spaces_next_to_comma(str_temp);
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
            remove_spaces_next_to_comma(str_temp);
            strcpy(str, str_temp);
            return;
        }
        /* a new non-white-space is found */
    }
}

int is_space_or_tab(char c) {
    return (isspace(c) && c != '\n');
}

void remove_spaces_next_to_comma(char *str){
    char *ptr;
    if((ptr = strchr(str,',')) != NULL){
        /* space before the comma */
        if(*(ptr-1) == ' '){
            memmove(ptr-1,ptr,strlen(ptr)+1);
            if(*(ptr) == ' '){
                /* also space after the comma */
                memmove(ptr,ptr+1,strlen(ptr+1)+1);
            }
        }
        if(*(ptr+1) == ' '){
            /* only space after the comma */
            memmove(ptr+1,ptr+2,strlen(ptr+2)+1);
        }
    }
}