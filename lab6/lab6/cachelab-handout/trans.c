//Jake Campos
//Jonathan Ward
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
	int diag = 0;
	int t = 0;
	int inv = 0;
	int bc, br;
	int bs = 0;

	if(M == 32 && N == 32)
	{
// blocksize 4 = 439, 8 = 287, 16 = 1141 
		bs = 8;
		for(br = 0; br < 32; br+= bs)
		{
			for(bc = 0; bc < 32; bc+= bs)
			{
				for(int p = br; p < br + bs; p++)
				{
					for(int q = bc; q < (bc + bs); q++)
					{
						if(p != q)
							B[q][p] = A[p][q];
						else
						{
							t = A[p][q];
							diag = p;
							inv = 1;
						}							
					}
					if(inv == 1)
					{
						B[diag][diag] = t;
						inv = 0;
					}
				}
			}
		}
		return;
	}
	if(M == 64 && N == 64)
	{
// blocksize 2 = 2835, 4 = 1735, 8 = 4635, 16 = 4651  
		bs = 4;
		for(br = 0; br < 64; br+= bs)
		{
			for(bc = 0; bc < 64; bc+= bs)
			{
				for(int p = br; p < br + bs; p++)
				{
					for(int q = bc; q < (bc + bs); q++)
					{
						if(p != q)
							B[q][p] = A[p][q];
						else
						{
							t = A[p][q];
							diag = p;
							inv = 1;
						}							
					}
					if(inv == 1)
					{
						B[diag][diag] = t;
						inv = 0;
					}
				}
			}
		}
		return;
	}
	if(M == 61 && N == 67)
	{
// blocksize 4 = 2425, 8 = 2118, 16 = 1992, 32 = 2590
		bs = 16;
		for(br = 0; br < 67; br+= bs)
		{
			for(bc = 0; bc < 61; bc+= bs)
			{
				for(int p = br; p < br + bs; p++)
				{
					for(int q = bc; q < (bc + bs); q++)
					{
						if(q >= 61 || p >= 67)
							continue;
						else
						{
							B[q][p] = A[p][q];
						}							
					}
				}
			}
		}
		return;
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

