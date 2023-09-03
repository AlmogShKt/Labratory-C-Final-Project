#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "../Header Files /util.h"
#include "../Header Files /Errors.h"

unsigned short twos_compliment(unsigned short pos_num) {
    return ~pos_num + 1;
}

char *short_to_binary(unsigned short num) {
    char *res, *ptr;
    int i;

    /* Handle negative numbers using two's complement representation */
    if (num < 0) {
        num = twos_compliment(num);
    }

    /* Allocate memory to store the binary string */
    res = handle_malloc((WORD_LEN + 1) * sizeof(char));
    if (res == NULL) {
        print_internal_error(ERROR_CODE_1);
        return NULL;
    }

    /* Initialize the binary string to all '0's */
    ptr = res;
    for (i = 0; i < WORD_LEN; i++) {
        *(ptr + i) = '0';
    }
    *(ptr + WORD_LEN) = '\0';

    /* Convert the unsigned short integer to binary representation */
    ptr = res + WORD_LEN - 1;
    while (num != 0) {
        *ptr = (num % 2) + '0';
        num /= 2;
        if (num != 0) {
            ptr--;
        }
    }
    return res;
}

char *short_to_base64(unsigned short num) {
    unsigned short nl, nr;
    char *res;
    char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    nr = num & SIX_BITS_MAX; /* masking the 6 MSB */
    nl = (num >> SIX_BITS) & SIX_BITS_MAX; /* masking the 6 LSB */
    res = handle_malloc((NUM_SEXTETES + 1) * sizeof(char));
    if (res == NULL) {
        return NULL;
    }
    *res = *(base64_table + nl);
    *(res + 1) = *(base64_table + nr);
    *(res + 2) = '\0';
    return res;
}

void *handle_malloc(long object_size) {
    void *object_ptr = malloc(object_size);
    if (object_ptr == NULL) {
        print_internal_error(ERROR_CODE_1);
    }
    return object_ptr;
}

char *add_new_file(char *file_name, char *ending) {
    char *c, *new_file_name;
    new_file_name = handle_malloc(MAX_LINE_LENGTH * sizeof(char));
    strcpy(new_file_name, file_name);
    /* deleting the file name if a '.' exists and forth */
    if ((c = strchr(new_file_name, '.')) != NULL) {
        *c = '\0';
    }
    /* adds the ending of the new file name */
    strcat(new_file_name, ending);
    return new_file_name;
}

int copy_file(char *file_name_dest, char *file_name_orig) {
    char str[MAX_LINE_LENGTH];
    FILE *fp, *fp_dest;
    fp = fopen(file_name_orig, "r");
    if (fp == NULL) {
        print_internal_error(ERROR_CODE_8);
        return 0;
    }
    fp_dest = fopen(file_name_dest, "w");
    if (fp_dest == NULL) {
        print_internal_error(ERROR_CODE_7);
        fclose(fp);
        return 0;
    }
    while (fgets(str, MAX_LINE_LENGTH, fp) != NULL) {
        fprintf(fp_dest, "%s", str);
    }
    fclose(fp);
    fclose(fp_dest);
    return 1;
}

void abrupt_close(int num_args, ...) {
    int i;
    char *str;
    FILE *fp;
    va_list args;
    va_start(args, num_args);
    for (i = 0; i < num_args; i++) {
        /* next argument is a string whose allocated memory needs to be freed*/
        if (strcmp(va_arg(args, char*), "%s") == 0) {
            i++;
            str = va_arg(args, char*);
            remove(str);
            free(str);
        }
            /* next argument is a file pointer that needs to be closed */
        else {
            fp = va_arg(args, FILE*);
            fclose(fp);
        }
    }
    va_end(args);
}

