/*
 * Filename: mat.cpp
 * Author: Pradeep Singh
 * Date: 3/12/2018
 * Description: Matrix - Matrix multiplication using OpenMP.
 *
 * Program usage:
 *                  g++ -g -Wall -Werror -fopenmp mat.cpp -O3 -o mat    // compile
 *                  ./mat <Number of threads>                           // run -- pass # of threads
 *
 */

#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
using namespace std;

int thread_count = 1;        //Default initialization of global thread count

/* Functions used  -- Declaration */

void multiply(double ** &mat1,double ** &mat2,double ** &res,int m,int n,int o);
void transpose(double ** &res, int m, int o);
void printer(double ** &mat, int row, int col); //For debugging only
void matAlloc(double ** &mat, int row, int col);
void matClean(double ** &mat, int row);
void matFill(double ** &mat, int row, int col);

/* Main Program starts here */
int main(int argc, char *argv[]) {

   /* Check whether proper number of arguments are passed or not */
   if(argc != 2) {
           cout << "Usage: " << argv[0] << " (number of threads)\n";
           exit(-1);
   }

   thread_count = atoi(argv[1]);
   double **matrix1, **matrix2, **result;       // Matrices
   int m = 4000, n = 3000, o = 3200;            // Matrix dimesnions

   timeval start, end;                          // Variables to measure time

   /* Dynamically memory allocation */
   matAlloc(matrix1, m, n);
   matAlloc(matrix2, n, o);
   matAlloc(result , m, o);

   /* Randomly fill matrices 1 and 2 */
   matFill(matrix1, m, n);
   matFill(matrix2, n, o);

   gettimeofday(&start, NULL);                  // starts measuring time

   /* Parallelizing starts from here -- using OpenMP */
   #pragma omp parallel num_threads(thread_count)
   multiply(matrix1, matrix2, result, m, n, o);

   gettimeofday(&end, NULL);                   // stop measuring time

   /* Print time elapsed -- Matrix Multiplication */
   cout << "Elapsed Multiply: " << ((end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec)/1000000.0 << endl;

   gettimeofday(&start, NULL);                  // start measuring time
   transpose(result, m, o);                     // transpose the matrix
   gettimeofday(&end, NULL);                    // stop measuring time

   /* Time elapsed -- transposing the matrix*/
   cout << "Elapsed Transpose: " << ((end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec)/1000000.0 << endl;

   /* Deallocate the allocated memory */
   matClean(matrix1, m);
   matClean(matrix2, n);
   matClean(result , o);

   return 0;
}

/* Matrix Multiplication happens here */
void multiply(double ** &mat1,double ** &mat2,double ** &res,int m,int n,int o) {
#pragma omp for
        for(int i=0; i<m; i++) {
                for(int j=0; j<o; j++) {
                        double sum = 0.0;
                        for(int k=0; k<n; k++) {
                                sum += mat1[i][k]*mat2[k][j];
                        }
                        res[i][j] = sum;
                }
        }
}

/* Transposes the resulting matrix  */
void transpose(double ** &res, int row, int col) {
        double ** temp; matAlloc(temp, col, row);
        #pragma omp parallel for num_threads(thread_count)
        for(int i=0; i<row; i++) {
                for(int j=0; j<col; j++) {
                        temp[j][i] = res[i][j];
                }
        }

        matClean(res, row);
        res = temp;
}

/*
Printer Function for Debugging Purposes Only
void printer(double ** &mat, int row, int col) {
        for(int i=0; i<row; i++) {
                for(int j=0; j<col; j++) {
                        cout << mat[i][j] << " ";
                }
                cout << "\n";
        }
        cout << "\n";
}
*/
/* Function to allocate memoery */
void matAlloc(double ** &mat, int row, int col) {
        mat = new double*[row];
        for(int i=0; i<row; i++) {
                mat[i] = new double[col];
        }
}

/* Function to free the allocated memory */
void matClean(double ** &mat, int row) {
   for(int i=0; i<row; i++) {
           delete [] mat[i];
   }
   delete [] mat;
}

/* Function to fill the matricies with random double precision numbers */
void matFill(double ** &mat, int row, int col) {
        srand(time(NULL));
for(int i=0; i<row; i++) {
                for(int j=0; j<col; j++){
                        mat[i][j] = (rand() % 10000)/10000.0;
                }
        }
}
