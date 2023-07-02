typedef struct node{
    char *name;
    char *content;
    int line;
    struct node *next;
}node;

/**
 * This function creates a node for a new macro saved from the source file
 * @param name a string to the name of the macro
 * @param content the content of the macro (excluding endmcro)
 * @param line_num the line number in the source file where the macro was defined
 * @return a new node of a macro
 */
node *make_node(char *name, char *content, int line_num);

/**
 * This function searchs for a macro name in a linked list with macros
 * @param head a pointer to head of a linked list where the macros were saved
 * @param name the string with the name of the macro we are looking for
 * @param found a pointer to a number that indicates if the macro was found or not
 * @return the node of the macro if it was found, NULL if not
 */
node *search_list(node *head, char *name, int *found);

/**
 * This function adds a new node of a macro to the end of a linked list
 * @param head a pointer to head of a linked list where the macros were saved
 * @param name a string with the name of the new macro
 * @param content a string with the content of the new macro
 * @param line_num the line number in the source file where the macro was defined
 */
void add_to_list(node **head, char *name, char *content, int line_num);

/**
 * This function deallocates memory from a node of a macro
 * @param node1 a pointer to the node to be deallocated
 */
void free_node(node *node1);

/**
 * This function deallocates memory from the entire lined list
 * @param head a pointer to head of a linked list where the macros were saved
 */
void free_list(node *head);

