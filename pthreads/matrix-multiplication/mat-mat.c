/*
 * File Name: mat-mat.c
 * Author: Pradeep Singh
 * Date : 2/27/2018
 * Description: Matrix - Matrix Multiplications using Pthreads.
 *              This program generate two matrices with random numbers between 0 and 1 using function srand()
 *              and rand(). We use malloc() to dynamically allocate memory for all the matrices. And, finally
 *              product of two matrices is computed using Pthreads (1,2,4,8,16,32) and result is stored in result matrix.
 *              Once computation is done, we de-allocate the memory from all the matrices using free() and speedup between
 *              different threads is measured and reported.
 *
 *              Program usage: - gcc -o mat-mat mat-mat.c                               // compile
 *                             - ./mat-mat                                              // run
 *                             - time ./mat-mat                                         // run with time function
 *                             - valgrind --tool=memcheck --leak-check=yes ./mat-mat    // check memory leaks
 *                             - gcc -Wall -Werror -o mat-mat mat-mat.c                 // check warnings and error
 *                             - gcc -O1 mat-mat.c -o mat-mat                           // optimization,
 *                                                                                      // replace -O1 with -O2 and - O3
 *
 *              PBS Usage: - qsub batch.mat-mat                                         // submit job
 *                         - qstat                                                      // check job status
 *                         - cat mat-mat.job_id                                         // check o/p
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>

// define matrix size
#define SIZE 3200

double **mat1, **mat2, **result;
int NUM_THREADS;

/* function to multiply two matrices */
void* multiplication(void* arg)
{
    int i, j, k;
    int slice, divide, row_start, row_end;
    divide = SIZE / NUM_THREADS;
    slice = *(int *)(arg);
    row_start = slice * divide;
    row_end = (slice+1) * divide;

    for (i = row_start; i < row_end; ++i) {
        for (j = 0; j < SIZE; ++j) {
            for (k = 0; k < SIZE; ++k) {
