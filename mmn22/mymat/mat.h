#include <stdio.h>
#include <string.h>

#define SIZE 4
#define LENGTH 100

typedef double mat[SIZE][SIZE];

mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F;

enum flag { FALSE, TRUE };

void read_mat();
void print_mat();
void add_mat();
void sub_mat();
void mul_mat();
void mul_scalar();
void trans_mat();

int stop();
void error();

typedef struct {
    char *name;
    void (*func)();
} mat_cmd;