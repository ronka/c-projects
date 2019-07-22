typedef struct node * DTptr;

typedef struct node {
    char *name;
    char *type;
    int value;
    DTptr next;
} DTnode;