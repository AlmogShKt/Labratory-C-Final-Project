//
// Created by Almog Shtaigmann on 13/05/2023.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char *merge_str_names(char *str_name_1, char *str_name_2) {
    char *full_str_name = (char *) malloc(strlen(str_name_1) + strlen(str_name_2) + 1);
    strcpy(full_str_name, str_name_1);
    strcat(full_str_name, str_name_2);
    return full_str_name;
}

char *strallocat(char *s0, char *s1) {
    char *str = (char *) malloc(strlen(s0) + strlen(s1) + 1);
    strcpy(str, s0);
    strcat(str, s1);
    return str;
}

int main2() {
    char *a;
    a = merge_str_names("Almog", "shtaigmann");
    printf("%s\n", a);
    a = strallocat("almog", "Almog aaaa");
    printf("%s\n", a);
    return 0;
}
