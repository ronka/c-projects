#include "mat.h"

mat_list mats[]={ 
    {"MAT_A",&MAT_A},
    {"MAT_B",&MAT_B},
    {"MAT_C",&MAT_C},
    {"MAT_D",&MAT_D},
    {"MAT_E",&MAT_E},
    {"MAT_F",&MAT_F}  
};

void read_mat(){
    char input[LENGTH], *token, *mat_name;
    int mat_i, u = 0, v = 0, x, y;

    fgets (input , LENGTH , stdin);

    if( *input == '\n' ){
        error("No matrix name enterd");
        return;
    }

    mat_name = clean_spaces(strtok(input, ",")); /* get matrix name */

    if( isdigit(mat_name[strlen(mat_name)-1]) ){
        error("No comma after matrix name");
        return;
    }

    mat_i = is_mat(mat_name);

    if( mat_i < 0 ){
        error("Undefined matrix name");
        return;
    }

    token = clean_spaces(strtok(NULL, ","));
    while( u < SIZE ){
        mats[ mat_i ].address[0][u][v] = 1;/* (token == NULL) ? strtod(token) : 1;*/
        if( v < 4 ){
            v++;
        } else{
            v = 0;
            u++;
        }
        token = clean_spaces(strtok(NULL, ","));
    }

    for(y=0;y<(SIZE) ;y++)
        for(x=0;x<(SIZE) ;x++)        
                printf("%f,",mats[mat_i].address[0][y][x]);
}
void print_mat(){
    printf("print_mat");
}
void add_mat(){
    printf("add_mat");
}
void sub_mat(){
    printf("sub_mat");
}
void mul_mat(){
    printf("mul_mat");
}
void mul_scalar(){
    printf("mul_scalar");
}
void trans_mat(){
    printf("trans_mat");
}

/* Helpers */

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

char *clean_spaces(char *str){
    int last_char_index;
    if( str == NULL || strlen(str) < 2 ) {
        return str;
    }

    last_char_index = strlen(str) - 2;

    while( str[0] == ' ' || str[0] == '\t' ){
        str++;
    }

    while( str[last_char_index] == ' ' || str[last_char_index] == '\t' ){
        str[last_char_index] = '\0';
        last_char_index--;
    }
    
    return str;
}