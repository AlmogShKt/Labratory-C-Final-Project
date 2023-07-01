/**
 * This function removes all extra unnecessary white spaces from the file
 * @param file_name string of the input file name
 * @return string the name of the new file after white spaces removed
 */
char *remove_extra_spaces_file(char file_name[]);
char *copy_text(FILE *fp, fpos_t *pos, int length);

/**
 * This function removes extra unnecessary white-spaces from a string
 * @param str string to change
 */
void remove_extra_spaces_str(char str[]);

/**
 * This function checks of a character is a ' ' or '\t'
 * @param c character to check
 * @return 1 if the character is either ' ' or '\t', 0 if not
 */
int is_space_or_tab(char c);
void remove_spaces_next_to_comma(char *str);
