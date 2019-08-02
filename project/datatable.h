/*
 *  Symbol Table
 */
typedef struct nodeST * STptr;

typedef struct nodeST {
    char *name;
    char *type;
    int value;
    STptr next;
} STnode;

/*
 * Data Table
 */
typedef struct nodeDT * DTptr;

typedef struct nodeDT {
    char *name;
    int value;
    DTptr next;
} DTnode;

/*
 * Machine Code Table
 */
typedef struct nodeMC * MCptr;

typedef struct nodeMC {
    word value;
    MCptr next;
} MCnode;