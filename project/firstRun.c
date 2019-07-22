#include "assembler.h"

Bool handleData(char *, DTptr *, int *, char *);
Bool handleFile(char *, DTptr *, int *);

Bool firstRun(FILE *sourceFile, DTptr *DataTable, DTptr *extFile, DTptr *entFile){
    char line[MAX_LINE], tempStr[MAX_LINE];
    char *token;
    int op, instr; /* var used to save operation and instraction */
    int cnt_lines = 0; /* current line */
    int DC = 0, IC = 0;
    Bool isStop = FALSE; /* checkstop flag */

    Instruction instructions[] = {
        {".data", 0},
        {".string", 1},
        {".extern", 2},
        {".entry", 3}
    };

    while (fgets(line, MAX_LINE, sourceFile)) {
        printf("---------------------------\nLINE: %d \n", cnt_lines);

        strncpy ( tempStr, line, MAX_LINE );
    
        /* clean white spaces 
        while( isspace(line) ){
            line++;
        } */
        
        if( *line == ';' || strlen(line) == 0 ){
            printf("- comment\n"); /* DELETE */
            cnt_lines++;
            continue;
        }

        token = strtok(tempStr," ");

        if( isMacro( token ) ){
            token = strtok(NULL, " = ");

            /* TODO: FIX THIS */
            /*removeSpaces(token);*/

            if( isRegister( token ) ){
                printf("%s is not a valid label name!!!\n", token); /* DELETE */
                return FALSE;
            }
            
            /* TODO: HANDLE INVALID MACRO */

            strcpy( tempStr, token );

            token = strtok(NULL, " "); /* TODO: FIX THIS */

            if( isInDT(*DataTable, tempStr, MACRO) ){
                printf("This macro allready exists"); /* DELETE */
                return FALSE;
            }

            if( ! DTaddData( DataTable, tempStr, MACRO, atoi(token) ) ){
                printf("ERROR adding data table node"); /* DELETE */
                return FALSE;
            }
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

            if( isInDT(*DataTable, token, NULL) ){
                printf("This label allready exists"); /* DELETE */
                return FALSE;
            }

            strcpy(tempStr, token);

            token = strtok(NULL," ");
        } else{
            *tempStr = NULL; /* our flag if there is label */
        }

        if( (instr = isInstruction( token, instructions )) > -1 ){
            /* TODO: HANDLE .data  */
            printf("- instruction: `%s`\n", instructions[instr].name); /* DELETE */
            switch (instr)
            {
            case 0: /* .data */
            case 1: /* .string */
                if( tempStr != NULL ){
                    token = strtok(NULL," ");

                    if( ! DTaddData( DataTable, tempStr, MACRO, atoi(token) ) ){
                        printf("This label allready exists"); /* DELETE */
                        return FALSE;
                    }

                    if(instr == 1){
                        break;
                    }
                    
                    do{
                        if( token[0] != '"' && ! isdigit(token) && ! isInDT(*DataTable, token , MACRO) ){
                            printf("unrecognized macro %s", token); /* DELETE */
                            return FALSE;
                        }
                        DC++;
                    } while(token = strtok(NULL,","));
                }
                /* code */
                break;
            
            default:
                break;
            }
            /*(*instructions[instr].func)( tempStr, &DataTable, &DC, token );*/

            /* TODO: HANDLE .extrn .entry */
        }

        if( (op = isOp( token )) > -1 ){
            if( instr > -1 ){
                printf("ERROR: CANT HAVE OP AFTER INSTRUCTION");
                return FALSE;
            }
            printf("- op: `%s`, number: %d\n", token, op); /* DELETE */
            token = strtok(NULL," ");
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

    printf("DATATABLE:\n");
    printDT( DataTable );

    return TRUE;
}

Bool handleData(char *label, DTptr * DataTable, int *DC,char * token){
    if( label != NULL ){
        token = strtok(NULL," ");

        if( ! DTaddData( DataTable, label, ".data", (*DC)) ){
            printf("This label allready exists"); /* DELETE */
            return FALSE;
        }
        do{
            if( token[0] != '"' && ! isdigit(token) && ! isInDT(*DataTable, token , MACRO) ){
                printf("unrecognized macro %s", token); /* DELETE */
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
    if( ! DTaddData( *file, label, "file", (*DC)) ){
        printf("This label allready exists"); /* DELETE */
        return FALSE;
    }
    return TRUE;
}