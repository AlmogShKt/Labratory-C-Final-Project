#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_strct.h"
#include "globals.h"

node *make_node(char *name, char *content){
    node *temp;
    temp = malloc(sizeof(node));
    if(temp == NULL){
        printf("make_node malloc failed\n");
        return NULL;
    }
    temp->name = malloc((strlen(name)+1) * sizeof(char));
    if(temp->name == NULL){
        printf("make_node name malloc failed\n");
        free(temp);
        return NULL;
    }
    strcpy(temp->name,name);
    temp->content = malloc((strlen(content)+1) * sizeof(char));
    if(temp->content == NULL){
        printf("make_node content malloc failed\n");
        free(temp->name);
        free(temp);
        return NULL;
    }
    strcpy(temp->content,content);
    // temp->right_child = NULL;
    // temp->left_child = NULL;
    temp->next = NULL;
    return temp;
}

node *search_list(node *head, char *name, int *found){
    int compare;
    *found = 0;
    /* tree is empty */
    if(head == NULL){
        return NULL;
    }
    /* node exists already */
    if((compare = strcmp(name,head->name)) == 0){
        *found = 1;
        printf("node %s already exists in the tree\n",name);
        return head;
    }
    /*
    else if(compare > 0){
        if(head->right_child == NULL){
            return head;
        }
        return search_tree(head->right_child,name, found);
    }
    else {
        if(head->left_child == NULL){
            return head;
        }
        return search_tree(head->left_child,name, found);
    }
    */
    if(head->next == NULL){
        return head;
    }
    return search_list(head->next,name,found);
}

void add_to_list(node **head, char *name, char *content){
    int error, found;
    node *new_node, *temp;
    error = found = 0;
    /* temp is the immediate parent of the new node in the tree */
    temp = search_list(*head,name,&found);
    if(found && strcmp(temp->content,content) != 0){
        /* the content of the same node name is not the same */
        printf("macro %s has more than one definition\n",name);
        exit(1);
    }
    if(!found){
        new_node = make_node(name,content);
        /* tree is empty */
        if(temp == NULL){
            *head = new_node;
            printf("new node %s was added to the head of the list\n",name);
        }
        /* list is not empty - the new node has a potential parent */
        else{
            temp->next = new_node;
            /*
            if(strcmp(name,temp->name) < 0){
                temp->left_child = new_node;
            }
            else {
                temp->right_child = new_node;
            }
            */
            printf("new node %s was added down the list\n",name);
        }
    }
}

void free_node(node *node1){
    free(node1->name);
    free(node1->content);
    free(node1);
}

void free_tree(node *head){
    /* todo - build function */
}