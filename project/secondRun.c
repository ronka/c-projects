#include "assembler.h"

void newMachineLine( word * );

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
    Bool regFlag;
    DTptr extFileFinal = NULL;
    MCptr machineCode = NULL;
    word machineLine, srcArg, destArg, indexSrcArg, indexDestArg;

    DC = DC + MEMORY_START;

    while (fgets(line, MAX_LINE, sourceFile)) {
        /* reset temp vars */
        newMachineLine( &machineLine );
        newMachineLine( &srcArg );
        newMachineLine( &destArg );
        newMachineLine( &indexSrcArg );
        newMachineLine( &indexDestArg );
        regFlag = FALSE;
        strncpy ( tempStr, line, MAX_LINE );

        printf("%d -- %s\n",DC, line);

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
            removeSpaces(token);
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
                    DC++;

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

                machineLine.print = 0;
                    
                if( ! MCaddNode( &machineCode, machineLine ) ){
                    printf("Failed to save machine code"); /* DELETE */
                    return FALSE;
                }

                DC++; /* for end of line */
                break;
            case INST_EXTERN: /* extern */
            case INST_ENTERY: /* entry */
            default:
                break;
            }
            
            continue;
        }

        if( (op = isOp( token )) > -1 ){
            machineLine.cmd.opcode = op;
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

                    if( isRegister( token ) ){
                        /* if its a register, extract register number */
                        machineLine.cmd.srcOp = OPADDRESS_DIRECT_REG;
                        srcArg.reg.srcOperand = token[1] - '0';

                        regFlag = TRUE;
                        DC++;
                    } else if( token[0] == '#' ){
                        /* if its a number */
                        machineLine.cmd.srcOp = OPADDRESS_DIRECT;
                        token++;
                        strcpy(tempStr, token);

                        if( strtol(tempStr, NULL, 10) != 0 ){
                            /* if a regular number */
                            srcArg.reg.srcOperand = strtol(tempStr, NULL, 10);
                        } else if( isInST(*SymbolTable, token, MACRO) ){
                            /* if a macro */
                            srcArg.num.value = getSTValue( *SymbolTable, token, MACRO );
                        } else{
                            printf("error parsing token insinde number %s",token);
                            return FALSE;
                        }

                        DC++;
                    } else if( isArray( token ) ){
                        strcpy(tempStr, token);
                        if( isInDT( *extFile, getLabelFromToken(tempStr) ) ){
                            srcArg.num.decode = OPADDRESS_EXTERNAL;
                        } else{
                            srcArg.num.decode = OPADDRESS_RELOCATABLE;
                            srcArg.num.value = getSTValue( *SymbolTable, getLabelFromToken(tempStr), DATA );
                        }

                        strcpy(tempStr, token);
                        if( strtol(getIndexFromToken(tempStr), NULL, 10) != 0 ){
                            /* if index of array is number */
                            indexSrcArg.num.value = strtol(getIndexFromToken(tempStr), NULL, 10);
                        } else if(isInST( *SymbolTable, getIndexFromToken(tempStr), MACRO )){
                            /* if index of array is macro */
                            indexSrcArg.num.value = getSTValue( *SymbolTable, getIndexFromToken(tempStr), MACRO );
                            if( isInDT( *extFile, getLabelFromToken(tempStr) ) ){
                                /* if index is from external file */
                                indexSrcArg.num.decode = OPADDRESS_EXTERNAL;
                            } else{
                                indexSrcArg.num.decode = OPADDRESS_RELOCATABLE;
                            }
                        } else{
                            printf("error parsing token insinde array %s",token);
                            return FALSE;
                        }

                        DC = DC + 2;
                    } else if( isInST( *SymbolTable, token, DATA ) ){
                        srcArg.num.decode = OPADDRESS_RELOCATABLE;
                        srcArg.num.value = getSTValue( *SymbolTable, token, DATA );
                        DC++;
                    } else{
                        printf("dest op - error parsing token %s",line);
                        return FALSE;
                    }

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
                    /* get next op */
                    token = strtok(NULL, " ");
                    removeSpaces(token);

                    if( isRegister( token ) ){
                        if( regFlag ){
                            /* if there was allready a register machine line */
                            machineLine.cmd.destOp = OPADDRESS_DIRECT_REG;
                            srcArg.reg.destOperand = token[1] - '0';
                        } else{
                            machineLine.cmd.destOp = OPADDRESS_DIRECT_REG;
                            destArg.reg.destOperand = token[1] - '0';

                            DC++;
                        }
                    } else if( token[0] == '#' ){
                        /* if its a number */
                        machineLine.cmd.destOp = OPADDRESS_DIRECT;
                        token++;
                        strcpy(tempStr, token);

                        if( strtol(tempStr, NULL, 10) != 0 ){
                            /* if a regular number */
                            destArg.reg.srcOperand = strtol(tempStr, NULL, 10);
                        } else if( isInST(*SymbolTable, token, MACRO) ){
                            /* if a macro */
                            destArg.num.value = getSTValue( *SymbolTable, token, MACRO );
                        } else{
                            printf("error parsing token insinde number %s",token);
                            return FALSE;
                        }
                        DC++;
                    } else if( isArray( token ) ){
                        strcpy(tempStr, token);
                        if( isInDT( *extFile, getLabelFromToken(tempStr) ) ){
                            destArg.num.decode = OPADDRESS_EXTERNAL;
                        } else{
                            destArg.num.decode = OPADDRESS_RELOCATABLE;
                            destArg.num.value = getSTValue( *SymbolTable, getLabelFromToken(tempStr), DATA );
                        }

                        strcpy(tempStr, token);
                        if( strtol(getIndexFromToken(tempStr), NULL, 10) != 0 ){
                            /* if index of array is number */
                            indexDestArg.num.value = strtol(getIndexFromToken(tempStr), NULL, 10);
                        } else if(isInST( *SymbolTable, getIndexFromToken(tempStr), MACRO )){
                            /* if index of array is macro */
                            indexDestArg.num.value = getSTValue( *SymbolTable, getIndexFromToken(tempStr), MACRO );
                            if( isInDT( *extFile, getLabelFromToken(tempStr) ) ){
                                /* if index is from external file */
                                indexDestArg.num.decode = OPADDRESS_EXTERNAL;

                                if( ! DTaddNode( &extFileFinal, token, DC ) ){
                                    printf("Failed to save extern"); /* DELETE */
                                    return FALSE;
                                }
                            } else{
                                indexDestArg.num.decode = OPADDRESS_RELOCATABLE;
                            }
                        } else{
                            printf("error parsing token insinde array %s",token);
                            return FALSE;
                        }

                        DC = DC + 2;
                    } else if( isInST( *SymbolTable, token, DATA ) ){
                        destArg.num.decode = OPADDRESS_RELOCATABLE;
                        destArg.num.value = getSTValue( *SymbolTable, token, DATA );
                        DC++;
                    } else if( isInDT( *extFile, token ) ) {
                        /* if index is from external file */
                        destArg.num.decode = OPADDRESS_EXTERNAL;
                        DC++;

                        if( ! DTaddNode( &extFileFinal, token, DC ) ){
                            printf("Failed to save extern"); /* DELETE */
                            return FALSE;
                        }
                    } else{
                        printf("src op - error parsing line( %s ), token( %s )",line, token);
                        return FALSE;
                    }

                /* no args group */
                case rts:
                case stop:
                    DC++; /* increment DC for machine code line */
                    break;
                default:
                    printf("- not recognized op\n"); /* DELETE */
                    exit(1);
                    break;
            }

            continue;

        }

    }

    printf("\nEND OF SECOND RUN: \n\n");

    printf("SymbolTable:\n");
    printST( *SymbolTable );

    printf("\nextFile:\n");
    printDT( extFileFinal );

    printf("\nentFile:\n");
    printDT( *entFile );

    return TRUE;
}

/* Reset A machine line all bits */
void newMachineLine( word * machineLine ){
    (*machineLine).print = 0;
}