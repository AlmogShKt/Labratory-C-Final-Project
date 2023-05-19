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

node *search_tree(node *head, char *name, char *content, int *error, int *found){
    /* node exists already */
    if(strcmp(name,head->name) == 0){
        /* the content of the same node name is not the same */
        if(strcmp(content,head->content) != 0) {
            printf("Macro has more than one definition\n");
            *error = 1;
            return NULL;
        }
        *found = 1;
        return head;
    }
    else if(strcmp(name,head->name)>0){
        /* reached a leaf in the right child of head*/
        if(head->right_child->right_child == NULL && head->right_child->left_child == NULL){
            return head;
        }
        return search_tree(head->right_child,name, content, error, found);
    }
    else {
        /* reached a leaf in the left child of head*/
        if(head->left_child->right_child == NULL && head->left_child->left_child == NULL){
            return head;
        }
        return search_tree(head->left_child,name, content, error, found);
    }
}

void add_to_tree(node **head, char *name, char *content){
    int error, found;
    node *new_node, *temp;
    error = found = 0;
    if((temp = search_tree(*head,name,content,&error,&found)) != NULL && error == 0 && found == 0){
        make_node(&new_node,name,content);
        if(strcmp(name,temp->name) < 0){
            temp->left_child = new_node;
            return;
        }
        else {
            temp->right_child = new_node;
        }
    }
}
