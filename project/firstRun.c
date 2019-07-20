#include "assembler.h"

Bool firstRun(FILE *sourceFile){
    char line[MAX_LINE], tempStr[MAX_LINE];
    char *token;
    int op, instr; /* var used to save operation and instraction */
    int cnt_lines = 0; /* current line */
    Bool isStop = FALSE; /* checkstop flag */

    DTptr DataTable = NULL; /* data table */

    Instruction instructions[] = {
        {".data", NULL},
        {".string", NULL},
        {".extern", NULL},
        {".entry", NULL}
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
            token = strtok(NULL, " ");

            if( isRegister( token ) ){
                printf("%s is not a valid label name!!!\n", token); /* DELETE */
                return FALSE;
            }
            
            /* TODO: HANDLE INVALID MACRO */

            strcpy( tempStr, token );

            /*removeSpaces(&token); */

            token = strtok(NULL, "= "); /* TODO: FIX THIS */

            if( ! DTaddData( &DataTable, tempStr, atoi(token) ) ){
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

            /* TODO: check if label allready exists */

            token = strtok(NULL," ");
        }

        if( (instr = isInstruction( token, instructions )) > -1 ){
            /* TODO: HANDLE .string .data  */
            printf("- instruction: `%s`\n", instructions[instr].name); /* DELETE */ 

            /* TODO: HANDLE .extrn .entry */

            token = strtok(NULL," ");
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