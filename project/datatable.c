#include "assembler.h"

Bool DTaddData(DTptr * head, char * name, char * type, int value) {
    DTptr p1, t;

    t = ( DTptr ) malloc(sizeof(DTnode));

    if( !t ){
        /* TODO: PRINT ERROR */
        return FALSE;
    }

    t->name = malloc(strlen(name) + 1);
    t->type = malloc(strlen(type) + 1);
    if( !t->name || !t->type ){
        return FALSE;
    }
    strcpy( t->name, name );
    strcpy( t->type, type );
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

Bool isInDT( DTptr head, char * name, char * type ){
    DTptr p1 = head;

    while ( p1 != NULL ){
        if( strcmp(p1->name,name) == 0 ){
            /*if( type[0] != '\0' ){
                if(strcmp(p1->type,type) == 0){
                    return TRUE;
                }
            }*/
            return TRUE;
        }
        p1 = p1->next;
    }

    return FALSE;
}

void printDT( DTptr head ){
    DTptr p1 = head;

    while ( p1 != NULL ){
        /* printf(" name: %s | type: %s | value: %d \n", p1->name, p1->type, p1->value );*/
        printf(" name: %s | value: %d \n", p1->name, p1->value );
        p1 = p1->next;
    }

}