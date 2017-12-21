/*Cachelab assignment for CS-2011 B'17
 * Written by Surya Vadivazhagu and Daniel McDonough
 *  
 *
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void trans32(int M, int N, int A[N][M], int B[M][N]);
void trans64(int M, int N, int A[N][M], int B[M][N]);
void transother(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission"; //do not change
void transpose_submit(int M, int N, int A[N][M], int B[M][N]){
	//If test is 32x32 then run the 32 function
	if(N == 32){
		trans32(M,N,A,B);
	}//if test is 6464 then run the 64 function
	else if(N == 64){
		trans64(M,N,A,B);
	}
	else{//if the test is  61x67 or anything else run this function
		transother(M,N,A,B);
	}


}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
	int i, j, tmp;

	for (i = 0; i < N; i++) {
		for (j = 0; j < M; j++) {
			tmp = A[i][j];
			B[j][i] = tmp;
		}
	}    

}

/* 
 * trans32 - A transpose function, optimized for the cache for array 32x32 
 */
char trans_desc32[] = "32x32 transpose";
void trans32(int M, int N, int A[N][M], int B[M][N]){
	int i, j, j1,i1, tmp, loc;
	
	int b = 8; //cache block size
	
	//move through whole array by increments of bxb cache size
	for (i = 0; i < N; i+=b) {
		for (j = 0; j < M; j+=b) {
			
			//move through the bxb block
			for (i1 = i; i1 < i+b; i1++){
				for (j1 = j; j1 < j+b; j1++){
					
					//check if diagonal, to save time store it in a local var
					if(j1 == i1){
						tmp = A[i1][j1]; //value of diagonal
						loc = i1; //location of diagonal
					}
					else{
						//translate value
						B[j1][i1] = A[i1][j1];
					}
				}
				
				if(i == j){
					//if diagonal translate the value
					B[loc][loc] = tmp;
				}
			}
		}
	}    
}
/* 
 * transother - A transpose function, optimized for the cache to work for 61x67 array.
 */
