#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "consts.h"
#include "datatable.h"

Bool firstRun(FILE *, DTptr *, DTptr *, DTptr *);
Bool isLabel(char *);
Bool isRegister(char*);
Bool isMacro(char *);

void removeSpaces(char*);

Bool DTaddData(DTptr *, char*, char*, int);
void printDT( DTptr );

int isInstruction(char *, Instruction *);
int isOp(char *);