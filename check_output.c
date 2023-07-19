//
// Created by sheme on 19/07/2023.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "check_output.h"
#include "util.h"

int chech_output(char *file_test){
    FILE *fob, *fext, *fent, *fref;
    char *str1, *str2, *f_temp, *f_ref;
    str1 = malloc(MAX_LINE_LENGTH);
    str2 = malloc(MAX_LINE_LENGTH);
    /* open reference file to compare results */
    fref = fopen((f_ref = add_new_file(file_test,".ref")),"r");
    if(fref == NULL){
        printf("couldn't open file %s\n",f_ref);
    }

    /* comparing ob file result */
    fob = fopen((f_temp = add_new_file(file_test,".ob")),"r");
    if(fob == NULL){
        printf("couldn't open file %s\n",f_temp);
    }
    while((fgets(str1,MAX_LINE_LENGTH,fob)) != NULL){
        if((fgets(str2,MAX_LINE_LENGTH,fref)) != NULL && strcmp(str1,str2) == 0){
            ;
        }
        else {
            printf("ob file does not match reference file\n");
            return 0;
        }
    }
    fclose(fob);
    free(f_temp);
    /* end of check */

    /* comparing ext file result */
    fext = fopen((f_temp = add_new_file(file_test,".ext")),"r");
    if(fext == NULL){
        printf("couldn't open file %s\n",f_temp);
    }
    while((fgets(str1,MAX_LINE_LENGTH,fext)) != NULL){
        if((fgets(str2,MAX_LINE_LENGTH,fref)) != NULL && strcmp(str1,str2) == 0){
            ;
        }
        else {
            printf("ext file does not match reference file\n");
            return 0;
        }
    }
    fclose(fext);
    free(f_temp);
    /* end of check */

    /* comparing ent file result */
    fent = fopen((f_temp = add_new_file(file_test,".ent")),"r");
    if(fent == NULL){
        printf("couldn't open file %s\n",f_temp);
    }
    while((fgets(str1,MAX_LINE_LENGTH,fent)) != NULL){
        if((fgets(str2,MAX_LINE_LENGTH,fref)) != NULL && strcmp(str1,str2) == 0){
            ;
        }
        else {
            printf("ent file does not match reference file\n");
            return 0;
        }
    }
    fclose(fent);
    free(f_temp);
    /* end of check */

    return 1;
}
