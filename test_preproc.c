#include <stdio.h>
#include <string.h>
#include "preproc.h"
#include "util.h"

int main(){
    /*
    FILE *fp;
    char file_name[SIZE];
    strcpy(file_name,"input");
    fp = fopen(file_name,"r");
    clear_blank_lines(fp,file_name);
     */
    char str[SIZE];
    strcpy(str,"abc   def    g   h    ijklm      ");
    printf("before: %s\n",str);
    remove_extra_spaces_str(str);
    printf("after: %s\n",str);



    return 0;
}