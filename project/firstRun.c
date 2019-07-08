#include "assembler.h"

Bool firstRun(FILE *sourceFile){
    char line[MAX_LINE], temp_line[MAX_LINE];
    char *token;
    int cnt_lines = 0;

    while (fgets(line, MAX_LINE, sourceFile)) {
        cnt_lines++;

        strncpy ( temp_line, line, MAX_LINE );
        
        /* clean white spaces 
        while( isspace(line) ){
            line++;
        } */
        
        if( *line == ';' || strlen(line) == 0 ){
            continue;
        }

        token = strtok(temp_line," ");
    
        if( isLabel( token ) ){
            token[strlen(token) - 1] = '\0'; /* clean delim */
            printf("%s || this is a label of line :\n", token, line);      
            printf("%s\n",line);

            /* check if label is valid name */
            if( ! isRegister( token ) ){
                printf("%s is not a valid label name!!!\n", token);
                return FALSE;
            }
        }
    }

    return TRUE;
}