#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "consts.h"
#include "datatable.h"

Bool firstRun(FILE *, STptr *, DTptr *, DTptr *);
Bool secondRun(FILE *, STptr *, DTptr *, DTptr *);

Bool isLabel(char *);
Bool isRegister(char*);
Bool isMacro(char *);

int getTwosComplement(int);

void removeSpaces(char*);

Bool DTaddNode(DTptr *, char*, int);
void printDT( DTptr );
Bool isInDT( DTptr, char * );
void updateDT( DTptr *, char *, int);

Bool STaddNode(STptr *, char*, char*, int);
void printST( STptr );
Bool isInST( STptr, char *, char* );
int getSTValue( STptr, char *, char *);

int isInstruction(char *);
int isOp(char *);
Bool isArray( char *);
char * getIndexFromToken( char * );
char * getLabelFromToken( char * );

Bool MCaddNode(MCptr *, word);