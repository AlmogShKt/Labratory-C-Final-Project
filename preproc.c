// #ifdef aaa

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preproc.h"
#include "util.h"
#include "globals.h"
#include "handle_text.h"
#include "Errors.h"
#include "lexer.h"

char *save_mcro_content(FILE *fp, fpos_t *pos, int *line_count) {
    int mcro_length;
    char *mcro;
    char str[MAX_LINE_LENGTH];
    if (fsetpos(fp, pos) != 0) {
        print_internal_error(ERROR_CODE_11);
        return NULL;
    }
    mcro_length = 0;
    str[0] = '\0';
    while (fgets(str, MAX_LINE_LENGTH, fp) && (strcmp(str, "endmcro \n")) != 0) {
        (*line_count)++;
        if (strcmp(str, "endmcro\n") != 0) {
            mcro_length += strlen(str);
        }
    }
    if (strcmp(str, "endmcro \n") != 0) {
        print_internal_error(ERROR_CODE_12);
        return NULL;
    }
    mcro = copy_text(fp, pos, mcro_length);
    return mcro;
}

int valid_mcro_name(char *str) {

}

int valid_mcro_decl(char *str, char **name, int line_count, char *file_name) {
    /* assumes "mcro " has been encountered right before the function was called */
    char *temp_name, *extra;
    temp_name = strtok(NULL, " \n");
    if (temp_name == NULL) {
        print_internal_error(ERROR_CODE_9);
        return 0;
    }
    if (is_instr(temp_name) || what_opcode(temp_name) >= 0 || \
        (*temp_name == '@' && what_reg(temp_name + 1) >= 0)) {
        location as_file;
        as_file.file_name = file_name;
        as_file.line_num = line_count;
        print_external_error(ERROR_CODE_17, as_file);
        return 0;
    }
    extra = strtok(NULL, "\n");
    if (extra != NULL) {
        print_internal_error(ERROR_CODE_10);
        return 0;
    }
    *name = handle_malloc((strlen(temp_name) + 1) * sizeof(char));
    strcpy(*name, temp_name);
    return 1;
}

void add_mcros(char *file_name, node **head) {
    int line_count;
    FILE *fp;
    char str[MAX_LINE_LENGTH];
    char *name, *content;
    fpos_t pos;
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        print_internal_error(ERROR_CODE_8);
        return;
    }
    line_count = 0;
    while (fgets(str, MAX_LINE_LENGTH, fp)) {
        line_count++;
        if (strcmp(strtok(str, " "), "mcro") == 0) {
            int mcro_line = line_count;
            if (!valid_mcro_decl(str, &name, line_count, file_name)) {
                continue;
            }
            fgetpos(fp, &pos);
            content = save_mcro_content(fp, &pos, &line_count);
            if (content == NULL) {
                continue;
            }
            /* going to the end of the macro */
            fsetpos(fp, &pos);
            /* adding the new mcro into the mcro_list */
            add_to_list(head, name, content, mcro_line);
        }
    }
    fclose(fp);
}

char *remove_mcros_decl(char file_name[]) {
    char *token, *new_file;
    char str[MAX_LINE_LENGTH];
    char str_copy[MAX_LINE_LENGTH];
    FILE *fp, *fp_out;
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        print_internal_error(ERROR_CODE_8);
        return NULL;
    }
    new_file = add_new_file(file_name, ".t02");
    fp_out = fopen(new_file, "w");
    if (fp_out == NULL) {
        print_internal_error(ERROR_CODE_7);
        abrupt_close(4, "file", fp, "%s", new_file);
        return NULL;
    }
    while (fgets(str, MAX_LINE_LENGTH, fp)) {
        strcpy(str_copy, str);   /* copying the line so we can manipulate one copy */
        token = strtok(str, " \n");
        /* blank line */
        if (token == NULL) {
            fprintf(fp_out, "\n");
            continue;
        }
        /* mcro was found */
        if (strcmp(token, "mcro") == 0) {
            while (strcmp(token, "endmcro") != 0) {
                fprintf(fp_out, "\n");
                fgets(str, MAX_LINE_LENGTH, fp);
                token = strtok(str, " \n");
                /* blank lines */
                while (token == NULL) {
                    fprintf(fp_out, "\n");
                    fgets(str, MAX_LINE_LENGTH, fp);
                    token = strtok(str, " \n");
                }
            }
            fprintf(fp_out, "\n");
        } else {
            fprintf(fp_out, "%s", str_copy);
        }
    }
    fclose(fp);
    fclose(fp_out);
    return new_file;
}

char *replace_mcro(char *str, node *mcro) {
    char *pos, *new_str;
    char str_beg[MAX_LINE_LENGTH];
    char str_end[MAX_LINE_LENGTH];
    strcpy(str_beg, str);
    pos = strstr(str_beg, mcro->name);
    *pos = '\0';
    strcpy(str_end, pos + strlen(mcro->name));
    new_str = handle_malloc((strlen(str_beg) + strlen(mcro->content) + strlen(str_end) + 1) * sizeof(char));
    if (new_str == NULL) {
        return NULL;
    }
    strcpy(new_str, str_beg);
    strcat(new_str, mcro->content);
    strcat(new_str, str_end);
    return new_str;
}

