#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "consts.h"

Bool firstRun(FILE *);
Bool isLabel(char *);
Bool isRegister(char*);
Bool isValidMacro(char *);
int isInstruction(char *, Instruction *);
int isOp(char *);