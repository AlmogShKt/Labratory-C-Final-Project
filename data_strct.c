#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_strct.h"
#include "util.h"

node *make_node(char *name, char *content){
    node *temp;
    temp = handle_malloc(sizeof(node));
    temp->name = name;
    temp->content = content;
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

void add_to_list(node **head, char *name, char *content){
    int found;
    node *new_node, *temp;
    found = 0;
    /* temp is the immediate parent of the new node in the list */
    temp = search_list(*head,name,&found);
    if(found && strcmp(temp->content,content) != 0){
        /* the content of the same node name is not the same */
        printf("macro %s has more than one definition\n",name);
        exit(1);
    }
    if(!found){
        new_node = make_node(name,content);
        /* list is empty */
        if(temp == NULL){
            *head = new_node;
            printf("new node %s was added to the head of the list\n",name);
        }
        /* list is not empty - the new node has a potential parent */
        else{
            temp->next = new_node;
            printf("new node %s was added down the list\n",name);
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