char trans_descother[] = "MXN transpose";
void transother(int M, int N, int A[N][M], int B[M][N]){
	int i, j, j1,i1, tmp, loc;
	int b = 16; //cache block size
	//move through whole array by increments of bxb cache size
	for (i = 0; i < N; i+=b) {
		for (j = 0; j < M; j+=b) {
			//move through the bxb block, such that the pointer doesnt go over N or M
			for (i1 = i; ((i1 < i+b) && (i1 < N)); i1++){
				for (j1 = j; ((j1 < j+b) && (j1 < M)); j1++){
					//check if diagonal, to save time store it in a local var
					if(j1 == i1){
						tmp = A[i1][j1]; //value of diagonal
						loc = i1; //location of diagonal
					}
					else{
						//translate value
						B[j1][i1] = A[i1][j1];
					}
				}
				if(i == j){
					//if diagonal translate the value
					B[loc][loc] = tmp;
				}
			}
		}
	}    
}
/* 
 * trans64 - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc64[] = "64x64 transpose";
void trans64(int M, int N, int A[N][M], int B[M][N]){
// Trans 64 by Vo Tran Thanh Luong

int colRun, rowRun, k, a0, a1, a2, a3, a4, a5, a6, a7; // supporting variables

    // two loops to go through each row and column block 
    for(colRun=0; colRun<64; colRun+=8 ){
       	for(rowRun=0; rowRun<64; rowRun+=8 ){

        /* 
        The first loop is for a 4 collumn 8 row of A. In this loop we  use the supporting variables to store all elements of a row. Then we try to transpose with the right positions in B. However, we are doing on a 4row x 8 collumn B to avoid cache miss so not all elements will be transposed correctly. For example A[3][5] cannot be transposed to B[5][3]. Thus, elements which can't be transposed correctly will be stored in another location for later use.
        */ 
        	for(k=0; k<4; k++){ 
        		a0 = A[colRun+k][rowRun+0];
        		a1 = A[colRun+k][rowRun+1];
        		a2 = A[colRun+k][rowRun+2];
        		a3 = A[colRun+k][rowRun+3];
        		a4 = A[colRun+k][rowRun+4];
        		a5 = A[colRun+k][rowRun+5];
        		a6 = A[colRun+k][rowRun+6];
        		a7 = A[colRun+k][rowRun+7];

        		// In the code, I comment "Good job" for the elements that are transposed correctly. "Later use" for later assignment
        		B[rowRun+0][colRun+k+0] = a0;   // good job
        		B[rowRun+0][colRun+k+4] = a5;	// later use
        		B[rowRun+1][colRun+k+0] = a1;	// good job
        		B[rowRun+1][colRun+k+4] = a6;	//later use
        		B[rowRun+2][colRun+k+0] = a2;	// good job
        		B[rowRun+2][colRun+k+4] = a7;	//later use
        		B[rowRun+3][colRun+k+0] = a3;	// good job
        		B[rowRun+3][colRun+k+4] = a4;	// later use
        	}


            /* Part B, moving sub-matrix b to sub-matrix c
             * and moving A->B for sub-matrix b and move matrix d
             */
            /*
            Now that we have dealt with the first 4 col 8 arrow of A. The next job to deal with the "later use" assignment above. The "later use" assignments that we did above have taken a lot of places, so we need to bring these elements to their right positions. 
             */
        	a0 = A[colRun+4][rowRun+4];
        	a1 = A[colRun+5][rowRun+4];
        	a2 = A[colRun+6][rowRun+4];
        	a3 = A[colRun+7][rowRun+4];
        	a4 = A[colRun+4][rowRun+3];
        	a5 = A[colRun+5][rowRun+3];
        	a6 = A[colRun+6][rowRun+3];
        	a7 = A[colRun+7][rowRun+3];


        	B[rowRun+4][colRun+0] = B[rowRun+3][colRun+4];  // B[4][0] = a4 = A[0][4] For example
        	B[rowRun+4][colRun+4] = a0;  // B[4][4] = A[4][4] For example
        	B[rowRun+3][colRun+4] = a4;
        	B[rowRun+4][colRun+1] = B[rowRun+3][colRun+5];
        	B[rowRun+4][colRun+5] = a1;
        	B[rowRun+3][colRun+5] = a5;
        	B[rowRun+4][colRun+2] = B[rowRun+3][colRun+6];
        	B[rowRun+4][colRun+6] = a2;
        	B[rowRun+3][colRun+6] = a6;
        	B[rowRun+4][colRun+3] = B[rowRun+3][colRun+7];
        	B[rowRun+4][colRun+7] = a3;
        	B[rowRun+3][colRun+7] = a7;

        	// this loops deal with the the remaning elements .
        	for(k=0;k<3;k++){


        		a0 = A[colRun+4][rowRun+5+k];
        		a1 = A[colRun+5][rowRun+5+k];
        		a2 = A[colRun+6][rowRun+5+k];
        		a3 = A[colRun+7][rowRun+5+k];
        		a4 = A[colRun+4][rowRun+k];
        		a5 = A[colRun+5][rowRun+k];
        		a6 = A[colRun+6][rowRun+k];
        		a7 = A[colRun+7][rowRun+k];


        		B[rowRun+5+k][colRun+0] = B[rowRun+k][colRun+4];
        		B[rowRun+5+k][colRun+4] = a0;
        		B[rowRun+k][colRun+4] = a4;
        		B[rowRun+5+k][colRun+1] = B[rowRun+k][colRun+5];
        		B[rowRun+5+k][colRun+5] = a1;
        		B[rowRun+k][colRun+5] = a5;
        		B[rowRun+5+k][colRun+2] = B[rowRun+k][colRun+6];
        		B[rowRun+5+k][colRun+6] = a2;
        		B[rowRun+k][colRun+6] = a6;
        		B[rowRun+5+k][colRun+3] = B[rowRun+k][colRun+7];
        		B[rowRun+5+k][colRun+7] = a3;
        		B[rowRun+k][colRun+7] = a7;


        	}


        }
} 
}


/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions(){

	/* Register your solution function */
	registerTransFunction(transpose_submit, transpose_submit_desc); 

	/* Register any additional transpose functions */
	registerTransFunction(trans, trans_desc); 
	registerTransFunction(trans32, trans_desc32); 
	registerTransFunction(transother, trans_descother); 
	registerTransFunction(trans64, trans_desc64); 
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N]){

	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < M; ++j) {
			if (A[i][j] != B[j][i]) {
				return 0;
			}
		}
	}
	return 1;
}

