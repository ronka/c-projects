#include "assembler.h"

Bool isLabel( char* str ){
    if(str[strlen(str) - 1] == ':'){
        return TRUE;
    }
    return FALSE;
}

int isOp( char* str ){
    int i;

    Opcode opcodes[] = {
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

Bool isValidMacro( char *str ){
   if( !strcmp( str, MACRO ) ){
        return TRUE;
    }

    return FALSE;
}

int isInstruction( char *str, Instruction insts[] ){
    int i;

    for (i=0 ; i < INSTRUCTIONS_LENGTH; i++){
        if( !strcmp( str, insts[i].name ) ){
            return i;
        }
    }

    return -1;
}

/**
 * Check if its a register name
 */
Bool isRegister( char *reg ) {
	return ( (reg[0] == 'r'
        && reg[1] >= '0'
        && reg[1] <= '7'
		&& reg[2] == '\0') ? TRUE : FALSE );
}