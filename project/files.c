#include "assembler.h"

Bool writeObjFile( MCptr  head, char * fileName ){
    char * fullFileName = concat(fileName, OBJECT_FILE_NAME);
    FILE *f = fopen(fullFileName, "w");
    MCptr p1 = head;
    
    if (f == NULL) {
        printf("Error opening file!\n"); /* DELETE */
        return FALSE;
    }

    while ( p1 != NULL ){
        /* print some text */
        fprintf(f, "%s\n", getBase4( p1->value ));
        p1 = p1->next;
    }

    fclose(f);

    return TRUE;
}

Bool writeDTFile( DTptr  head, char * fileName, char * suffix ){
    char * fullFileName = concat(fileName, suffix);
    FILE *f = fopen(fullFileName, "w");
    DTptr p1 = head;
    
    if (f == NULL) {
        printf("Error opening file!\n"); /* DELETE */
        return FALSE;
    }

    while ( p1 != NULL ){
        /* print some text */
        fprintf(f, "%s\t %04d\n", p1->name, p1->value);
        p1 = p1->next;
    }

    fclose(f);

    return TRUE;
}
