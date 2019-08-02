#include "assembler.h"

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


Bool secondRun(FILE *sourceFile, STptr *SymbolTable, DTptr *extFile, DTptr *entFile){
    char line[MAX_LINE], tempStr[MAX_LINE];
    char *token;
    int op, inst; /* var used to save operation and instraction */
    int DC = 0, IC = 0; /* memory track */
    MCptr machineCode = NULL;
    word machineLine;

    DC = DC + MEMORY_START;

    while (fgets(line, MAX_LINE, sourceFile)) {
        newMachineLine( &machineLine );
        strncpy ( tempStr, line, MAX_LINE );
        
        /*
         * If comment, continue to next line
         */

        if( *line == ';' || strlen(line) == 1 ){
            continue;
        }

        token = strtok(tempStr," ");

        removeSpaces(token);

        if( isMacro( token ) ){ /* if its macro, skip */
            continue;
        } else if( isLabel( token ) ){ /* if its label, skip */
            token[strlen(token) - 1] = '\0'; /* clean delim (:) */
            if( isInDT(*entFile, token) ) {
                updateDT(entFile, token, DC);
            }
            token = strtok(NULL," ");
        }

        if( (inst = isInstruction( token )) > -1 ){
            switch (inst)
            {
            case INST_DATA: /* .data */
                token = strtok(NULL,",");
                
                do{
                    removeSpaces(token);
                    strcpy(tempStr, token);

                    if( strtol(tempStr, NULL, 10) != 0 ){
                        /* if a regular number */
                        machineLine.print = strtol(tempStr, NULL, 10);
                    } else if( isInST(*SymbolTable, token , MACRO) ){
                        /* if macro */
                        machineLine.print = getSTValue( *SymbolTable, token, MACRO );
                    } else{
                        printf("error parsing data, -%s - %d\n",token, token);
                        return FALSE;
                    }

                    if( ! MCaddNode( &machineCode, machineLine ) ){
                        printf("Failed to save machine code"); /* DELETE */
                        return FALSE;
                    }

                    token = strtok(NULL,",");

                } while(token != '\0');
                break;
            case INST_STRING: /* .string */
                token = strtok(NULL," ");

                removeSpaces(token);
                token++; /* skip first '"' */

                do{
                    machineLine.print = token[0];
                    
                    if( ! MCaddNode( &machineCode, machineLine ) ){
                        printf("Failed to save machine code"); /* DELETE */
                        return FALSE;
                    }
                    token++;
                    DC++;
                } while(token[0] != '"');

                break;
            case INST_EXTERN: /* extern */
            case INST_ENTERY: /* entry */
            default:
                break;
            }
            
            continue;
        }

        removeSpaces(token);

        if( (op = isOp( token )) > -1 ){

        }

    }

    printf("\nEND OF SECOND RUN: \n\n");

    printf("SymbolTable:\n");
    printST( *SymbolTable );

    printf("\nextFile:\n");
    printDT( *extFile );

    printf("\nentFile:\n");
    printDT( *entFile );

    return TRUE;
}