#include <stdio.h>

#define N 11 /* size of matrix */

enum foundPath { FALSE, TRUE }; /* if path is found */

typedef int ADJMAT[N][N];

int path(ADJMAT, int, int);
int pathRec(ADJMAT, int, int, int);
void printMat(ADJMAT);
void printResult(ADJMAT, int, int);

int main(){
    ADJMAT A;
    int u, v,startNode, endNode;

    printf("\n Enter adjacency matrix %dX%d,",N, N);
    
    for ( u=0 ; u<N ; u++ ){
        printf("\n - Row %d: ", u);
        
        for( v=0 ; v<N ; v++ ){
            scanf("%d", &A[u][v]);    
        }
    }
    printf("\n\n");

    printf("\n Your matrix: \n");
    printMat( A );
    printf("\n\n");

    printf("\n enter -1 -1 as nodes to stop \n");

    while( getchar() != EOF ){
        printf("\n Check path between 2 nodes:");
        scanf("%d %d", &startNode, &endNode);
        if( startNode < 0 || endNode < 0 ){
            break;
        }
        printResult( A, startNode ,endNode );
    }

    /* input 4x4 */
    /*
    printResult( A, 0, 2 );
    printResult( A, 1, 3 );
    printResult( A, 3, 2 );
    */

    printf("\n\n");
    return 0;
}

void printResult( ADJMAT A, int u, int v ){;
    switch( path(A,u,v) ){
        case TRUE:
            printf("\n path(A,%d,%d): TRUE", u, v);
            break;
        case FALSE:
            printf("\n path(A,%d,%d): FALSE", u, v);
            break;
        default:
            printf("\n Error: unkown situtation");
            break;
    }
}

int path(ADJMAT A, int u, int v){
    /* edge case, if non exisiting nodes or path from node to self */
    if( (u > N || u < 0) || (v > N || v < 0) ) { return FALSE; }
    if( u == v ) { return TRUE; }

    /* the path is check recursively */
    return pathRec( A, u, 0 ,v );
}

int pathRec(ADJMAT A, int u, int v, int dest){
    /* if out of boundaries */
    if( u > N || v > N ) { return FALSE; }
    /*
    if u is the parent of v,
    and v node is the destination node,
    there is a path and return TRUE
    */
    if( A[u][v] == 1 && v == dest ) { return TRUE; }

    /*
    if u is the parent of v,
    and v is not the desination node
    */

    if( A[u][v] == 1 ){
        /*
        chcek if the destination node is on of v children,
        or check the other u's children node
        */
        return pathRec(A,v,0,dest) || pathRec(A,u,v+1,dest);
    }

    /*
    if u is not the parent of v,
    check the if the next node is u child node
    */
    return pathRec(A,u,v+1,dest);
}

void printMat(ADJMAT A){
    int u, v;
    
    /* print matrix header */
    printf("\n [   ");
    for ( u=0 ; u<N ; u++ ){
        printf(" %d ", u);
        if( u != N-1 ) { printf("|"); }   
    }
    printf("]");

    /* print matrix header body divider */
    printf("\n [  ");
    for ( u=0 ; u<N ; u++ ){
        printf("----");
    }
    printf("]");

    /* print matrix content */
    for ( u=0 ; u<N ; u++ ){
        /* print row number */
        printf("\n [ %d:", u);
        
        /* print row content */
        for( v=0 ; v<N ; v++ ){
            printf(" %d ", A[u][v]);
            if( v != N-1 ) { printf("|"); }   
        }
        printf("]"); 
    }
}