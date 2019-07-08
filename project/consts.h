#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MEMORY_START 100
#define EXT_LEN 4
#define OUTPUT_BASE 4
#define MAX_LINE 256

#define SOURCE_FILE_EXTENSION ".as"
#define ENTRIES_FILE_EXTENSION ".ent"
#define EXTERNALS_FILE_EXTENSION ".ext"
#define OBJECT_FILE_NAME ".ob"

typedef enum {
	FALSE, TRUE
} Bool;

/* Opcodes enum table */
typedef enum {
    mov,
    cmp,
    add,
    sub,
    not,
    cls,
    lea,
    inc,
    dec,
    jmp,
    bne,
    red,
    prn,
    jsr,
    rts,
    stop
} Opcode;

#endif