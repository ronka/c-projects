#include "assembler.h"

Bool handleData(char *, DTptr *, int *, char *);
Bool handleFile(char *, DTptr *, int *);

Bool firstRun(FILE *sourceFile, STptr *SymbolTable, DTptr *extFile, DTptr *entFile){
    char line[MAX_LINE], tempStr[MAX_LINE];
    char *token;
    int op, instr; /* var used to save operation and instraction */
    int cnt_lines = 0; /* current line */
    int DC = 0, IC = 0;
    DC = DC + MEMORY_START;
    Bool labelFlag;
    Bool isStop = FALSE; /* checkstop flag */

    while (fgets(line, MAX_LINE, sourceFile)) {
        labelFlag = FALSE;
        printf("---------------------------\n");

        strncpy ( tempStr, line, MAX_LINE );
        
        /*
         * If comment, continue to next line
         * */
        if( *line == ';' || strlen(line) == 0 ){
            printf("- comment\n"); /* DELETE */
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
            printf("- label: %s\n", token); /* DELETE */

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

        if( (instr = isInstruction( token )) > -1 ){
            switch (instr)
            {
            case 0: /* .data */
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
            case 1: /* .string */
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

                continue;
            case 2: /* extern */
                token = strtok(NULL," ");

                if( ! DTaddNode( extFile, token, DC ) ){
                    printf("Failed to save extern"); /* DELETE */
                    return FALSE;
                }
                continue;
            case 3: /* entry */
                token = strtok(NULL," ");

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
                    /* get destination var */
                    token = strtok(NULL, ",");
                    
                    removeSpaces(token);

                    if( token[0] == '#' ){
                        token++;
                        /* if its not a digit and not a macro, throw error */
                        if( isdigit(atoi(token)) == 0 && ! isInST(*SymbolTable, tempStr, MACRO) ){
                            printf("not a good number"); /* DELETE */
                            return FALSE;
                        }
                        DC++;
                    } else if( isRegister(token) ) {
                        DC++;
                    } else if( isArray( token ) ){
                        DC += 2;
                    }

                    printf("%s",token);
                    exit(1);

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
                 
                /* no args group */
                case rts:
                case stop:
                    
                continue;
            }

            token = strtok(NULL," ");

            continue;
        }

        printf("- not recognized op\n"); /* DELETE */
        exit(1);

    }

    printf("SymbolTable:\n");
    printST( *SymbolTable );

    printf("\nextFile:\n");
    printDT( *extFile );

    printf("\nentFile:\n");
    printDT( *entFile );

    return TRUE;
}