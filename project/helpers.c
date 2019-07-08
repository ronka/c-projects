#include "assembler.h"

Bool isLabel( char* str ){
    if(str[strlen(str) - 1] == ':'){
        return TRUE;
    }
    return FALSE;
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