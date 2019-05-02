#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 4
#define LENGTH 100

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

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

void error(char[]);
char *clean_spaces(char *);
int is_mat(char[]);

typedef struct {
    char *name;
    void (*func)();
} cmd_list;

typedef struct {
    char *name;
    mat *address;    
} mat_list;