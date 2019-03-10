#include <stdio.h>

int howManyBits();
void printBin(unsigned short int);

unsigned short my_rotate(unsigned short, char);

int main() {
    unsigned short a;
    int b;

    printf("\n Enter number: ");
    scanf("%hu", &a);

    printf("\n How much to rotate: ");
    scanf("%d",&b);

    printf("\n Before: ");  
    printBin(a);
    printf("\n After:  ");

    printBin( my_rotate( a, (char)b ) );
    printf("\n");

    return 0;
}

unsigned short my_rotate(unsigned short a, char b){
    unsigned int maskLeft = 1 << ( howManyBits() - 1 ); /* 10...0 */
    unsigned int maskRight = 1; /* 0...01 */

    int offset = (int)b;
    int temp;

    while ( offset ) {
        if ( offset > 0 ) {
            /* get the left most bit */
            temp = ( (a&maskLeft) == 0 ) ? 0 : 1;
            a <<= 1;
            offset--;

            /* if the bit was 1, change the inserted bit to 1
            else do nothing, defualt is 0 */
            if( temp == 1 ){
                a ^= maskRight;
            }
        } else{
            /* get the right most bit */
            temp = ( (a&maskRight) == 0 ) ? 0 : 1;
            a >>= 1;
            offset++;

            if( temp == 1 ){
                a ^= maskLeft;
            }
        }
    }
    
    return a;
}

void printBin(unsigned short int x){
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
    return sizeof(unsigned short int)*8;
}