#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "preproc.h"
#include "util.h"

int main(){
    FILE *fp, *fp1, *fp2;
    char file_name[MAX_LINE_LENGTH];
    node *head;
    strcpy(file_name,"input");
    fp = fopen(file_name,"r");
    if(fp == NULL){
        printf("fopen before remove blank lines in 'main' failed");
        exit (1);
    }
    remove_blank_lines(fp,file_name);
    fclose(fp);
    fp1 = fopen(file_name,"r");
    if(fp1 == NULL){
        printf("fopen before remove extra spaces file in 'main' failed");
        exit (1);
    }
    remove_extra_spaces_file(fp1,file_name);
    fclose(fp1);
    fp2 = fopen(file_name,"r");
    head = malloc(sizeof(node));
    search_mcros(fp2,&head);

    return 0;
}