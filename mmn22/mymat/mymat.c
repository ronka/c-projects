#include "mat.h"

mat_cmd cmd[] = {
    {"read_mat", read_mat},
    {"print_mat", print_mat},
    {"add_mat", add_mat},
    {"sub_mat", sub_mat},
    {"mul_mat", mul_mat},
    {"mul_scalar", mul_scalar},
    {"trans_mat", trans_mat},
};

int main(){
    int i = 0;
    enum flag state = TRUE;
    char command[80];

    printf("\nWelcome to The Matrix calculator, the following commands are supported: \n");
    printf("read_mat,print_mat,add_mat,sub_mat,mul_mat,mul_scalar,trans_mat,stop \n");

    while ( state ){
        printf("\n\n Enter a function: \n");
        scanf("%s", command);
        
        state = stop();
        printf("\n %s", &command);
    }

    return 0;
}
