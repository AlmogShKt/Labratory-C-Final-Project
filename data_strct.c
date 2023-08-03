#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_strct.h"
#include "util.h"
#include "Errors.h"

node *make_node(char *name, char *content, int line_num){
    node *temp;

    /* Check if memory allocation for the node succeeded */
    temp = handle_malloc(sizeof(node));

    temp->name = name;        /* Set the name of the node */
    temp->content = content;  /* Set the content string of the node */
    temp->line = line_num;    /* Set the line number associated with the content */
    temp->next = NULL;        /* Initialize the next pointer to NULL */

    return temp;  /* Return a pointer to the newly created node */
}

node *search_list(node *head, char *name, int *found){
    *found = 0;

    /* If the list is empty */
    if (head == NULL) {
        return NULL;
    }

    /* If the node exists already */
    if (strcmp(name, head->name) == 0) {
        *found = 1;
        printf("Node %s already exists in the list\n", name);
        return head;
    }

    /* If the end of the list is reached */
    if (head->next == NULL) {
        return head;
    }

    /* Recursively search the rest of the list */
    return search_list(head->next, name, found);
}

void add_to_list(node **head, char *name, char *content, int line_num){
    int found;
    node *new_node, *temp;
    found = 0;

    /* Temp is the immediate parent of the new node in the list
     * or if the macro name already exists in the list, temp is the mcro with the same name.
     * */
    temp = search_list(*head,name,&found);

    /* If the list already has a macro with the same name */
    if(found && strcmp(temp->content,content) != 0){
        /* The content of the same node name is not the same - skipping this macro definition */
        print_internal_error(ERROR_CODE_13);
        free(name);
        free(content);
        return;
    }

    /* If the macro with the same name is not found in the list */
    if(!found){
        new_node = make_node(name,content,line_num);

        /* If the list is empty, add the new node to the head of the list */
        if(temp == NULL){
            *head = new_node;
        }

        /* If the list is not empty, add the new node down the list */
        else{
            temp->next = new_node;
        }
    }
}

void free_node(node *node1){
    /* Free memory allocated for the name, content and node */
    free(node1->name);
    free(node1->content);
    free(node1);
}

void free_list(node *head){
    /* Free memory for the current node and its contents while storing the current node in a temporary pointer */
    while(head != NULL) {
        node *temp = head;
        head = head->next;
        free_node(temp);
    }
}