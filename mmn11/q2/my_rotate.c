#include <stdio.h>

int howManyBits();
void printBin(unsigned short int);

unsigned short my_rotate(unsigned short, char);

int main() {
    unsigned short a;
    char b;

    printf("\n Enter number: ");
    scanf("%hu", &a);

    printf("\n How much to rotate: ");
    scanf(" %c",&b);


    printf("(%d,%c)\n", a, b);

    printBin(b - '0');
    printf("\n");
    printBin(a);
    printf("\n");
    printf("\n");

    printBin(my_rotate(a,b));
    printf("\n");

    /* printBin(num); */

    return 0;
}

unsigned short my_rotate(unsigned short a, char b){
    unsigned int maskLeft = 1 << ( howManyBits() - 1 ); /* 10...0 */
    unsigned int maskRight = 1; /* 0...01 */

    unsigned int mask;

    int offset = b - '0';
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

        

        printf("\n %d \n",temp);
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