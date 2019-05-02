#include "mat.h"

mat_list mats[]={ 
    {"MAT_A",&MAT_A},
    {"MAT_B",&MAT_B},
    {"MAT_C",&MAT_C},
    {"MAT_D",&MAT_D},
    {"MAT_E",&MAT_E},
    {"MAT_F",&MAT_F}  
};

void error(char *error){
    printf(RED "\n %s \n" RESET, error);
}

int is_mat(char *mat_name){
    int i;

    for (i=0 ; i < 6; i++){
        if(!strcmp( mat_name, mats[i].name )){
            return i;
        }
    }

    return -1;
}