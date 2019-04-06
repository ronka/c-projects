#include <stdio.h>

int howManyBits();
int countOddBits(unsigned long int);

int main() {
    int num, cnt;
    printf("\n Enter Number: ");
    scanf("%d", &num);
    cnt = countOddBits(num);
    printf("\n Bits ON in odd places: %d \n", cnt);

    return 0;
}

int countOddBits(unsigned long int num){
    unsigned int mask = 1;
    unsigned int cnt = 0, index = 0;

    while( mask ){
        if( ((num&mask) != 0) && (index%2 == 1) ){
            cnt++;
        }
        index++;
        mask <<= 1;
    }

    return cnt;
}