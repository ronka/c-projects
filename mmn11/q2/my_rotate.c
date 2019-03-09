#include <stdio.h>

int howManyBits();
void printBin(unsigned long int);

int main() {
    int num, cnt;
    printf("\n Enter Number: ");
    scanf("%d", &num);

    printBin(num);
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