#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <String.h>
#include "preproc.h"
#include "util.h"
#include "globals.h"

char *remove_extra_spaces_file(FILE *fp, char file_name[]){
    char *new_file_name;
    int i;
    char str[MAX_LINE_LENGTH];
    new_file_name = add_new_file(file_name,".t01");
    FILE *fp_temp;
    fp_temp = fopen(new_file_name,"w");
    if(fp_temp == NULL){
        printf("fopen in 'remove_extra_spaces_file' failed");
        return NULL;
    }
    while(!feof(fp)){
        fgets(str,MAX_LINE_LENGTH,fp);
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
    /* the function assumes that pos + length < end. this was checked by save_mcro_content function*/
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

char *save_mcro_content(FILE *fp, fpos_t *pos){
    int mcro_length;
    char *mcro;
    char str[MAX_LINE_LENGTH];
    if(fsetpos(fp,pos) != 0){
        printf("setting position to read macro failed\n");
        return NULL;
    }
    mcro_length = 0;

    while(!feof(fp) && (strcmp(str,"endmcro\n")) != 0){
        fgets(str,MAX_LINE_LENGTH,fp);
        if(strcmp(str,"endmcro\n") != 0){
            mcro_length += strlen(str);
        }
    }
    if(strcmp(str,"endmcro\n") != 0){
        printf("macro without 'endmcro'\n");
        return NULL;
    }
    mcro = copy_text(fp,pos,mcro_length);
    return mcro;
}

int valid_mcro_decl(char *str, char **name){
    char *extra_text, *temp_name, *extra;
    temp_name = strtok(NULL, " \n");
    if (temp_name == NULL) {
        printf("mcro without name\n");
        return 0;
    }
    extra = strtok(NULL, "\n");
    if (extra != NULL) {
        printf("extra text after macro name\n");
        return 0;
    }
    *name = temp_name;
    return 1;
}

void add_mcros(FILE *fp, node **head){
    char str[MAX_LINE_LENGTH];
    char *name, *content;
    fpos_t pos;
    while(!feof(fp)){
        fgets(str,MAX_LINE_LENGTH,fp);
        if(strcmp(strtok(str," "),"mcro") == 0){
            if(!valid_mcro_decl(str,&name)){
                continue;
            }
            /* debugging search_mcros */
            fgetpos(fp,&pos);
            content = save_mcro_content(fp,&pos);
            /* adding the new mcro into the mcro_BST */
            add_to_list(head,name,content);
            /* going to the end of the macro */
            fsetpos(fp,&pos);
        }
    }
}

char *remove_mcros_decl(FILE *fp, char file_name[]){
    char *token, *new_file;
    char str[MAX_LINE_LENGTH];
    char str_copy[MAX_LINE_LENGTH];
    FILE *fp_out;
    new_file = add_new_file(file_name,".t02");
    fp_out = fopen(new_file,"w");
    do{
        fgets(str,MAX_LINE_LENGTH,fp);
        if(feof(fp)){
            break;
        }
        strcpy(str_copy,str);
        token = strtok(str," \n");
        if(token == NULL){
            fprintf(fp_out,"\n");
            continue;
        }
        if(strcmp(token,"mcro") == 0){
            while(strcmp(token,"endmcro") != 0){
                fprintf(fp_out,"\n");
                fgets(str,MAX_LINE_LENGTH,fp);
                token = strtok(str," \n");
                while(token == NULL){
                    fprintf(fp_out,"\n");
                    fgets(str,MAX_LINE_LENGTH,fp);
                    token = strtok(str," \n");
                }
            }
            fprintf(fp_out,"\n");
        }
        else {
            fprintf(fp_out,"%s",str_copy);
        }
    }
    while(!feof(fp));
    fclose(fp);
    fclose(fp_out);
    return new_file;
}

char *replace_mcro(char *str,node *mcro){
    char *pos, *new_str;
    char str_beg[MAX_LINE_LENGTH];
    char str_end[MAX_LINE_LENGTH];
    strcpy(str_beg,str);
    pos = strstr(str_beg,mcro->name);
    *pos = '\0';
    strcpy(str_end,pos+strlen(mcro->name));
    new_str = malloc((strlen(str_beg) + strlen(mcro->content) + strlen(str_end) + 1) * sizeof(char));
    if(new_str == NULL){
        printf("malloc in replace_mcro failed");
        exit(1);
    }
    strcpy(new_str,str_beg);
    strcat(new_str,mcro->content);
    strcat(new_str,str_end);
    return new_str;
}

char *replace_mcros(FILE *fp, char file_name[], node *head){
    node *mcro;
    char *pos, *new_str, *file_temp, *final_file_name;
    char str[MAX_LINE_LENGTH];
    FILE *fp_temp, *fp_final;
    file_temp = add_new_file(file_name,".tmp");
    final_file_name = add_new_file(file_name,".am");
    copy_file(file_temp,file_name);
    fclose(fp);
    mcro = head;
    while(mcro != NULL){
        fp_temp = fopen(file_temp,"r");
        if(fp_temp == NULL){

            exit(1);
        }
        fp_final = fopen(final_file_name,"w");
        if(fp_temp == NULL){
            printf("fopen failed\n");
            exit(1);
        }
        while(!feof(fp_temp)){
            fgets(str,MAX_LINE_LENGTH,fp_temp);
            if(feof(fp)){
                break;
            }
            pos = strstr(str,mcro->name);
            if(pos != NULL) {
                /* remove the '\n' at the end str */
                *(str+strlen(str)-1) = '\0';
                new_str = replace_mcro(str,mcro);
                if (new_str == NULL) {
                    printf("replace_mcro failed\n");
                    exit(1);
                }
                fprintf(fp_final,"%s",new_str);
            }
            else {
                fprintf(fp_final, "%s", str);
            }
        }
        fclose(fp_temp);
        fclose(fp_final);
        mcro = mcro->next;
        if(mcro == NULL){
            break;
        }
        remove(file_temp);
        rename(final_file_name,file_temp);
    }
    fclose(fp_temp);
    remove(file_temp);
    fclose(fp_final);
    return final_file_name;
}