#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_strct.h"

void make_node(node **new_node, char *name, char *content){
    node *temp;
    temp = malloc(sizeof(node));
    if(temp == NULL){
        printf("make_node malloc failed\n");
        return;
    }
    temp->name = malloc(SIZE * sizeof(char));
    if(temp->name == NULL){
        printf("make_node name malloc failed\n");
        free(temp);
        return;
    }
    strcpy(temp->name,name);
    temp->content = malloc(SIZE * sizeof(char));
    if(temp->content == NULL){
        printf("make_node content malloc failed\n");
        free(temp->name);
        free(temp);
        return;
    }
    temp->right_child = NULL;
    temp->left_child = NULL;
    strcpy(temp->content,content);
    *new_node = temp;
}

node *search_tree(node *head, char *name, char *content, int *error){
    /* reached the end of the tree and not found, or an empty tree */
    if(head == NULL){
        return NULL;
    }
    /* node exists already */
    if(strcmp(name,head->name) == 0){
        /* the content of the same node name is not the same */
        if(strcmp(content,head->content) != 0) {
            printf("Macro has more than one definition\n");
            *error = 1;
            return NULL;
        }
        return head;
    }
    else if(strcmp(name,head->name)>0){
        return search_tree(head->right_child,name, content);
    }
    else {
        return search_tree(head->left_child,name, content);
    }
}

void add_to_tree(node **head, char *name, char *content){
    int error;
    node *new_node;
    error = 0;
    if(search_tree(*head,name,content,&error) == NULL && error == 0){
        make_node(&new_node,name,content);
        if(new_node == NULL){
            return;
        }
    }
    /* reached the end of the tree or an empty tree */
    if((*head) == NULL){
        *head = new_node;
    }
    /* node exists already */
    if(strcmp(new_node->name,(*head)->name) == 0){
        /* the content of the same node name is not the same */
        if(strcmp(new_node->content,(*head)->content) != 0) {
            printf("Macro has more than one definition\n");
        }
        return;
    }
    else if(strcmp(new_node->name,(*head)->name)>0){
        add_to_tree(&((*head)->right_child),new_node);
    }
    else {
        add_to_tree(&((*head)->left_child),new_node);
    }
}
