#include "assembler.h"

int main(int argc, char *argv[]) {
    int i;
    char *currentFileName, *fileNameNoSuffix;
    Bool isCompiled;
    FILE *sourceFile;
    STptr SymbolTable = NULL;
    DTptr extFile = NULL, entFile = NULL; /* data table */

    if (argc == 1) {
        printf("Error, no input arguments.\n");
        return 0;
    }

    for (i = 1; i < argc; i++){
        isCompiled = TRUE;
        extFile = NULL;
        entFile = NULL;
        SymbolTable = NULL;
        fileNameNoSuffix = malloc(strlen(argv[i]));
        currentFileName = malloc(strlen(argv[i]) + strlen(SOURCE_FILE_EXTENSION) + 1);

        sprintf(fileNameNoSuffix, "%s", argv[i]);
        sprintf(currentFileName, "%s%s", argv[i], SOURCE_FILE_EXTENSION);
        printf("Compiling %s...\n", currentFileName);

        sourceFile = fopen(currentFileName, "r");

        if (sourceFile == NULL) {
            printf("Unable to open file %s.\n", currentFileName);
            continue;
        }

        isCompiled = firstRun(sourceFile, &SymbolTable, &extFile, &entFile);

        rewind(sourceFile);

        /* if first run on the file failed */
        if( ! isCompiled ){
            printf("Unable to parse %s.\n", currentFileName);
            cleanUp(fileNameNoSuffix, currentFileName, SymbolTable, extFile, entFile);
            continue;
        }

        isCompiled = secondRun(sourceFile, &SymbolTable, &extFile, &entFile, fileNameNoSuffix);

        if( ! isCompiled ){
            printf("Failed parsing the file %s.\n", argv[i]);
            cleanUp(fileNameNoSuffix, currentFileName, SymbolTable, extFile, entFile);
            continue;
        }

        printf("%s Compiled successfully\n\n", currentFileName);
    }

    cleanUp(fileNameNoSuffix, currentFileName, SymbolTable, extFile, entFile);

    return 0;
}

void cleanUp( char* str1,char* str2,STptr st, DTptr dt, DTptr dt2){
    free(str1);
    free(str2);
    freeST( st );
    freeDT( dt );
    freeDT( dt2 );
}