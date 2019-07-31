#include "assembler.h"

Bool secondRun(FILE *sourceFile, STptr *SymbolTable, DTptr *extFile, DTptr *entFile){
    char line[MAX_LINE], tempStr[MAX_LINE];
    char *token;
    int op, inst; /* var used to save operation and instraction */
    int DC = 0, IC = 0; /* memory track */
    Bool labelFlag, regFlag;

    DC = DC + MEMORY_START;

    while (fgets(line, MAX_LINE, sourceFile)) {
        labelFlag = FALSE;
        regFlag = FALSE;
        strncpy ( tempStr, line, MAX_LINE );
        
        /*
         * If comment, continue to next line
         * */
        if( *line == ';' || strlen(line) == 1 ){
            continue;
        }

        token = strtok(tempStr," ");

        removeSpaces(token);

        if( isMacro( token ) ){ /* if its macro, skip */
            token = strtok(NULL, "=");
            
            removeSpaces(token);
        } else if( isLabel( token ) ){ /* if its label, skip */
            token[strlen(token) - 1] = '\0'; /* clean delim (:) */
            if( isInDT(*entFile, token) ){
                updateDT(entFile, token, 6666);
            }
            token = strtok(NULL," ");
        }

        if( (inst = isInstruction( token )) > -1 ){
            switch (inst)
            {
            case INST_DATA: /* .data */
                
            case INST_STRING: /* .string */
                
            case INST_EXTERN: /* extern */

            case INST_ENTERY: /* entry */

            default:
                break;
            }
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