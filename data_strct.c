#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_strct.h"
#include "util.h"
#include "Errors.h"

node *make_node(char *name, char *content, int line_num){
    node *temp;
    temp = handle_malloc(sizeof(node));
    temp->name = name;
    temp->content = content;
    temp->line = line_num;
    temp->next = NULL;
    return temp;
}

node *search_list(node *head, char *name, int *found){
    *found = 0;
    /* list is empty */
    if(head == NULL){
        return NULL;
    }
    /* node exists already */
    if(strcmp(name,head->name) == 0){
        *found = 1;
        printf("node %s already exists in the tree\n",name);
        return head;
    }
    if(head->next == NULL){
        return head;
    }
    return search_list(head->next,name,found);
}

void add_to_list(node **head, char *name, char *content, int line_num){
    int found;
    node *new_node, *temp;
    found = 0;
    /* temp is the immediate parent of the new node in the list
     * or if the mcro name already exists in the list, temp is the mcro with the same name.
     * */
    temp = search_list(*head,name,&found);
    /* the list has already a mcro with the same name */
    if(found && strcmp(temp->content,content) != 0){
        /* the content of the same node name is not the same - skipping this mcro definition */
        print_internal_error(ERROR_CODE_13);
        free(name);
        free(content);
        return;
    }
    if(!found){
        new_node = make_node(name,content,line_num);
        /* list is empty */
        if(temp == NULL){
            /* new node was added to the head of the list */
            *head = new_node;
        }
        /* list is not empty - the new node has a potential parent */
        else{
            /* new node %s was added down the list */
            temp->next = new_node;
        }
    }
}

void free_node(node *node1){
    free(node1->name);
    free(node1->content);
    free(node1);
}

void free_list(node *head){
    while(head != NULL) {
        node *temp = head;
        head = head->next;
        free_node(temp);
    }
}