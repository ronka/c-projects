#include <stdio.h>

#define N 4 /* size of matrix 8 */

enum foundPath { FALSE, TRUE }; /* if path is found */

typedef int ADJMAT[N][N];

void printMat(ADJMAT);
int path(ADJMAT, int, int);
int path_rec(ADJMAT, int, int, int);

int main(){
    ADJMAT A;
    int u, v;

    printf("\n Enter adjacency matrix %dX%d,",N, N);
    
    for ( u=0 ; u<N ; u++ ){
        printf("\n - Row number %d: ", u);
        
        for( v=0 ; v<N ; v++ ){
            scanf("%d", &A[u][v]);    
        }
    }

    /*
     * Print all elements of array
     */
    printf("\nElements in array are: ");
    printMat(A);

    return 0;
}

int path(ADJMAT A, int u, int v){
    if( u > N || v > N ) { return FALSE; }
    if( u == v ) { return TRUE; }

    return path_rec( A, u, 0 ,v );
}

int path_rec(ADJMAT A, int u, int v, int dest){
    if( u > N || v > N ) { return FALSE; }
    if( A[u][v] == 1 && v == dest ) { return TRUE; }
    return TRUE;
}

void printMat(ADJMAT A){
    int u, v;
    for ( u=0 ; u<N ; u++ ){
        printf("\n - Row number %d: ", (u + 1));
        
        for( v=0 ; v<N ; v++ ){
            printf("%d, ", A[u][v]);    
        }

        printf("\n\n"); 
    }
}