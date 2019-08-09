#include "assembler.h"

int getMemoryUsageOfOp( char *, STptr *, char *);

Bool firstRun(FILE *sourceFile, STptr *SymbolTable, DTptr *extFile, DTptr *entFile){
    char line[MAX_LINE], tempStr[MAX_LINE];
    char *token;
    int op, inst; /* var used to save operation and instraction */
    int DC = 0, lineCnt = 1; /* memory track */
    Bool labelFlag, regFlag, errorFlag = FALSE;

    DC = DC + MEMORY_START;

    while (fgets(line, MAX_LINE, sourceFile)) {
        labelFlag = FALSE;
        regFlag = FALSE;
        strncpy ( tempStr, line, MAX_LINE );
        
        /*
         * If comment, continue to next line
         * */
        if( *line == ';' || strlen(line) == 1 ){
            lineCnt++;
            continue;
        }

        token = strtok(tempStr," ");

        removeSpaces(token);

        if( isMacro( token ) ){
            token = strtok(NULL, "=");
            
            removeSpaces(token);

            if( isRegister( token ) ){
                printf("%04d - not a valid macro name\n", lineCnt);
                lineCnt++;
                errorFlag = TRUE;
                continue;
            }
            
            /* TODO: HANDLE INVALID MACRO */

            strcpy( tempStr, token );
            token = strtok(NULL, " ");

            /* check if macro is already exsits */
            if( isInST(*SymbolTable, tempStr, MACRO) ){
                printf("%04d - macro allready exists\n",lineCnt);
                lineCnt++;
                errorFlag = TRUE;
                continue;
            }

            if( ! STaddNode( SymbolTable, tempStr, MACRO, atoi(token) ) ){
                printf("%04d - error adding data table node\n", lineCnt);
                lineCnt++;
                errorFlag = TRUE;
                continue;
            }

            lineCnt++;
            continue;
        }

        if( isLabel( token ) ){
            token[strlen(token) - 1] = '\0'; /* clean delim (:) */
            
            /**
             *  check if label is valid name
             */
            if( isRegister( token ) ){
                printf("%04d - not a valid label name\n", lineCnt);
                lineCnt++;
                errorFlag = TRUE;
                continue;
            }

            if( isInST(*SymbolTable, token, DATA) ){
                printf("%04d - label allready exists\n",lineCnt);
                lineCnt++;
                errorFlag = TRUE;
                continue;
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
                    lineCnt++;
                    continue;
                }
                
                token = strtok(NULL,",");

                if( ! STaddNode( SymbolTable, tempStr, DATA, DC ) ){
                    printf("%04d - failed to save symbol\n",lineCnt);
                    lineCnt++;
                    errorFlag = TRUE;
                    continue;
                }

                do{
                    /* if a digit and not in the symbol table */
                    if( (isdigit(atoi(token)) == 0) && isInST(*SymbolTable, token , MACRO) ){
                        printf("%04d unrecognized macro\n", lineCnt);
                        errorFlag = TRUE;
                        continue;
                    }
                    DC++;
                    token = strtok(NULL,",");
                } while(token != NULL);

                continue;
            case INST_STRING: /* .string */
                if( ! labelFlag ){
                    /* if there is no label, no need to save the data */
                    lineCnt++;
                    continue;
                }

                token = strtok(NULL," ");

                removeSpaces(token);

                /* check if valid string */
                if( token[0] != '"' || token[strlen(token) - 1] != '"' ){
                    printf("%04d - invalid string\n",lineCnt);
                    lineCnt++;
                    errorFlag = TRUE;
                    continue;
                }
                
                if( ! STaddNode( SymbolTable, tempStr, DATA, DC ) ){
                    printf("%04d - failed to save symbol\n",lineCnt);
                    lineCnt++;
                    errorFlag = TRUE;
                    continue;
                }

                /* count string chars */
                do{
                    token++;
                    DC++;
                } while(token[0] != '"');

                lineCnt++;
                continue;
            case INST_EXTERN: /* extern */
                token = strtok(NULL," ");

                removeSpaces(token);

                if( ! DTaddNode( extFile, token, DC ) ){
                    printf("%04d - failed to save extern\n",lineCnt);
                    lineCnt++;
                    errorFlag = TRUE;
                    continue;
                }

                lineCnt++;
                continue;
            case INST_ENTERY: /* entry */
                token = strtok(NULL," ");

                removeSpaces(token);

                if( ! DTaddNode( entFile, token, DC ) ){
                    printf("%04d - failed to save extern\n",lineCnt);
                    lineCnt++;
                    errorFlag = TRUE;
                    continue;
                }

                lineCnt++;
                continue;
            default:
                break;
            }
        }

        removeSpaces(token);

        if( (op = isOp( token )) > -1 ){
            if( labelFlag ){
                if( isInST(*SymbolTable, tempStr, DATA) ){
                    printf("%04d - this label allready exists\n",lineCnt);
                    lineCnt++;
                    errorFlag = TRUE;
                    continue;
                }

                if( ! STaddNode( SymbolTable, tempStr, DATA, DC ) ){
                    printf("%04d - failed to add to symbol table\n",lineCnt);
                    lineCnt++;
                    errorFlag = TRUE;
                    continue;
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
                    printf("%04d - not recognized op\n", lineCnt);
                    lineCnt++;
                    errorFlag = TRUE;
                    continue;
                    break;
            }

            if( regFlag ){
                DC++;
            }

            lineCnt++;
            continue;
        }

        printf("%04d - not recognized op\n", lineCnt);
        errorFlag = TRUE;
        continue;

    }

    return ! errorFlag;
}

int getMemoryUsageOfOp( char * token, STptr * SymbolTable, char * tempStr ){
    if( isRegister(token) ) {
        /* if its a register in the dest op no need to increment DC */
        return 0;
    } else if( isArray( token ) ){
        return 2;
    }

    return 1;
}