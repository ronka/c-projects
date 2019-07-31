#include "assembler.h"

Bool isLabel( char* str ){
    if(str[strlen(str) - 1] == ':'){
        return TRUE;
    }
    return FALSE;
}

int isOp( char* str ){
    int i;

    char *opcodes[] = {
        "mov",
        "cmp",
        "add",
        "sub",
        "not",
        "cls",
        "lea",
        "inc",
        "dec",
        "jmp",
        "bne",
        "red",
        "prn",
        "jsr",
        "rts",
        "stop",
    };

    for (i=0 ; i < OPCODES_LENGTH; i++){
        if( !strcmp( str, opcodes[i] ) ){
            return i;
        }
    }

    return -1;
}

Bool isMacro( char *str ){
   if( !strcmp( str, DEFINE ) ){
        return TRUE;
    }

    return FALSE;
}

int isInstruction( char *str ){
    int i;

    Instruction insts[] = {
        {".data", 0},
        {".string", 1},
        {".extern", 2},
        {".entry", 3}
    };

    for (i=0 ; i < INSTRUCTIONS_LENGTH; i++){
        if( !strcmp( str, insts[i].name ) ){
            return i;
        }
    }

    return -1;
}

void removeSpaces(char* str) {
    char *end;

    /* remove spacesin the start of string */
    while(isspace((unsigned char)*str)){
        str++;
    }

    /* if empty string */
    if(*str == 0) {
        return;
    }

    /* remove strings at the end */
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)){
        end--;
    }
    
    end[1] = '\0';
}

/* Check if its a register name */

Bool isRegister( char *reg ) {
	return ( (reg[0] == 'r'
        && reg[1] >= '0'
        && reg[1] <= '7'
		&& reg[2] == '\0') ? TRUE : FALSE );
}

/**
 *  Arrays
 */

Bool isArray( char *arr ){
    int i;
    Bool flag = FALSE;

    for( i=0 ; i < strlen(arr) ; i++ ){
        if( arr[i] == '[' && !flag ){
            flag = TRUE;
        }

        if( flag && arr[i+1] == '\0' && arr[i] == ']' ){
            return TRUE;
        }
    }

    return FALSE;
}

/* extract index from array tokens, ex: LIST[sz] => sz */

char * getIndexFromToken( char *arrToken ){
    char * token;
    int i = 0;

    token = strtok(arrToken,"[");
    token = strtok(NULL,"]");

    return token;
}