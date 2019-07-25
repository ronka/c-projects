#include "assembler.h"

Bool handleData(char *, DTptr *, int *, char *);
Bool handleFile(char *, DTptr *, int *);

Bool firstRun(FILE *sourceFile, STptr *SymbolTable, DTptr *extFile, DTptr *entFile){
    char line[MAX_LINE], tempStr[MAX_LINE];
    char *token;
    int op, instr; /* var used to save operation and instraction */
    int cnt_lines = 0; /* current line */
    int DC = 0, IC = 0;
    Bool labelFlag;
    Bool isStop = FALSE; /* checkstop flag */

    Instruction instructions[] = {
        {".data", 0},
        {".string", 1},
        {".extern", 2},
        {".entry", 3}
    };

    while (fgets(line, MAX_LINE, sourceFile)) {
        labelFlag = FALSE;
        printf("---------------------------\nLINE: %d \n", cnt_lines);

        strncpy ( tempStr, line, MAX_LINE );
        
        /*
         * If comment
         * */
        if( *line == ';' || strlen(line) == 0 ){
            printf("- comment\n"); /* DELETE */
            cnt_lines++;
            continue;
        }

        token = strtok(tempStr," ");

        if( isMacro( token ) ){
            
            token = strtok(NULL, "=");

            /* TODO: FIX THIS */
            removeSpaces(token);

            if( isRegister( token ) ){
                printf("%s is not a valid label name!!!\n", token); /* DELETE */
                return FALSE;
            }
            
            /* TODO: HANDLE INVALID MACRO */

            strcpy( tempStr, token );

            token = strtok(NULL, " ");

            token[strlen(token)-1]=0;

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
            token[strlen(token) - 1] = '\0'; /* clean delim */
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

        if( (instr = isInstruction( token, instructions )) > -1 ){
            /* TODO: HANDLE .data  */
            printf("- instruction: `%s`\n", instructions[instr].name); /* DELETE */
            switch (instr)
            {
            case 0: /* .data */
                if( ! labelFlag ){
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
                token = strtok(NULL," ");

                if( ! STaddNode( SymbolTable, tempStr, DATA, DC ) ){
                    printf("Failed to save symbol"); /* DELETE */
                    return FALSE;
                }

                continue;
            case 2: /* extern */
                token = strtok(NULL," ");

                token[strlen(token)-1] = 0; /* remove last char EOF */

                if( ! DTaddNode( extFile, token, DC ) ){
                    printf("Failed to save extern"); /* DELETE */
                    return FALSE;
                }
                continue;
            case 3: /* entry */
                token = strtok(NULL," ");

                token[strlen(token)-1] = 0; /* remove last char EOF */

                if( ! DTaddNode( entFile, token, DC ) ){
                    printf("Failed to save extern"); /* DELETE */
                    return FALSE;
                }

                continue;
            default:
                break;
            }
            /*(*instructions[instr].func)( tempStr, &SymbolTable, &DC, token );*/

            /* TODO: HANDLE .extrn .entry */
        }

        if( (op = isOp( token )) > -1 ){
            if( labelFlag ){
                if( ! STaddNode( SymbolTable, tempStr, CODE, DC ) ){
                    printf("failed to add to symbol table\n"); /* DELETE */
                    return FALSE;
                } else{

                }
            }
            printf("- op: `%s`, number: %d\n", token, op); /* DELETE */
            token = strtok(NULL," ");
        } else {
            printf("not recognized op: %s", token);
            return FALSE;
        }

        /* TODO: CHECKSTOP */
        if( !strcmp( STOP, token ) ){
            printf("- stop\n"); /* DELETE */
            isStop = TRUE;
        }

        if( !(instr < 0 || op < 0 || isStop == FALSE) ){
            printf("ERROR: NO ACTION/SYMBOL ERROR\n");
            return FALSE;
        }

        cnt_lines++;
        printf("---------------------------\n\n");
    }

    printf("SymbolTable:\n");
    printST( *SymbolTable );

    printf("\nextFile:\n");
    printDT( *extFile );

    printf("\nentFile:\n");
    printDT( *entFile );

    return TRUE;
}
/*
Bool handleData(char *label, DTptr * SymbolTable, int *DC,char * token){
    if( label != NULL ){
        token = strtok(NULL," ");

        if( ! DTaddData( SymbolTable, label, (*DC)) ){
            printf("This label allready exists"); 
            return FALSE;
        }
        do{
            if( token[0] != '"' && ! isdigit(token) && ! isInDT(*SymbolTable, token) ){
                printf("unrecognized macro %s", token);
                return FALSE;
            }
            (*DC)++;
        } while(token = strtok(NULL,","));
    }

    return TRUE;
}

Bool handleFile(char * label, DTptr * file, int *DC){
    if( label == NULL ){
        return FALSE;
    }
    if( ! DTaddData( *file, label, (*DC)) ){
        printf("This label allready exists");
        return FALSE;
    }
    return TRUE;
}
*/