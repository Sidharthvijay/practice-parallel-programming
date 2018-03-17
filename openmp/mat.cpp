
*
 * Filename: Mat.cpp
 * Author: Pradeep Singh
 * Date: 3/12/2018
 * Description:
 *
 *
 * Compilation: g++ -g -Wall -Werror -fopenmp Mat.cpp -O3 -o ompMatMat
 * Usage:       ./Mat <Number of threads>
 *
 */

#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
using namespace std;

int thread_count = 1; //Default initialization of global thread count

//Function Prototypes: Pass by reference is used here to avoid use of triple
//pointers.
void multiply(double ** &mat1,double ** &mat2,double ** &res,int m,int n,int o);
void transpose(double ** &res, int m, int o);
void printer(double ** &mat, int row, int col); //For debugging only
void matAlloc(double ** &mat, int row, int col);
void matClean(double ** &mat, int row);
void matFill(double ** &mat, int row, int col);

int main(int argc, char *argv[]) {
   //Check for proper number of input arguments or exit failure
   if(argc != 2) {
           cout << "Usage: " << argv[0] << " (number of threads)\n";
           exit(-1);
   }

   //Number of user requested threads
   thread_count = atoi(argv[1]);

   //Double pointers to the matricies
   double **matrix1, **matrix2, **result;

   //Matrix Dimensions
   int m = 4000, n = 3000, o = 3200;

   //Timer variables for gettimeofDay
   timeval start, end;

//Dynamically allocate matricies
   matAlloc(matrix1, m, n);
   matAlloc(matrix2, n, o);
   matAlloc(result , m, o);

   //Random fill of matrix1 & matrix2
   matFill(matrix1, m, n);
   matFill(matrix2, n, o);

//Begin operation timer and record start time
   gettimeofday(&start, NULL);

   //Implement Parallel Code Here
   #pragma omp parallel num_threads(thread_count)
      multiply(matrix1, matrix2, result, m, n, o);

   //End and record operation end time
   gettimeofday(&end, NULL);

   //Determine and output elapsed operation time
   cout << "Elapsed Multiply: " << ((end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec)/1000000.0 << endl;

   //printer(result, m, o);
   //Begin operation timer and record start time
   gettimeofday(&start, NULL);

   //Implement Parallel Code Here
   //#pragma omp parallel num_threads(thread_count)
   transpose(result, m, o);

   //End and record operation end time
   gettimeofday(&end, NULL);

   //Determine and output elapsed operation time
   cout << "Elapsed Transpose: " << ((end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec)/1000000.0 << endl;
   //printer(matrix1, m, n);
   //printer(matrix2, n, o);
   //printer(result, o, m);
   //Free the dyanmic allocated memory
   matClean(matrix1, m);
   matClean(matrix2, n);
   matClean(result , o);

   return 0;
}



//Performs the m by n times n by o matrix multiplicaton
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

//Transposes the resulting matrix
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
//Printer Function for Debugging Purposes Only
void printer(double ** &mat, int row, int col) {
        for(int i=0; i<row; i++) {
                for(int j=0; j<col; j++) {
                        cout << mat[i][j] << " ";
                }
                cout << "\n";
        }
        cout << "\n";
}

//Dynamically allocate the matrix with given dimensions
void matAlloc(double ** &mat, int row, int col) {
        mat = new double*[row];
        for(int i=0; i<row; i++) {
                mat[i] = new double[col];
        }
}

//Clean up the mess and free the allocations
void matClean(double ** &mat, int row) {
   for(int i=0; i<row; i++) {
           delete [] mat[i];
   }
   delete [] mat;
}

//Fill the matricies with random double precision numbers
void matFill(double ** &mat, int row, int col) {
        srand(time(NULL));
for(int i=0; i<row; i++) {
                for(int j=0; j<col; j++){
                        mat[i][j] = (rand() % 10000)/10000.0;
                }
        }
}


