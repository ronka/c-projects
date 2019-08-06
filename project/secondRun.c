#include "assembler.h"

void newMachineLine( word * );

Bool secondRun(FILE *sourceFile, STptr *SymbolTable, DTptr *extFile, DTptr *entFile, char * fileName){
    char *line = malloc(MAX_LINE);
    char *tempStr = malloc(MAX_LINE);
    char *token = malloc(MAX_LINE);
    int op, inst; /* var used to save operation and instraction */
    int DC = 0, lineCnt = 1; /* memory track */
    Bool regFlag, errorFlag = FALSE;
    DTptr extFileFinal = NULL;
    MCptr machineCode = NULL;
    MCptr tempMachineCode = NULL;
    word machineLine, srcArg, destArg, indexSrcArg, indexDestArg;

    DC = DC + MEMORY_START;

    while (fgets(line, MAX_LINE, sourceFile)) {
        /* reset temp vars */
        newMachineLine( &machineLine );
        newMachineLine( &srcArg );
        newMachineLine( &destArg );
        newMachineLine( &indexSrcArg );
        newMachineLine( &indexDestArg );
        tempMachineCode = NULL;
        regFlag = FALSE;
        strncpy ( tempStr, line, MAX_LINE );
 
        /* If comment, continue to next line */
        if( *line == ';' || strlen(line) == 1 ){
            lineCnt++;
            continue;
        }

        token = strtok(tempStr," ");

        removeSpaces(token);

        if( isMacro( token ) ){ /* if its macro, skip */
            lineCnt++;
            continue;
        } else if( isLabel( token ) ){ /* if its label, skip */
            token[strlen(token) - 1] = '\0'; /* clean delim (:) */
            if( isInDT(*entFile, token) ) {
                /* update address of entry label */
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
                        printf("%04d - invalid string\n",lineCnt);
                        lineCnt++;
                        errorFlag = TRUE;
                        continue;
                    }

                    if( ! MCaddNode( &machineCode, machineLine ) ){
                        printf("Failed to save machine code\n");
                        exit(1);
                    }
                    DC++;

                    token = strtok(NULL,",");

                } while(token != NULL);
                break;
            case INST_STRING: /* .string */
                token = strtok(NULL," ");

                removeSpaces(token);
                token++; /* skip first '"' */

                do{
                    machineLine.print = token[0];
                    
                    if( ! MCaddNode( &machineCode, machineLine ) ){
                        printf("Failed to save machine code\n");
                        exit(1);
                    }
                    token++;
                    DC++;
                } while(token[0] != '"');

                machineLine.print = 0;
                    
                if( ! MCaddNode( &machineCode, machineLine ) ){
                    printf("Failed to save machine code\n");
                    exit(1);
                }

                DC++; /* for end of line */
                break;
            case INST_EXTERN: /* extern */
            case INST_ENTERY: /* entry */
            default:
                break;
            }

            lineCnt++;
            continue;
        }

        if( (op = isOp( token )) > -1 ){
            machineLine.cmd.opcode = op;

            if( ! MCaddNode( &tempMachineCode, machineLine ) ){
                printf("Failed to save machine code\n");
                exit(1);
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

                    if( isRegister( token ) ){
                        machineLine.cmd.srcOp = OPADDRESS_DIRECT_REG;
                        srcArg.reg.srcOperand = token[1] - '0'; /* if its a register, extract register number */

                        if( ! MCaddNode( &tempMachineCode, srcArg ) ){
                            printf("Failed to save machine code\n");
                            exit(1);
                        }

                        regFlag = TRUE;
                        DC++;
                    } else if( token[0] == '#' ){
                        /* if its a number */
                        machineLine.cmd.srcOp = OPADDRESS_INSTANT;
                        token++;
                        strcpy(tempStr, token);

                        if( strtol(tempStr, NULL, 10) != 0 ){
                            /* if a regular number */
                            srcArg.num.value = strtol(tempStr, NULL, 10);
                        } else if( isInST(*SymbolTable, token, MACRO) ){
                            /* if a macro */
                            srcArg.num.value = getSTValue( *SymbolTable, token, MACRO );
                        } else{
                            printf("%04d - invalid number",lineCnt); /* DELETE */
                            lineCnt++;
                            errorFlag = TRUE;
                            continue;
                        }

                        if( ! MCaddNode( &tempMachineCode, srcArg ) ){
                            printf("Failed to save machine code \n"); /* DELETE */
                            exit(1);
                        }

                        DC++;
                    } else if( isArray( token ) ){
                        getLabelFromToken(token, tempStr);

                        machineLine.cmd.srcOp = OPADDRESS_INDEX;

                        if( isInDT( *extFile, tempStr ) ){
                            srcArg.num.decode = OPADDRESS_EXTERNAL;
                        } else{
                            srcArg.num.decode = OPADDRESS_RELOCATABLE;
                            srcArg.num.value = getSTValue( *SymbolTable, tempStr, DATA );
                        }

                        getIndexFromToken(token, tempStr);

                        if( strtol(tempStr, NULL, 10) != 0 ){
                            /* if index of array is number */
                            indexSrcArg.num.value = strtol(tempStr, NULL, 10);
                        } else if(isInST( *SymbolTable, tempStr, MACRO )){
                            /* if index of array is macro */
                            indexSrcArg.num.value = getSTValue( *SymbolTable, tempStr, MACRO );
                        } else{
                            printf("%04d - invalid index",lineCnt);
                            lineCnt++;
                            errorFlag = TRUE;
                            continue;
                        }

                        if( !(MCaddNode( &tempMachineCode, srcArg ) && MCaddNode( &tempMachineCode, indexSrcArg )) ){
                            printf("Failed to save machine code\n");
                            exit(1);
                        }

                        DC = DC + 2;
                    } else if( isInST( *SymbolTable, token, DATA ) ){
                        srcArg.num.decode = OPADDRESS_RELOCATABLE;
                        srcArg.num.value = getSTValue( *SymbolTable, token, DATA );
                        DC++;

                        machineLine.cmd.srcOp = OPADDRESS_DIRECT;

                        if( ! MCaddNode( &tempMachineCode, srcArg ) ){
                            printf("Failed to save machine code");
                            exit(1);
                        }
                    } else if( isInDT( *extFile, token ) ) {
                        /* if index is from external file */
                        srcArg.num.decode = OPADDRESS_EXTERNAL;
                        machineLine.cmd.srcOp = OPADDRESS_DIRECT;
                        DC++;

                        if( ! DTaddNode( &extFileFinal, token, DC ) ){
                            printf("Failed to save extern");
                            exit(1);
                        }


                        if( ! (MCaddNode( &tempMachineCode, srcArg )) ){
                            printf("Failed to save machine code");
                            exit(1);
                        }
                    } else{
                        printf("%04d - error parsing token\n",lineCnt);
                        lineCnt++;
                        errorFlag = TRUE;
                        continue;
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
                            /* if was allready a register machine line */
                            machineLine.cmd.destOp = OPADDRESS_DIRECT_REG;
                            srcArg.reg.destOperand = token[1] - '0';
                            
                            MCreplaceNodeN( &tempMachineCode, srcArg, 1 ); /* replace srcArg value */
                        } else{
                            machineLine.cmd.destOp = OPADDRESS_DIRECT_REG;
                            destArg.reg.destOperand = token[1] - '0';

                            DC++;

                            if( ! MCaddNode( &tempMachineCode, destArg ) ){
                                printf("Failed to save machine code"); 
                                exit(1);
                            }
                        }
                    } else if( token[0] == '#' ){
                        /* if its a number */
                        machineLine.cmd.destOp = OPADDRESS_INSTANT;
                        token++;
                        strcpy(tempStr, token);

                        if( strtol(tempStr, NULL, 10) != 0 ){
                            /* if a regular number */
                            destArg.num.value = strtol(tempStr, NULL, 10);
                        } else if( isInST(*SymbolTable, token, MACRO) ){
                            /* if a macro */
                            destArg.num.value = getSTValue( *SymbolTable, token, MACRO );
                        } else{
                            printf("%04d - error parsing token insinde number\n",lineCnt);
                            lineCnt++;
                            errorFlag = TRUE;
                            continue;
                        }

                        if( ! MCaddNode( &tempMachineCode, destArg ) ){
                            printf("Failed to save machine code");
                            exit(1);
                        }

                        DC++;
                    } else if( isArray( token ) ){
                        getLabelFromToken(token, tempStr);

                        machineLine.cmd.destOp = OPADDRESS_INDEX;
                        if( isInDT( *extFile, tempStr ) ){
                            destArg.num.decode = OPADDRESS_EXTERNAL;
                        } else{
                            destArg.num.decode = OPADDRESS_RELOCATABLE;
                            destArg.num.value = getSTValue( *SymbolTable, tempStr, DATA );
                        }

                        getIndexFromToken(token, tempStr);

                        if( strtol(tempStr, NULL, 10) != 0 ){
                            /* if index of array is number */
                            indexDestArg.num.value = strtol(tempStr, NULL, 10);
                        } else if(isInST( *SymbolTable, tempStr, MACRO )){
                            /* if index of array is macro */
                            indexDestArg.num.value = getSTValue( *SymbolTable, tempStr, MACRO );
                        } else{
                            printf("%s, %s\n", tempStr, tempStr);
                            printf("%04d - invalid index inside array\n",lineCnt);
                            lineCnt++;
                            errorFlag = TRUE;
                            continue;
                        }

                        if( ! (MCaddNode( &tempMachineCode, destArg ) && MCaddNode( &tempMachineCode, indexDestArg )) ){
                            printf("Failed to save machine code");
                            exit(1);
                        }

                        DC = DC + 2;
                    } else if( isInDT( *extFile, token ) ) {
                        /* if index is from external file */
                        destArg.num.decode = OPADDRESS_EXTERNAL;
                        machineLine.cmd.destOp = OPADDRESS_DIRECT;
                        DC++;

                        if( ! DTaddNode( &extFileFinal, token, DC ) ){
                            printf("Failed to save extern");
                            exit(1);
                        }

                        if( ! (MCaddNode( &tempMachineCode, destArg )) ){
                            printf("Failed to save machine code");
                            exit(1);
                        }
                    }  else if( isInST( *SymbolTable, token, DATA ) ){
                        destArg.num.decode = OPADDRESS_RELOCATABLE;
                        destArg.num.value = getSTValue( *SymbolTable, token, DATA );
                        DC++;

                        machineLine.cmd.destOp = OPADDRESS_DIRECT;

                        if( ! (MCaddNode( &tempMachineCode, destArg )) ){
                            printf("Failed to save machine code");
                            exit(1);
                        }

                    } else{
                        printf("%04d - invalid token\n",lineCnt);
                        lineCnt++;
                        errorFlag = TRUE;
                        continue;
                    }

                    /* update first line of the machine code chunk */
                    MCreplaceNodeN( &tempMachineCode, machineLine, 0 );
                /* no args group */
                case rts:
                case stop:
                    DC++; /* increment DC for machine code line */
                    break;
                default:
                    printf("%04d not recognized op\n",lineCnt); /* DELETE */
                    lineCnt++;
                    errorFlag = TRUE;
                    continue;
                    break;
            }

            while( tempMachineCode != NULL ){
                MCaddNode( &machineCode, tempMachineCode->value );
                tempMachineCode = tempMachineCode->next;
            }

            lineCnt++;
            continue;

        }

    }

    writeObjFile( machineCode, fileName );
    
    if( extFileFinal != NULL ){
        writeDTFile( extFileFinal, fileName, EXTERNALS_FILE_EXTENSION );
    }
    if( *entFile != NULL ){
        writeDTFile( *entFile, fileName, ENTRIES_FILE_EXTENSION );
    }

    return ! errorFlag;
}

/* Reset A machine line all bits */
void newMachineLine( word * machineLine ){
    (*machineLine).print = 0;
}