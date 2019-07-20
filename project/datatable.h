typedef struct node * DTptr;

typedef struct node {
    char *name;
    int value;
    DTptr next;
} DTnode;