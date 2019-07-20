#include "assembler.h"

Bool DTaddData(DTptr * head, char * name, int value) {
    DTptr p1, t;

    t = ( DTptr ) malloc(sizeof(DTnode));

    if( !t ){
        /* TODO: PRINT ERROR */
        return FALSE;
    }

    t->name = malloc(strlen(name) + 1);
    strcpy( t->name, name );
    t->value = value;
    t->next = NULL;

    /* if list is empty, assign this node as head */
    if( ( *head ) == NULL ){
        *head = t;
        return TRUE;
    }

    p1 = *head;

    while( p1->next != NULL ){
        p1 = p1->next;
    }

    p1->next = t;
    
    return TRUE;

}

void printDT( DTptr head ){
    DTptr p1 = head;

    while ( p1 != NULL ){
        printf(" %s -> %d \n", p1->name, p1->value );
        p1 = p1->next;
    }

}