/*
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <omp.h>

#define m 2000
#define n 2000
#define o 2000

// declaring matrices of NxN size
double **mat1, **mat2, **result;
int NUM_THREADS;

int main(int argc, char *argv[])
{

   if (argc != 2) {
      fprintf( stderr, "%s <number of threads>\n", argv[0] );
      return -1;
   }

   NUM_THREADS = atoi( argv[1] );

   int i, j, k;
   struct timeval start, end;

/* Dynamic memory allocation for Matrices*/
/* Allocate memory for matrix rows and columns = 1000 X 1000 */

   /* Matrix -- 1 */
   mat1 = (double **) malloc(m * sizeof(double *));        /* allocating memory to rows */
   for (i=0;i<n;i++)                                       /* allocating memory to col */
       mat1[i] = (double *) malloc(n * sizeof(double));

/* Matrix -- 2 */
   mat2 = (double **) malloc(n * sizeof(double *));       /* allocating memory to rows */
   for (i=0;i<o;i++)                                      /* allocating memory to col */
       mat2[i] = (double *) malloc(o * sizeof(double));

   /* Result Matrix */
   result = (double **) malloc(m * sizeof(double *));     /* allocating memory to rows */
   for (i=0;i<o;i++)                                      /* allocating memory to col */
       result[i] = (double *) malloc(o * sizeof(double));

   /* Generating matrix elements with random numbers between 0 and 1 */
   srand(time(NULL));                                        /* srand() sets the seed for rand() */
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++) {
         mat1[i][j] = (double) rand()/ RAND_MAX;             // generates random number between 0 and 1        
      }
   }

   for (i = 0; i < n; i++) {
      for (j = 0; j < o; j++) {
         mat2[i][j] = (double) rand()/ RAND_MAX;              // generates random number between 0 and 1
      }
   }

   gettimeofday(&start, NULL);                                  /* start measuring time */

   #pragma omp parallel num_threads(NUM_THREADS)
   for (i = 0; i < m; i++) {
       for (j = 0; j < o; j++) {
              result[i][j] = 0;
            for (k =0; k < n; k++) {
               result[i][j] += mat1[i][k] * mat2[k][j];
           }
       }
   }

   gettimeofday(&end, NULL);    /* function to measure execution time */

   /* Print the execution time */
   printf("\n Execution Time: %fs \n", ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0));

   // Free memory (deallocate the memory)
   /* Free the allocated memory for all three matrices using free() */
   for (i = 0; i < n; i++){
       free(mat1[i]);
   }
    free(mat1);

   for (i = 0; i < o; i++){
       free(mat2[i]);
   }
   free(mat2);

   for (i = 0; i < o; i++){
       free(result[i]);
   }
   free(result);

   return 0;
}

