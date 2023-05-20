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
    char str[MAX_LINE_LENGTH];
    FILE *fp_temp;
    str[0] = '\0';
    strcat(file_name,"1.tmp");
    fp_temp = fopen(file_name,"w");
    if(fp_temp == NULL){
        printf("fopen in 'remove_blank_lines' failed");
        return;
    }
    while(fgets(str,MAX_LINE_LENGTH,fp) != NULL){
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
    char *c;
    int i;
    char str[MAX_LINE_LENGTH];
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
    fgets(str,MAX_LINE_LENGTH,fp);
    while(!feof(fp)){
        remove_extra_spaces_str(str);
        fprintf(fp_temp,"%s",str);
        fgets(str,MAX_LINE_LENGTH,fp);
    }
    fclose(fp);
    fclose(fp_temp);
}

char *copy_text(FILE *fp, fpos_t **pos, int length){
    /* the function assumes that pos + length < end. this was checked by save_mcro_content function*/
    int i;
    char *str;
    if(fsetpos(fp,*pos) != 0){
        printf("fsetpos in copy_text failed\n");
        return NULL;
    }
    str = malloc((length+1) * sizeof(char));{
        if(str == NULL){
            printf("malloc in copy_text failed\n");
            free(str);
            /* todo - why free if malloc failed? */
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
    char str[MAX_LINE_LENGTH];
    if(fsetpos(fp,*pos) != 0){
        printf("setting position to read macro failed\n");
        return NULL;
    }
    mcro_length = 0;
    fgets(str,MAX_LINE_LENGTH,fp);
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

int valid_mcro_decl(char *str, char **name){
    char *extra_text;
    if(strcmp(strtok(str," "),"mcro") == 0) {
        *name = strtok(NULL, " \n");
        if (*name == NULL) {
            printf("%s\tmcro without name\n", str);
            return 0;
        }
        extra_text = strtok(NULL, "\n");
        if (extra_text != NULL) {
            printf("%s\textra text after macro name\n", str);
            return 0;
        }
        return 1;
    }
    printf("no mcro declaration in line\n");
    return 0;
}

void search_mcros(FILE *fp, node **head){
    char str[MAX_LINE_LENGTH];
    char *name, *content, *extra_text;
    fpos_t *pos;
    fgets(str,MAX_LINE_LENGTH,fp);
    while(!feof(fp)){
        if(strcmp(strtok(str," "),"mcro") == 0){
            if(!valid_mcro_decl(str,&name)){
                continue;
                /* todo: problem - does not read new line */
            }
            fgetpos(fp,pos);
            content = save_mcro_content(fp,&pos);
            /* adding the new mcro into the mcro_BST */
            add_to_tree(head,name,content);
            /* going to the end of the macro */
            fsetpos(fp,pos);
        }
        fgets(str,MAX_LINE_LENGTH,fp);
    }
}

void replace_mcros(FILE *fp,node *head, char file_name[]){
    char *c;
    char str[MAX_LINE_LENGTH];
    char *token, *mcro;
    FILE *fp_outout;
    c = strchr(file_name,'2');
    *c = '\0';
    strcat(file_name,".am");
    /* todo - first detele .as and only after add .am */
    fp_outout = fopen(file_name,"w");
    fgets(str,MAX_LINE_LENGTH,fp);
    while(!feof(fp)){
        if(strcmp((token = strtok(str," \n")),"mcro") == 0){
            if(!valid_mcro_decl(str,&mcro)){
                continue;
            }

        }
    }
}