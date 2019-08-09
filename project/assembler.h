/**
 * This is the api of the assembler
 * Here function protoypes and consts are decalred
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "consts.h"
#include "datatable.h"

/* Main functions */

Bool firstRun(FILE *, STptr *, DTptr *, DTptr *);
Bool secondRun(FILE *, STptr *, DTptr *, DTptr *, char *);

/**
 * Helpers
 */

int isOp(char *);
Bool isLabel(char *);
Bool isMacro(char *);
Bool isArray(char *);
Bool isRegister(char *);
int isInstruction(char *);
char *getIndexFromToken(char *, char *);
char *getLabelFromToken(char *, char *);

int getTwosComplement(int);
void removeSpaces(char *);

char *getBase4(word);
char *concat(char *, char *);

void print_short_bin(unsigned short int);
void cleanUp(char *, char *, STptr, DTptr, DTptr);

/**
 * Data symbol table fuctions
 */

void printDT(DTptr);
Bool isInDT(DTptr, char *);
void updateDT(DTptr *, char *, int);
Bool DTaddNode(DTptr *, char *, int);

void printST(STptr);
Bool isInST(STptr, char *, char *);
int getSTValue(STptr, char *, char *);
Bool STaddNode(STptr *, char *, char *, int);

void printMC(MCptr);
Bool MCaddNode(MCptr *, word);
Bool MCreplaceNodeN(MCptr *, word, int);

void freeST(STptr);
void freeDT(DTptr);
void freeMC(MCptr);

/**
 * Files functions
 */

Bool writeObjFile(MCptr, char *);
Bool writeDTFile(DTptr, char *, char *);