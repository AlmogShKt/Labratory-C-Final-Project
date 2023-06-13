typedef struct node{
    char *name;
    char *content;
    int line;
    struct node *next;
}node;

node *make_node(char *name, char *content, int line_num);
node *search_list(node *head, char *name, int *found);
void add_to_list(node **head, char *name, char *content, int line_num);
void free_node(node *node1);
void free_list(node *head);

