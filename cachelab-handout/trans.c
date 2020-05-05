/* 
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

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
   int i,j,k;
   int temp_value0;
   int temp_value1;
   int temp_value2;
   int temp_value3;
   int temp_value4;
   int temp_value5;
   int temp_value6;
   int temp_value7;       

   if(N == 32 && M == 32){
   for(i = 0; i < N; i += 8){
		for(j = 0; j < M; j += 8){
			for(k = i ; k < i + 8; k++){
			temp_value0 = A[k][j];
            temp_value1 = A[k][j+1];
            temp_value2 = A[k][j+2];
            temp_value3 = A[k][j+3];
            temp_value4 = A[k][j+4];
            temp_value5 = A[k][j+5];
            temp_value6 = A[k][j+6];
            temp_value7 = A[k][j+7];
          
            B[j][k] = temp_value0;
            B[j+1][k] = temp_value1;
            B[j+2][k] = temp_value2;
            B[j+3][k] = temp_value3;
            B[j+4][k] = temp_value4;
            B[j+5][k] = temp_value5;
            B[j+6][k] = temp_value6;
            B[j+7][k] = temp_value7;	
			}	
		}
	}
	}
   else if(N == 64 && M == 64){
        for(i = 0; i < N; i+=8){
			 for(j = 0; j < M; j+=8){
			 	//up half of 8*8 matrix
				 for(k = i;k < i + 4; k++){
					temp_value0 = A[k][j];
					temp_value1 = A[k][j+1];
					temp_value2 = A[k][j+2];
					temp_value3 = A[k][j+3];
					temp_value4 = A[k][j+4];
					temp_value5 = A[k][j+5];
					temp_value6 = A[k][j+6];
					temp_value7 = A[k][j+7];
					//top left block line,downward direction
					B[j][k] = temp_value0;
					B[j+1][k] = temp_value1;
					B[j+2][k] = temp_value2;
					B[j+3][k] = temp_value3;
					//top right block lint,upward direction
					B[j][k+4] = temp_value4;
					B[j+1][k+4] = temp_value5;
					B[j+2][k+4] = temp_value6;
					B[j+3][k+4] = temp_value7;
				}
				//down half of 8*8 matrix
				for(k = 0; k < 4; k++ ){
					temp_value0 = A[i+4][j+k];
					temp_value1 = A[i+5][j+k];
					temp_value2 = A[i+6][j+k];
					temp_value3 = A[i+7][j+k];
					temp_value4 = B[j+k][i+4];
					temp_value5 = B[j+k][i+5];
					temp_value6 = B[j+k][i+6];
					temp_value7 = B[j+k][i+7];
					B[j+k][i+4] = temp_value0;
					B[j+k][i+5] = temp_value1;
					B[j+k][i+6] = temp_value2;
					B[j+k][i+7] = temp_value3;
					B[j+4+k][i] = temp_value4;
					B[j+4+k][i+1] = temp_value5;
					B[j+4+k][i+2] = temp_value6;
					B[j+4+k][i+3] = temp_value7;

				}
				for(k = i+4; k < i+8; k++){
					temp_value0 = A[k][j+4];
					temp_value1 = A[k][j+5];
					temp_value2 = A[k][j+6];
					temp_value3 = A[k][j+7];
					B[j+4][k] = temp_value0;
					B[j+5][k] = temp_value1;
					B[j+6][k] = temp_value2;
					B[j+7][k] = temp_value3;
				}
			 }
		}   
   
   
   
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
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
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

