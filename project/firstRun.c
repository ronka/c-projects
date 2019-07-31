#include "assembler.h"

int getMemoryUsageOfOp( char *, STptr *, char *);

Bool firstRun(FILE *sourceFile, STptr *SymbolTable, DTptr *extFile, DTptr *entFile){
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

        if( isMacro( token ) ){
            token = strtok(NULL, "=");
            
            removeSpaces(token);

            if( isRegister( token ) ){
                printf("%s is not a valid macro name!!!\n", token); /* DELETE */
                return FALSE;
            }
            
            /* TODO: HANDLE INVALID MACRO */

            strcpy( tempStr, token );
            token = strtok(NULL, " ");

            /* check if macro is already exsits */
            if( isInST(*SymbolTable, tempStr, MACRO) ){
                printf("This macro allready exists"); /* DELETE */
                return FALSE;
            }

            if( ! STaddNode( SymbolTable, tempStr, MACRO, atoi(token) ) ){
                printf("ERROR adding data table node"); /* DELETE */
                return FALSE;
            }

            continue;
        }

        if( isLabel( token ) ){
            token[strlen(token) - 1] = '\0'; /* clean delim (:) */
            
            /**
             *  check if label is valid name
             */

            if( isRegister( token ) ){
                printf("%s is not a valid label name!!!\n", token); /* DELETE */
                return FALSE;
            }

            if( isInST(*SymbolTable, token, CODE) ){
                printf("This label allready exists"); /* DELETE */
                return FALSE;
            }

            strcpy(tempStr, token);

            token = strtok(NULL," ");

            labelFlag = TRUE;
        } else{
            labelFlag = FALSE; /* our flag if there is label */
        }

        if( (inst = isInstruction( token )) > -1 ){
            switch (inst)
            {
            case INST_DATA: /* .data */
                if( ! labelFlag ){
                    /* if there is no label, no need to save the data */
                    continue;
                }
                
                token = strtok(NULL,",");

                if( ! STaddNode( SymbolTable, tempStr, DATA, DC ) ){
                    printf("Failed to save symbol"); /* DELETE */
                    return FALSE;
                }

                do{
                    /* if a digit and not in the symbol table */
                    if( (isdigit(atoi(token)) == 0) && isInST(*SymbolTable, token , MACRO) ){
                        printf("unrecognized macro %s", token); /* DELETE */
                        return FALSE;
                    }
                    DC++;
                    token = strtok(NULL,",");
                } while(token != '\0');

                continue;
            case INST_STRING: /* .string */
                if( ! labelFlag ){
                    /* if there is no label, no need to save the data */
                    continue;
                }

                token = strtok(NULL," ");

                removeSpaces(token);

                /* check if valid string */
                if( token[0] != '"' || token[strlen(token) - 1] != '"' ){
                    printf("invalid string"); /* DELETE */
                    exit(1);
                    return FALSE;
                }
                
                if( ! STaddNode( SymbolTable, tempStr, DATA, DC ) ){
                    printf("Failed to save symbol"); /* DELETE */
                    return FALSE;
                }

                /* count string chars */
                do{
                    token++;
                    DC++;
                } while(token[0] != '"');

                continue;
            case INST_EXTERN: /* extern */
                token = strtok(NULL," ");

                removeSpaces(token);

                if( ! DTaddNode( extFile, token, DC ) ){
                    printf("Failed to save extern"); /* DELETE */
                    return FALSE;
                }
                continue;
            case INST_ENTERY: /* entry */
                token = strtok(NULL," ");

                removeSpaces(token);

                if( ! DTaddNode( entFile, token, DC ) ){
                    printf("Failed to save extern"); /* DELETE */
                    return FALSE;
                }

                continue;
            default:
                break;
            }
        }

        removeSpaces(token);

        if( (op = isOp( token )) > -1 ){
            if( labelFlag ){
                if( isInST(*SymbolTable, tempStr, CODE) ){
                    printf("This label allready exists"); /* DELETE */
                    return FALSE;
                }

                if( ! STaddNode( SymbolTable, tempStr, CODE, DC ) ){
                    printf("failed to add to symbol table\n"); /* DELETE */
                    return FALSE;
                }
            }

            switch(op){
                /* two args group */
                case mov:
                case cmp:
                case add:
                case sub:
                case lea:
                    /* get origin op */
                    token = strtok(NULL, ",");
                    
                    removeSpaces(token);

                    DC = DC + getMemoryUsageOfOp(token, SymbolTable, tempStr);

                    if( isRegister( token ) ){
                        regFlag = TRUE;
                    }

                    /* get next op */
                    token = strtok(NULL, " ");

                    removeSpaces(token);
                /* one arg group */
                case not:
                case clr:
                case inc:
                case dec:
                case jmp:
                case bne:
                case red:
                case prn:
                case jsr:
                    /* get dest op */
                    DC = DC + getMemoryUsageOfOp(token, SymbolTable, tempStr);

                    if( isRegister( token ) ){
                        regFlag = TRUE;
                    }
                /* no args group */
                case rts:
                case stop:
                    DC++;
                    break;
                default:
                    printf("- not recognized op\n"); /* DELETE */
                    exit(1);
                    break;
            }

            if( regFlag ){
                DC++;
            }

            continue;
        }

        printf("- not recognized op\n"); /* DELETE */
        exit(1);

    }

    printf("\nEND OF FIRST RUN: \n\n");

    printf("SymbolTable:\n");
    printST( *SymbolTable );

    printf("\nextFile:\n");
    printDT( *extFile );

    printf("\nentFile:\n");
    printDT( *entFile );

    return TRUE;
}

int getMemoryUsageOfOp( char * token, STptr * SymbolTable, char * tempStr ){
    /* DELETE */
    /* if( isMacro(token) ){
        return 1;
    } else if( token[0] == '#' ){
        token++; /* skip the # char */
        /* if( isdigit(atoi(token)) == 0 && ! isInST(*SymbolTable, token, MACRO) ){
            printf("not a good number,%s",token); /* DELETE */
            /* return FALSE;
        }
        return 1;
    } else */
    if( isRegister(token) ) {
        /* if its a register in the dest op no need to increment DC */
        return 0;
    } else if( isArray( token ) ){
        return 2;
    }

    return 1;
}