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
    /* opening input file for reading */
    fp = fopen(file_name,"r");
    if(fp == NULL){
        print_internal_error(ERROR_CODE_2);
        return NULL;
    }
    /* saving new name for a temp file */
    new_file_name = add_new_file(file_name,".t01");
    if(new_file_name == NULL){
        abrupt_close(2,"file",fp);
        return NULL;
    }
    /* opening new file for writing */
    fp_temp = fopen(new_file_name,"w");
    if(fp_temp == NULL){
        abrupt_close(4,"file",fp,"%s",new_file_name);
        print_internal_error(ERROR_CODE_7);
        return NULL;
    }
    /* reading each line of the input file and removing extra unnecessary white-spaces */
    while(fgets(str,MAX_LINE_LENGTH,fp) != NULL){
        /* replacing a comment line with newline character */
        if(*str == ';'){
            *str = '\n';
            *(str+1) = '\0';
        }
        else {
            /* removing extra unnecessary white-spaces from the line */
            remove_extra_spaces_str(str);
        }
        /* saving the changed line to the new file */
        fprintf(fp_temp,"%s",str);
    }
    /* putc('\n',fp_temp); */
    /* closing files */
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
void remove_extra_spaces_str(char str[]) {
    /* i for original string, j for modified string */
    int i, j;
    char str_temp[MAX_LINE_LENGTH];
    i = j = 0;
    /* eliminating white-spaces in the beginning of the line */
    while(is_space_or_tab(*(str+i))){
        i++;
    }
    while (*(str+i) != '\n'){
        /* copying character */
        while(!is_space_or_tab(*(str+i)) && *(str+i) != '\n'){
            *(str_temp + j) = *(str+i);
            i++;
            j++;
        }
        /* if loop stopped because end of line char */
        if(*(str+i) == '\n'){
            break;
        }
        /* if loop stopped because of a white-space skipping them until another character is encountered*/
        while(is_space_or_tab(*(str+i))){
            i++;
        }
        /* if stopped not because of end of line char then copy one space for all the others that were skipped */
        if(*(str+i) != '\n'){
            *(str_temp + j) = ' ';
            j++;
        }
    }
    *(str_temp + j) = *(str+i);
    *(str_temp + j + 1) = '\0';
    remove_spaces_next_to_comma(str_temp);
    strcpy(str,str_temp);
}

void remove_extra_spaces_str1(char str[]){
    int i, j;
    char str_temp[MAX_LINE_LENGTH];
    i = j = 0;
    /* skipping all white-spaces in the beginning of the line */
    while(is_space_or_tab(*(str+i))){
        i++;
    }
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
        if(*(str+i+1) == '\0'){
            *(str_temp + j) = *(str+i);
            remove_spaces_next_to_comma(str_temp);
            strcpy(str,str_temp);
            return;
        }
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