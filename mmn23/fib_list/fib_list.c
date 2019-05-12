#include "fib_list.h"

int main(){
    int size, i;
    ptr head = NULL, tail = NULL;

    printf("\n Enter the size of the Fibonacci sequence you want to build: ");
    scanf("%d", &size);

    for ( i=0 ; i < size ; i++ ){
        if ( i <= 1 ){
            add2list( &head, &tail, 1 );
        } else{
            add2list( &head, &tail,
                head->data + head->next->data );
        }
    }

    printlist( head, tail );

    printf("\n\n");

    return 0;
}

void printlist( ptr head, ptr tail ){
    ptr p1 = head;

    printf("\n The Fibonacci Sequence: \n");

    while ( p1 != NULL && p1 != tail ){
        printf(" %d ->", p1->data );
        p1 = p1->next;
    }

    printf(" %d", p1->data );
}

void add2list( ptr * hptr, ptr * tptr, int data ){
    ptr p1, p2, t;

    t = ( ptr ) malloc(sizeof(item));

    if( !t ){
        printf("\n cannot allocate memory \n");
        exit(0);
    }

    t->data = data;

    /* if list is empty, assign this node as head and tail */
    if( ! ( *hptr ) ){
        t->next = t;
        *hptr = t;
        *tptr = t;
        return;
    }

    p1 = *hptr;
    p2 = *tptr;
    *hptr = t;
    t->next = p1;
    
    p2->next = *hptr;

    return;
}