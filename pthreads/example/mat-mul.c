/*
 * File Name: mat-mat.c
 * Author: Pradeep Singh
 * Date : 2/7/2018
 * Description: Matrix - Matrix Multiplication
 *              This program generate two 1000 X 1000 matrices with random numbers between 0 and 1.
 *              using function srand() and rand(). We use malloc() to dynamically allocate memory for
 *              all the matrices. And, finally product of two matrices is computed using nested for loops
 *              and result is stored in result matrix. Once computation is done, we de-allocate the memory
 *              from all the matrices using free().
 *
 *  Usage: - gcc -o mat-mat mat-mat.c -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

// Number of threads
#define NUM_THREADS 1
#define SIZE 5

void* multiplication(void* arg)
{
   int i,j,k;
   float **mat1, **mat2, **result;

   int divide = SIZE / NUM_THREADS;

   for (i = divide; i < (divide + 1); i++){
       for (j = 0; j <SIZE ; j++){
           for (k = 0; k < SIZE; k++){
                result[i][j] = 0.1;
                result[i][j] +=  mat1[i][j] * mat2[k][j];
           }
       }
   }
return 0;
}


int main()
{

   float **mat1, **mat2, **result;
   float a = 1.0;                                     /* define varibales */
   int i, j;

/* Dynamic memory allocation for Matrices*/
/* Allocate memory for matrix rows and columns = 1000 X 1000 */

/* Allocating memory to rows of all 3 matrices */
   mat1 = (float **) malloc(SIZE * sizeof(float *));
   mat2 = (float **) malloc(SIZE * sizeof(float *));
   result = (float **) malloc(SIZE * sizeof(float *));

/* Allocating memory to cols of all 3 matrices */
   for (i=0;i<SIZE;i++){
       mat1[i] = (float *) malloc(SIZE * sizeof(float));
       mat2[i] = (float *) malloc(SIZE * sizeof(float));
       result[i] = (float *) malloc(SIZE * sizeof(float));
   }

/* Generating matrix elements with random numbers between 0 and 1 */
   srand(time(NULL));
   for (i = 0; i < SIZE; i++) {
      for (j = 0; j < SIZE; j++) {
         mat1 [i][j] = (float) rand()/ (float) (RAND_MAX/a);
         mat2 [i][j] = (float) rand()/ (float) (RAND_MAX/a);
      }
   }

   // declare the threads
     pthread_t threads[NUM_THREADS];

    int *param  = (int *)malloc(2 * sizeof(int));

   // declareing threads
     for (i = 0; i < NUM_THREADS; i++){
    //     int* p;
         pthread_create(&threads[i], NULL, multiplication, param);
     }

   // joining and waiting for all threads to complete
     for (i = 0; i < NUM_THREADS; i++){
         pthread_join(threads[i], NULL);
     }

/* Free the allocated memory for all three matrices using free() */
   for (i = 0; i < SIZE; i++){
       free(mat1[i]);
       free(mat2[i]);
       free(result[i]);
   }

   free(mat1);
   free(mat2);
   free(result);

   mat1 = NULL;
   mat2 = NULL;
   result = NULL;
   
   return 0;
}

