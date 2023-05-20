typedef struct node{
    char *name;
    char *content;
    struct node *left_child;
    struct node *right_child;
}node;

#define MAX_LINE_LENGTH 80

void make_node(node **new_node, char *name, char *content);
node *search_tree(node *head, char *name, char *content, int *error, int *found);
void add_to_tree(node **head, char *name, char *content);

