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

char * getIndexFromToken( char *arrToken, char *result ){
    char * token = malloc(strlen(arrToken) + 1);
    int i = 0, j = 0;
    
    while( arrToken[i] != '[' ){
        i++;
    }

    i++;

    while( arrToken[i] != ']' ){
        token[j] = arrToken[i];
        i++;
        j++;
    }

    token[i] = '\0';

    strcpy( result, token );
    free(token);
    
    return result;
}

/* extract label from array tokens, ex: LIST[sz] => LIST */

char * getLabelFromToken( char *arrToken, char*result ){
    char * token = malloc(strlen(arrToken) + 1);
    int i = 0;
    
    while( arrToken[i] != '[' ){
        token[i] = arrToken[i];
        i++;
    }

    token[i] = '\0';
    
    strcpy( result, token );
    free(token);
    
    return result;
}

/* print two's complement */

int getTwosComplement(int num) { 
    unsigned long mask = 1; /* create mask */
    Bool flag = FALSE;
    int i;

    for (i = 0; i < WORD_SIZE; i++) { 
        if( flag ){
            num = (num^mask); /* flip bit */
        }

        if( !flag && ((num&mask) != 0) ){
            flag = TRUE;
        }

        mask <<= 1;
    } 
    
    return num;
} 

void print_short_bin(unsigned short int x){
    unsigned int mask = 1 << ( WORD_SIZE - 1 );

    while( mask ){
        if( (x&mask) == 0){
            printf("0");
        } else{
            printf("1");
        }
        mask >>= 1;
    }
}

char * getBase4( word line ){
    unsigned int mask01 = 1; /* 1 in binary == ..00001 */
    unsigned int mask10 = 2; /* 2 in binary == ..00010 */
    char * result = malloc(WORD_SIZE/2 - 1); /* every 2 bits are 1, minus one because array index starts at 0 */
    int i = WORD_SIZE/2 - 1;
    
    if(!result) {
        printf("error allocated memeory"); /* DELETe */
        exit(1);
    }

    while( mask01 && mask10 && i >= 0 ){
        if( (line.print&mask10) == 0 && (line.print&mask01) == 0 ){
            /* 00 */
            result[i] = '*';
        } else if( (line.print&mask10) == 0 && (line.print&mask01) != 0 ){
            /* 01 */
            result[i] = '#';
        } else if( (line.print&mask10) != 0 && (line.print&mask01) == 0 ){
            /* 10 */
            result[i] = '%';
        } else if( (line.print&mask10) != 0 && (line.print&mask01) != 0 ){
            /* 11 */
            result[i] = '!';
        } else{
            result[i] = 'X';
        }
        mask01 <<= 2;
        mask10 <<= 2;
        i--;
    }

    result[WORD_SIZE/2] = '\0';

    return result;
}


/**
 * concat 2 strings
 */
char* concat(char *s1, char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}