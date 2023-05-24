#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "preproc.h"
#include "util.h"

int main(){
    node *node1, *head, *search;
    FILE *fp;
    fpos_t pos;
    int length, i, found;
    char file_name[MAX_LINE_LENGTH];
    char *temp;
    char str[MAX_LINE_LENGTH];
    char *name, *content;

    strcpy(file_name,"input_test.as");
    fp = fopen(file_name,"r");
    if(fp == NULL){
        printf("fopen 0 in main failed\n");
        exit(1);
    }
    char *new_file1 = remove_extra_spaces_file(fp,file_name);
    printf("%s\n",new_file1);
    fclose(fp);

    fp = fopen(new_file1,"r");
    if(fp == NULL){
        printf("fopen 1 in main failed\n");
        exit(1);
    }
    head = NULL;
    add_mcros(fp,&head);

    rewind(fp);
    char *new_file2 = remove_mcros_decl(fp,new_file1);
    printf("%s\n",new_file2);
    fclose(fp);

    fp = fopen(new_file2,"r");
    if(fp == NULL){
        printf("fopen 2 in main failed\n");
        exit(1);
    }
    char *file_final = replace_mcros(fp,new_file2,head);
    printf("%s\n",file_final);
    fclose(fp);


    return 0;
}