char *replace_all_mcros(char file_name[], node *head) {
    node *mcro;
    char *pos, *new_str, *file_temp, *final_file_name;
    char str[MAX_LINE_LENGTH];
    FILE *fp_temp, *fp_final;
    file_temp = add_new_file(file_name, ".tmp");
    final_file_name = add_new_file(file_name, ".am");
    if (!copy_file(file_temp, file_name) || \
        !copy_file(final_file_name, file_name)) {
        /* copying file failed - closing open files and freeing allocated memory */
        print_internal_error(ERROR_CODE_14);
        abrupt_close(4, "%s", file_temp, "%s", final_file_name);
        return NULL;
    }
    mcro = head;
    while (mcro != NULL) {
        fp_temp = fopen(file_temp, "r");
        if (fp_temp == NULL) {
            print_internal_error(ERROR_CODE_8);
            abrupt_close(4, "%s", file_temp, "%s", final_file_name);
            return NULL;
        }
        fp_final = fopen(final_file_name, "w");
        if (fp_final == NULL) {
            print_internal_error(ERROR_CODE_7);
            abrupt_close(6, "file", fp_temp, "%s", file_temp, "%s", final_file_name);
            return NULL;
        }
        while (fgets(str, MAX_LINE_LENGTH, fp_temp)) {
            pos = strstr(str, mcro->name);
            if (pos != NULL) {
                /* remove the '\n' at the end str */
                *(str + strlen(str) - 1) = '\0';
                new_str = replace_mcro(str, mcro);
                if (new_str == NULL) {
                    abrupt_close(8, "file", fp_final, "file", fp_temp, "%s", file_temp, "%s", final_file_name);
                    return NULL;
                }
                fprintf(fp_final, "%s", new_str);
                free(new_str);
            } else {
                fprintf(fp_final, "%s", str);
            }
        }
        fclose(fp_temp);
        fclose(fp_final);
        mcro = mcro->next;
        if (mcro == NULL) {
            break;
        }
        remove(file_temp);
        rename(final_file_name, file_temp);
    }
    remove(file_temp);
    free(file_temp);
    return final_file_name;
}

int mcro_call_before_decl(char file_name[], node *head) {
    FILE *fp;
    int line_count, check;
    node *mcro;
    char str[MAX_LINE_LENGTH];
    fp = fopen(file_name, "r");
    check = 0;
    line_count = 0;
    while (fgets(str, MAX_LINE_LENGTH, fp) != NULL) {
        line_count++;
        /* if line is a mcro declaration then go to next line */
        if (strstr(str, "mcro") != NULL) {
            continue;
        }
        mcro = head;
        while ((mcro != NULL) && (mcro->line < line_count)) {
            mcro = mcro->next;
        }
        if (mcro == NULL) {
            continue;
        }
        while (mcro != NULL) {
            if (strstr(str, mcro->name) != NULL) {
                print_internal_error(ERROR_CODE_16);
                printf("Macro %s call in line %d", mcro->name, line_count);
                check = 1;
            }
            mcro = mcro->next;
        }
    }
    fclose(fp);
    return check;
}

int mcro_exec(char file_name[]) {
    node *head;
    char *new_file1, *new_file2, *final_file;
    /* removing unnecessary white spaces in the input file and saving the result in a new temp file */
    new_file1 = remove_extra_spaces_file(file_name);
    if (new_file1 == NULL) {
        return 0;
    }
    head = NULL;
    /* scanning and saving all the macros in the input file in a linked list of macros */
    add_mcros(new_file1, &head);
    /* ensures that there is no macro call before it's declaration in the input file */
    if (mcro_call_before_decl(new_file1, head)) {
        free_list(head);
        abrupt_close(2, "%s", new_file1);
        return 0;
    }
    /* removing the declaration of the macros from the input file and saving the result in a new temp file */
    new_file2 = remove_mcros_decl(new_file1);
    if (new_file2 == NULL) {
        free_list(head);
        abrupt_close(2, "%s", new_file1);
        print_internal_error(ERROR_CODE_15);
        return 0;
    }
    free(new_file1);    /* frees allocated memory for the name of the first temp file */
    /* replaces all macro calls with their definitions as saved in the linked list
     * and saving the result in a new file */
    final_file = replace_all_mcros(new_file2, head);
    if (final_file == NULL) {
        free_list(head);
        abrupt_close(4, "%s", new_file2);
        print_internal_error(ERROR_CODE_15);
        return 0;
    }
    /* freeing allocated memory for the strings of the new file names */
    free(new_file2);
    free(final_file);
    free_list(head);    /* freeing themacros  linked list */
    /* removing the temp files */
    remove(add_new_file(file_name, ".t01"));
    remove(add_new_file(file_name, ".t02"));
    /* printf("Macros expansion in file %s completed successfully\n",file_name); */
    return 1;
}

int is_valid_end_macro(char *str) {
    return strcmp(str, "endmcro\n") != 0 || strcmp(str, "endmcro \n") != 0;
}

// #endif