#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MEMORY_START 100
#define EXT_LEN 4
#define OUTPUT_BASE 4
#define MAX_LINE 256
#define OPCODES_LENGTH 16
#define INSTRUCTIONS_LENGTH 4

#define SOURCE_FILE_EXTENSION ".as"
#define ENTRIES_FILE_EXTENSION ".ent"
#define EXTERNALS_FILE_EXTENSION ".ext"
#define OBJECT_FILE_NAME ".ob"

#define DECODE_SIZE 2
#define OPERAND_SIZE 2
#define REG_SIZE 3
#define OPCODE_SIZE 4
#define UNUSED_SIZE 4
#define VALUE_SIZE 12

#define MACRO "macro"
#define DEFINE ".define"
#define CODE "code"
#define STOP "stop"
#define DATA ".data"
#define STRING ".string"

typedef enum {
	FALSE, TRUE
} Bool;

enum instructionMap {
    INST_DATA, INST_STRING, INST_EXTERN, INST_ENTERY
};

/* Opcodes enum table */
typedef enum {
    mov,
    cmp,
    add,
    sub,
    not,
    clr,
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

typedef struct {
    char *name;
    int num;
} Instruction;

typedef struct {
	unsigned int decode : DECODE_SIZE;
	unsigned int destOp : OPERAND_SIZE;
	unsigned int srcOp : OPERAND_SIZE;
	unsigned int opcode : OPCODE_SIZE;
	unsigned int reserved : UNUSED_SIZE;
} cmd_word;

/* Structure of an argument word */
typedef struct {
	unsigned int decode: DECODE_SIZE;
	unsigned int value : VALUE_SIZE;
} arg_word;


typedef struct {
	unsigned int decode 	: DECODE_SIZE;
	unsigned int destOperand: REG_SIZE;
	unsigned int srcOperand : REG_SIZE;
	unsigned int zeroBit 	: UNUSED_SIZE;
} reg_word;

typedef union {
	cmd_word cmd;
	reg_word reg;
	arg_word num;
	unsigned int print: 14;
} word;

#endif