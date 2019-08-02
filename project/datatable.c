#include "assembler.h"

/**
 * Symbol table
 */

Bool STaddNode(STptr * head, char * name, char * type, int value) {
    STptr p1, t;

    t = ( STptr ) malloc(sizeof(STnode));

    if( !t ){
        /* TODO: PRINT ERROR */
        return FALSE;
    }

    t->name = malloc(strlen(name) + 1);
    t->type = malloc(strlen(type) + 1);
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

Bool isInST( STptr head, char * name, char * type ){
    STptr p1 = head;

    while ( p1 != NULL ){
        if( strcmp(p1->name,name) == 0 ){
            if( type[0] != '\0' ){
                if(strcmp(p1->type,type) == 0){
                    return TRUE;
                }
            }
            return TRUE;
        }
        p1 = p1->next;
    }

    return FALSE;
}

int getSTValue( STptr head, char * name, char * type ){
    STptr p1 = head;

    while ( p1 != NULL ){
        if( strcmp(p1->name,name) == 0 && strcmp(p1->type,type) == 0 ){
            return p1->value;
        }
        p1 = p1->next;
    }

    return 0;
}

void printST( STptr head ){
    STptr p1 = head;

    while ( p1 != NULL ){
        printf(" name: %s | type: %s | value: %d \n", p1->name, p1->type, p1->value );
        p1 = p1->next;
    }
}

/*
 * Data Table Stuff
 */

Bool DTaddNode(DTptr * head, char * name, int value) {
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

Bool isInDT( DTptr head, char * name ){
    DTptr p1 = head;

    while ( p1 != NULL ){
        if( strcmp(p1->name,name) == 0 ){
            return TRUE;
        }
        p1 = p1->next;
    }

    return FALSE;
}

void printDT( DTptr head ){
    DTptr p1 = head;

    while ( p1 != NULL ){
        printf(" name: %s | value: %d \n", p1->name, p1->value );
        p1 = p1->next;
    }

}

void updateDT( DTptr * head, char * symbol, int value ){
    DTptr p1;
    p1 = *head;

    while( p1 != NULL ){
        if( strcmp(p1->name, symbol) == 0 ){
            p1->value = value;
            break;
        }
        p1 = p1->next;
    }
}

/**
 * Machine Code Table
 */
Bool MCaddNode(MCptr * head, word value) {
    MCptr p1, t;

    t = ( MCptr ) malloc(sizeof(MCnode));

    if( !t ){
        return FALSE;
    }

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