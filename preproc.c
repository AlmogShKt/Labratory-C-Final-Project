#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <String.h>
#include "preproc.h"
#include "util.h"

int is_line_blank(char str[]){
    int i, blank_line;
    static int line_num = 1;
    blank_line = 1;
    i = 0;
    /* removing a comment line */
    if(*str == ';'){
        *str = '\0';
    }
    while(isspace(*(str+i)) && *(str+i) != '\0'){
        i++;
    }
    if(*(str+i) == '\0'){
        printf("line number %d is blank\n",line_num);
    }
    else {
        printf("line number %d is not blank\n",line_num);
        blank_line = 0;
    }
    line_num++;
    return blank_line;
}

void clear_blank_lines(FILE *fp, char file_name[]){
    int i;
    char str[SIZE];
    FILE *fp_temp;
    str[0] = '\0';
    strcat(file_name,"1.tmp");
    fp_temp = fopen(file_name,"w");
    while(fgets(str,SIZE,fp) != NULL){
        if(is_line_blank(str)){
            ;
        }
        else{
            fprintf(fp_temp,"%s",str);
        }
    }
    fclose(fp);
    fclose(fp_temp);
}