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
 *              PBS Usage: - qsub -v T=8 batch.mat-mat                                  // submit job, where T is # of threads
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
                 result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    return 0;
}

/* Main program starts here */
int main(int argc, char *argv[])
{
    int a = 1.0;
    int i, j;

    struct timeval start, end;
    gettimeofday(&start, NULL);                         /* start measuring time */

    if (argc != 2) {
       fprintf( stderr, "%s <number of threads>\n", argv[0] );
       return -1;
    }

    NUM_THREADS = atoi( argv[1] );

    /* Dynamic memory allocation for Matrices*/
    /* Allocate memory for matrix rows and columns = 1000 X 1000 */

    /* Allocating memory to rows of all 3 matrices */
    mat1 = (double **) malloc(SIZE * sizeof(double *));
    mat2 = (double **) malloc(SIZE * sizeof(double *));
    result = (double **) malloc(SIZE * sizeof(double *));

    /* Allocating memory to cols of all 3 matrices */
    for (i=0;i<SIZE;i++){
        mat1[i] = (double *) malloc(SIZE * sizeof(double));
        mat2[i] = (double *) malloc(SIZE * sizeof(double));
        result[i] = (double *) malloc(SIZE * sizeof(double));
    }

    /* Generating matrix elements with random numbers between 0 and 1 */
    srand(time(NULL));
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            mat1 [i][j] = (double) rand()/ (double) (RAND_MAX/a);
            mat2 [i][j] = (double) rand()/ (double) (RAND_MAX/a);
        }
    }

    /* Declare the threads */
    pthread_t threads[NUM_THREADS];
    
   int *param  = (int *)malloc(sizeof(int));
   for (i = 0; i < NUM_THREADS; ++i)
       pthread_create(&threads[i], NULL, multiplication, (void *)param);
   }

   /* Join threads */
   for (i = 0; i < NUM_THREADS; ++i){
       pthread_join(threads[i], NULL);
   }

   /* Free the allocated memory from matrices */
   for (i = 0; i < SIZE; i++){
       free(mat1[i]);
       free(mat2[i]);
       free(result[i]);
   }

   free(mat1);
   free(mat2);
   free(result);
   free(param);

   mat1 = NULL;
   mat2 = NULL;
   result = NULL;

   gettimeofday(&end, NULL);    /* function to measure execution time */

   /* Print the time elapsed */
   printf("\n Time Elapsed: %fs \n", ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0));

   return 0;
} 
