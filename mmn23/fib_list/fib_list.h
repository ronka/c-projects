#include <stdio.h>
#include <stdlib.h>

typedef struct node * ptr;

typedef struct node {
    int data;
    ptr next;
} item;

void add2list(ptr *, ptr *, int);
void printlist(ptr, ptr);