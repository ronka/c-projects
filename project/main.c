#include "assembler.h"

int main(int argc, char *argv[]) {
    int i;
    char *currentFileName;
    Bool status;
    FILE *sourceFile;
    DTptr DataTable = NULL, extFile = NULL, entFile = NULL; /* data table */

    if (argc == 1) {
        printf("Error, no input arguments.\n");
        return 0;
    }

    for (i = 1; i < argc; i++){
        currentFileName = malloc(strlen(argv[i]) + strlen(SOURCE_FILE_EXTENSION) + 1);

        sprintf(currentFileName, "%s%s", argv[i], SOURCE_FILE_EXTENSION);
        printf("Compiling %s...\n", currentFileName);

        sourceFile = fopen(currentFileName, "r");

        if (sourceFile == NULL) {
            printf("Unable to open file %s.\n", currentFileName);
            continue;
        }

        status = firstRun(sourceFile, &DataTable, &extFile, &entFile);

        /* if first run on the file failed */
        if( ! status ){
            printf("Unable to parse %s.\n", currentFileName);
            continue;
        }
    }

    return 0;
}