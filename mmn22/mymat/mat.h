#include <stdio.h>
#include <string.h>

#define SIZE 4
#define LENGTH 100

typedef double mat[SIZE][SIZE];

mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F;

enum flag { FALSE, TRUE };

void read_mat(void);
void print_mat(void);
void add_mat(void);
void sub_mat(void);
void mul_mat(void);
void mul_scalar(void);
void trans_mat(void);
int stop();
void error(void);

typedef struct {
    char *name;
    void (*func)(void);
} mat_cmd;

mat_cmd cmd[] = {
    {"read_mat", read_mat},
    {"print_mat", print_mat},
    {"add_mat", add_mat},
    {"sub_mat", sub_mat},
    {"mul_mat", mul_mat},
    {"mul_scalar", mul_scalar},
    {"trans_mat", trans_mat},
    {"stop", stop},
    {"error", error}
};