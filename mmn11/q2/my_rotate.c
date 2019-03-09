#include <stdio.h>

int howManyBits();
void printBin(unsigned long int);

unsigned short my_rotate(unsigned short, char);

int main() {
    unsigned short a;
    char offset;
    printf("\n Enter Number and offset: ");
    scanf("%h%c", &a, &offset);

    printf("%d,%d)", a, offset);
    // printBin(num);
}

unsigned short my_rotate(unsigned short a, char offset){
    return 4;
}

void printBin(unsigned long int x){
    unsigned int mask = 1 << ( howManyBits() - 1 );

    while( mask ){
        if( (x&mask) == 0)
            printf("0");
        else
            printf("1");
        mask >>= 1;
    }
}

int howManyBits(){
    return sizeof(unsigned long int)*8;
}