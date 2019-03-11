#include <stdio.h>

int how_many_bits();
void print_short_bin(unsigned short int);

unsigned short my_rotate(unsigned short, char);

int main() {
    unsigned short a, result;
    char b;

    printf("\n Enter number: ");
    scanf("%hu", &a);

    printf("\n How much to rotate: ");
    scanf("%hhd",&b);

    result = my_rotate( a, b );

    printf("\n result: %d\n", result);

    printf("\n Before: ");
    print_short_bin(a);
    printf("\n After:  ");
    print_short_bin( result );
    printf("\n\n");

    return 0;
}

unsigned short my_rotate(unsigned short a, char b){
    unsigned int maskLeft = 1 << ( how_many_bits() - 1 ); /* 10...0 */
    unsigned int maskRight = 1; /* 0...01 */

    int offset = b;
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

void print_short_bin(unsigned short int x){
    unsigned int mask = 1 << ( how_many_bits() - 1 );

    while( mask ){
        if( (x&mask) == 0){
            printf("0");
        } else{
            printf("1");
        }
        mask >>= 1;
    }
}

/* calculate number of bits */
int how_many_bits(){
    return sizeof(unsigned short int)*8;
}