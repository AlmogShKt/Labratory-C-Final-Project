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

    /* Set the file pointer to the provided position */
    if (fsetpos(fp, pos) != 0) {
        print_internal_error(ERROR_CODE_11);
        return NULL;
    }
    mcro_length = 0;
    str[0] = '\0';

    /* Read lines from the file until "endmcro" is encountered */
    while (fgets(str, MAX_LINE_LENGTH, fp) && (strcmp(str, "endmcro\n")) != 0) {
        /* checking for a line with endmcro and extra text */
        if ((strstr(str, "endmcro") != NULL) && strlen(str) != strlen("endmcro")) {
            print_internal_error(ERROR_CODE_12);
            return NULL;
        }
        (*line_count)++;
        if (strcmp(str, "endmcro\n") != 0) {
            mcro_length += strlen(str);
        }
    }

    /* Copy the macro content into a dynamically allocated string */
    mcro = copy_text(fp, pos, mcro_length);
    return mcro;
}

int valid_mcro_decl(char *str, char **name, int line_count, char *file_name) {
    /* assumes "mcro " has been encountered right before the function was called */
    char *temp_name, *extra;

    temp_name = strtok(NULL, " \n");
    if (temp_name == NULL) {
        print_internal_error(ERROR_CODE_9);
        return 0;
    }

    /* Check if the macro name clashes with instruction name, opcode, or register name */
    if (is_instr(temp_name) || what_opcode(temp_name) >= 0 || what_reg(temp_name) >= 0) {
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

    /* Copy the extracted macro name into a dynamically allocated string */
    *name = handle_malloc((strlen(temp_name) + 1) * sizeof(char));
    strcpy(*name, temp_name);

    return 1;
}

int add_mcros(char *file_name, node **head) {
    int line_count, success;
    FILE *fp;
    char str[MAX_LINE_LENGTH];
    char *name, *content;
    fpos_t pos;

    /*Lets assume that the process success */
    success = 1;

    /*Open the file and handle it*/
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        print_internal_error(ERROR_CODE_8);
        success = 0;
        return success;
    }
    line_count = 0;
    while (fgets(str, MAX_LINE_LENGTH, fp)) {
        line_count++;

        /* Extract the macro name from the declaration line and validate it */
        if (strcmp(strtok(str, " "), "mcro") == 0) {
            int mcro_line = line_count;
            if (!valid_mcro_decl(str, &name, line_count, file_name)) {
                success = 0;
                continue;
            }

            /* Save the macro content starting from the current file position */
            fgetpos(fp, &pos);
            content = save_mcro_content(fp, &pos, &line_count);
            if (content == NULL) {
                success = 0;
                continue;
            }

            /* going to the end of the macro */
            fsetpos(fp, &pos);
            /* adding the new mcro into the mcro_list */
            add_to_list(head, name, content, mcro_line);
        }
    }

    /*Close ythe file and return the result*/
    fclose(fp);
    return success;
}

char *remove_mcros_decl(char file_name[]) {
    char *token, *new_file;
    char str[MAX_LINE_LENGTH];
    char str_copy[MAX_LINE_LENGTH];
    FILE *fp, *fp_out;

    /* Open the input file for reading */
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        print_internal_error(ERROR_CODE_8);
        return NULL;
    }

    /* Create a new filename with the ".t02" extension */
    new_file = add_new_file(file_name, ".t02");

    /* Open the new file for writing */
    fp_out = fopen(new_file, "w");
    if (fp_out == NULL) {
        print_internal_error(ERROR_CODE_7);
        abrupt_close(4, "file", fp, "%s", new_file);
        return NULL;
    }

    /* Process the input file line by line */
    while (fgets(str, MAX_LINE_LENGTH, fp)) {
        /* copying the line so we can manipulate one copy */
        strcpy(str_copy, str);
        token = strtok(str, " \n");

        /* blank line */
        if (token == NULL) {
            fprintf(fp_out, "\n");
            continue;
        }

        /* mcro was found */
        if (strcmp(token, "mcro") == 0) {
            /* Skip lines until the "endmcro" marker is found */
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
            /* Write the line to the new file if it's not part of a macro declaration */
            fprintf(fp_out, "%s", str_copy);
        }
    }
    /*Close file ptr*/
    fclose(fp);
    fclose(fp_out);

    /* Return the name of the new file without macros */
    return new_file;
}

char *replace_mcro(char *str, node *mcro) {
    char *pos, *new_str;
    char str_beg[MAX_LINE_LENGTH];
    char str_end[MAX_LINE_LENGTH];

    /* Make a copy of the input string to manipulate */
    strcpy(str_beg, str);

    /* Find the first occurrence of the macro name in the string */
    pos = strstr(str_beg, mcro->name);
    *pos = '\0';

    /* Copy the part of the string after the macro name */
    strcpy(str_end, pos + strlen(mcro->name));

    /* Allocate memory for the new string with the macro content replaced */
    new_str = handle_malloc((strlen(str_beg) + strlen(mcro->content) + strlen(str_end) + 1) * sizeof(char));
    if (new_str == NULL) {
        return NULL;
    }

    /* Construct the new string by concatenating the parts */
    strcpy(new_str, str_beg);
    strcat(new_str, mcro->content);
    strcat(new_str, str_end);

    /*Return the result */
    return new_str;
}

