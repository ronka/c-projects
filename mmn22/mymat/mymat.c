#include "mat.h"

cmd_list cmds[] = {
    {"read_mat", read_mat},
    {"print_mat", print_mat},
    {"add_mat", add_mat},
    {"sub_mat", sub_mat},
    {"mul_mat", mul_mat},
    {"mul_scalar", mul_scalar},
    {"trans_mat", trans_mat},
    {"stop", trans_mat}
};

int main(){
    int i;
    enum flag state = TRUE;
    char command[80];

    printf("\nWelcome to The Matrix Calculator, the following commands are supported: \n");
    printf("read_mat,print_mat,add_mat,sub_mat,mul_mat,mul_scalar,trans_mat,stop \n");

    while ( state ){
        printf("\n Enter a function: \n");
        scanf("%s", command);

        for (i=0 ; i < 8; i++){
            /*
            search the command in the list of commands,
            if found, run the function and break the for loop.
            */
            if( i < 8 && !strcmp( command, cmds[i].name )){
                if( i == 7 ){
                    state = FALSE;
                } else{
                    (*cmds[i].func)();
                }
                break;
            }
        }

        /* if index reached 8 the command is not in the list */
        if( i == 8 ){
            error("Invalid command, please try again");
        }
        
    }

    return 0;
}
