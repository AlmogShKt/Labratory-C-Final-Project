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
        /* line is blank */
        ;
    }
    else {
        /* line number %d is not blank */
        ;
        blank_line = 0;
    }
    line_num++;
    return blank_line;
}

void remove_blank_lines(FILE *fp, char file_name[]){
    int i;
    char str[SIZE];
    FILE *fp_temp;
    str[0] = '\0';
    strcat(file_name,"1.tmp");
    fp_temp = fopen(file_name,"w");
    if(fp_temp == NULL){
        printf("fopen in 'remove_blank_lines' failed");
        return;
    }
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

void remove_extra_spaces_file(FILE *fp, char file_name[]){
    int *c;
    int i;
    char str[SIZE];
    FILE *fp_temp;
    str[0] = '\0';
    c = strchr(file_name,'1');
    *c = '\0';
    strcat(file_name,"2.tmp");
    fp_temp = fopen(file_name,"w");
    if(fp_temp == NULL){
        printf("fopen in 'remove_extra_spaces_file' failed");
        return;
    }
    while(fgets(str,SIZE,fp) != NULL){
        remove_extra_spaces_str(str);
        fprintf(fp_temp,"%s",str);
    }
    fclose(fp);
    fclose(fp_temp);
}

char *copy_text(FILE *fp, fpos_t **pos, int length){
    /* the function assumes that pos + length < end */
    int i;
    char *str;
    if(fsetpos(fp,pos) != 0){
        printf("fset in copy_text failed\n");
        return NULL;
    }
    str = malloc((length+1) * sizeof(char));{
        if(str == NULL){
            printf("malloc in copy_text failed\n");
            free(str);
            return NULL;
        }
    }
    for(i = 0; i < length; i++){
        *(str+i) = getc(fp);
    }
    *(str+i) = '\0';
    fgetpos(fp,*pos);
    return str;
}

char *save_mcro_content(FILE *fp, fpos_t **pos){
    int mcro_length;
    char *mcro;
    char str[SIZE];
    if(fsetpos(fp,*pos) != 0){
        printf("setting position to read macro failed\n");
        return NULL;
    }
    mcro_length = 0;
    fgets(str,SIZE,fp);
    while(!feof(fp) && (strcmp(str,"endmcro")) != 0){
        mcro_length += strlen(str);
    }
    if(feof(fp)){
        printf("macro without 'endmcro'\n");
        return NULL;
    }
    mcro = copy_text(fp,pos,mcro_length);
    return mcro;
}

void search_mcro(FILE *fp){
    char str[SIZE];
    char *found, *token, *name, *extra_text;
    fpos_t *pos;
    fgets(str,SIZE,fp);
    while(!feof(fp)){
        if(found = strstr(str,"mcro")){
            token = strtok(found," ");
            name = strtok(NULL," \n");
            if(name == NULL){
                printf("mcro without name\n");
                continue;
            }
            extra_text = strtok(NULL,"\n");
            if(extra_text != NULL){
                printf("extra text after macro name\n");
                continue;
            }
            fgetpos(fp,pos);
            save_mcro_content(fp,&pos);
            /* going to the end of the macro */


            /*
             * need to take the macro into the tree
             */
            fsetpos(fp,pos);
        }
    }

}