char *replace_all_mcros(char file_name[], node *head) {
    node *mcro;
    char *pos, *new_str, *file_temp, *final_file_name;
    char str[MAX_LINE_LENGTH];
    FILE *fp_temp, *fp_final;

    /* Create a temporary filename with the ".tmp" extension */
    file_temp = add_new_file(file_name, ".tmp");

    /* Create the final modified filename with the ".am" extension */
    final_file_name = add_new_file(file_name, ".am");

    /* Copy the original file to both the temporary file and the final modified file */
    if (!copy_file(file_temp, file_name) || \
        !copy_file(final_file_name, file_name)) {
        /* Copying file failed - closing open files and freeing allocated memory */
        print_internal_error(ERROR_CODE_14);
        abrupt_close(4, "%s", file_temp, "%s", final_file_name);
        return NULL;
    }

    mcro = head;
    while (mcro != NULL) {
        /* Process each macro in the macro list */
        fp_temp = fopen(file_temp, "r");
        if (fp_temp == NULL) {
            print_internal_error(ERROR_CODE_8);
            abrupt_close(4, "%s", file_temp, "%s", final_file_name);
            return NULL;
        }

        /* Open the final modified file for writing */
        fp_final = fopen(final_file_name, "w");
        if (fp_final == NULL) {
            print_internal_error(ERROR_CODE_7);
            abrupt_close(6, "file", fp_temp, "%s", file_temp, "%s", final_file_name);
            return NULL;
        }

        /* Process each line of the temporary file */
        while (fgets(str, MAX_LINE_LENGTH, fp_temp)) {
            pos = strstr(str, mcro->name);
            /*If we found relevant part that need to be modify*/
            if (pos != NULL) {
                /* remove the '\n' at the end str */
                *(str + strlen(str) - 1) = '\0';

                /* Replace the macro name with its content */
                new_str = replace_mcro(str, mcro);
                if (new_str == NULL) {
                    abrupt_close(8, "file", fp_final, "file", fp_temp, "%s", file_temp, "%s", final_file_name);
                    return NULL;
                }

                /* Write the modified line to the final modified file */
                fprintf(fp_final, "%s", new_str);
                free(new_str);
            } else {
                /* Write the original line to the final modified file */
                fprintf(fp_final, "%s", str);
            }
        }

        /* Close the temporary and final modified files */
        fclose(fp_temp);
        fclose(fp_final);

        /* Move to the next macro in the list */
        mcro = mcro->next;
        if (mcro == NULL) {
            break;
        }

        /* Replace the temporary file with the final modified file for the next iteration */
        remove(file_temp);
        rename(final_file_name, file_temp);
    }

    /* Remove the temporary file and free the allocated memory for the filename */
    remove(file_temp);
    free(file_temp);

    /* Return the name of the final modified file */
    return final_file_name;
}

int mcro_call_before_decl(char file_name[], node *head) {
    FILE *fp;
    int line_count, check;
    node *mcro;
    char str[MAX_LINE_LENGTH];

    /* Open the input file for reading */
    fp = fopen(file_name, "r");
    check = 0;
    line_count = 0;
    while (fgets(str, MAX_LINE_LENGTH, fp) != NULL) {
        line_count++;
        /* if line is a mcro declaration then go to next line */
        if (strstr(str, "mcro") != NULL) {
            continue;
        }

        /* Search for the first macro declaration after the current line */
        mcro = head;
        while ((mcro != NULL) && (mcro->line < line_count)) {
            mcro = mcro->next;
        }

        /* If there are no more macro declarations, continue to the next line */
        if (mcro == NULL) {
            continue;
        }

        /* Check for macro calls in the current line */
        while (mcro != NULL) {
            if (strstr(str, mcro->name) != NULL) {
                /* A macro call is found before its declaration */
                print_internal_error(ERROR_CODE_16);
                check = 1;

                /*!printf("Macro %s call in line %d", mcro->name, line_count);*/
            }
            mcro = mcro->next;
        }
    }
    fclose(fp);

    /* Return 1 if any macro call is found before its declaration, otherwise return 0 */
    return check;
}


int mcro_exec(char file_name[]) {
    node *head;
    char *new_file1, *new_file2, *final_file, *temp_file_name1, *temp_file_name2;

    /* Step 1: Removing unnecessary white spaces in the input file and saving the result in a new temp file */
    new_file1 = remove_extra_spaces_file(file_name);
    if (new_file1 == NULL) {
        return 0;
    }

    /* Initialize the macro linked list */
    head = NULL;

    /* Step 2: Scanning and saving all the macros in the input file in a linked list of macros */
    if (!add_mcros(new_file1, &head)) {
        free_list(head);
        abrupt_close(2, "%s", new_file1);
        return 0;
    }

    /* Step 3: Ensuring that there is no macro call before its declaration in the input file */
    if (mcro_call_before_decl(new_file1, head)) {
        free_list(head);
        abrupt_close(2, "%s", new_file1);
        return 0;
    }

    /* Step 4: Removing the declaration of the macros from the input file and saving the result in a new temp file */
    new_file2 = remove_mcros_decl(new_file1);
    if (new_file2 == NULL) {
        free_list(head);
        abrupt_close(2, "%s", new_file1);
        print_internal_error(ERROR_CODE_15);
        return 0;
    }

    /* Free allocated memory for the name of the first temp file*/
    free(new_file1);

    /* Step 5: Replacing all macro calls with their definitions as saved in the linked list and saving the result in a new file */
    final_file = replace_all_mcros(new_file2, head);
    if (final_file == NULL) {
        free_list(head);
        abrupt_close(4, "%s", new_file2);
        print_internal_error(ERROR_CODE_15);
        return 0;
    }

    temp_file_name1 = add_new_file(file_name, ".t01");
    temp_file_name2 = add_new_file(file_name, ".t02");
    remove(temp_file_name1);
    remove(temp_file_name2);

    free(temp_file_name1);
    free(temp_file_name2);


    /* Step 6: Freeing allocated memory for the strings of the new file names */
    free(new_file2);
    free(final_file);
    free_list(head);

    return 1;
}