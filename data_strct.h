typedef struct node{
    char *name;
    char *content;
    // struct node *left_child;
    // struct node *right_child;
    struct node *next;
}node;

node *make_node(char *name, char *content);
node *search_list(node *head, char *name, int *found);
void add_to_list(node **head, char *name, char *content);
void free_node(node *node1);
void free_tree(node *head);

