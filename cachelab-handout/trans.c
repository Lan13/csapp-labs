/* Author: Junwei Lan
 * StudentID: PB20111689
 * Created Date: 2023.4.10
 * Updated Date: 2023.5.29
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
    if (M == 32 && N == 32) {
        int v0, v1, v2, v3, v4, v5, v6, v7;
        for (int i = 0; i < N; i += 8) {
            for (int j = 0; j < M; j += 8) {
                for (int k = i; k < i + 8; k++) {
                    v0 = A[k][j];
                    v1 = A[k][j + 1];
                    v2 = A[k][j + 2];
                    v3 = A[k][j + 3];
                    v4 = A[k][j + 4];
                    v5 = A[k][j + 5];
                    v6 = A[k][j + 6];
                    v7 = A[k][j + 7];
                    B[j][k] = v0;
                    B[j + 1][k] = v1;
                    B[j + 2][k] = v2;
                    B[j + 3][k] = v3;
                    B[j + 4][k] = v4;
                    B[j + 5][k] = v5;
                    B[j + 6][k] = v6;
                    B[j + 7][k] = v7;
                }
            }
        }
        return;
    }
    if (M == 64 && N == 64) {
        int v0, v1, v2, v3, v4, v5, v6, v7;
        for (int i = 0; i < N; i += 8) {
            for (int j = 0; j < M; j += 8) {
                for (int k = i; k < i + 4; k++) {
                    v0 = A[k][j];
                    v1 = A[k][j + 1];
                    v2 = A[k][j + 2];
                    v3 = A[k][j + 3];
                    v4 = A[k][j + 4];
                    v5 = A[k][j + 5];
                    v6 = A[k][j + 6];
                    v7 = A[k][j + 7];

                    // a 顺序存放
                    B[j][k] = v0;
                    B[j + 1][k] = v1;
                    B[j + 2][k] = v2;
                    B[j + 3][k] = v3;

                    // b 逆序存放
                    B[j][k + 4] = v7;
                    B[j + 1][k + 4] = v6;
                    B[j + 2][k + 4] = v5;
                    B[j + 3][k + 4] = v4;
                }
                for (int k = 0; k < 4; k++) {
                    // c 按列读取
                    v0 = A[i + 4][j + 3 - k];
                    v1 = A[i + 5][j + 3 - k];
                    v2 = A[i + 6][j + 3 - k];
                    v3 = A[i + 7][j + 3 - k];
                    // d 按列读取
                    v4 = A[i + 4][j + 4 + k];
                    v5 = A[i + 5][j + 4 + k];
                    v6 = A[i + 6][j + 4 + k];
                    v7 = A[i + 7][j + 4 + k];

                    // b' 将错位逆序顺序换回
                    B[j + 4 + k][i] = B[j + 3 - k][i + 4];
                    B[j + 4 + k][i + 1] = B[j + 3 - k][i + 5];
                    B[j + 4 + k][i + 2] = B[j + 3 - k][i + 6];
                    B[j + 4 + k][i + 3] = B[j + 3 - k][i + 7];

                    // c -> c'
                    B[j + 3 - k][i + 4] = v0;
                    B[j + 3 - k][i + 5] = v1;
                    B[j + 3 - k][i + 6] = v2;
                    B[j + 3 - k][i + 7] = v3;
                    // d -> d'
                    B[j + 4 + k][i + 4] = v4;
                    B[j + 4 + k][i + 5] = v5;
                    B[j + 4 + k][i + 6] = v6;
                    B[j + 4 + k][i + 7] = v7;
                }
            }
        }
        return;
    }
    if (M == 61 && N == 67) {
        int v0, v1, v2, v3, v4, v5, v6, v7;
        // 分割成3块，64x56，64x5，3x61
        for (int i = 0; i + 8 < N; i += 8) {
            for (int j = 0; j + 8 < M; j += 8) {
                for (int k = i; k < i + 8; k++) {
                    v0 = A[k][j];
                    v1 = A[k][j + 1];
                    v2 = A[k][j + 2];
                    v3 = A[k][j + 3];
                    v4 = A[k][j + 4];
                    v5 = A[k][j + 5];
                    v6 = A[k][j + 6];
                    v7 = A[k][j + 7];
                    B[j][k] = v0;
                    B[j + 1][k] = v1;
                    B[j + 2][k] = v2;
                    B[j + 3][k] = v3;
                    B[j + 4][k] = v4;
                    B[j + 5][k] = v5;
                    B[j + 6][k] = v6;
                    B[j + 7][k] = v7;
                }
                
            }
        }
        for (int ii = 0; ii < 64; ii++) {
            v0 = A[ii][56];
            v1 = A[ii][57];
            v2 = A[ii][58];
            v3 = A[ii][59];
            v4 = A[ii][60];
            B[56][ii] = v0;
            B[57][ii] = v1;
            B[58][ii] = v2;
            B[59][ii] = v3;
            B[60][ii] = v4;
        }
        for (int jj = 0; jj < M; jj++) {
            v0 = A[64][jj];
            v1 = A[65][jj];
            v2 = A[66][jj];
            B[jj][64] = v0;
            B[jj][65] = v1;
            B[jj][66] = v2;
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

