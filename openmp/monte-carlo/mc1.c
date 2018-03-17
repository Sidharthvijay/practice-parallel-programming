/*
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

int main(int argc, char* argv[])
{
    int niter = 1000000;            //number of iterations per FOR loop
    double x=0.0,y=0.0;                     //x,y value for the random coordinate
    int i=0;                          //loop counter
    int count=0;                //Count holds all the number of how many good coordinates
    double z=0.0;                       //Used to check if x^2+y^2<=1
    double pi;                      //holds approx value of pi
   omp_set_dynamic(0);
    int numthreads = 1;
struct timeval start, end;

 gettimeofday(&start, NULL);
    #pragma omp parallel firstprivate(x, y, z, i) shared(count) num_threads(numthreads)
    {
        srandom((int)time(NULL) ^ omp_get_thread_num());    //Give random() a seed value
        for (i=0; i<niter; ++i)              //main loop
        {
            x = (double)random()/RAND_MAX;      //gets a random x coordinate
            y = (double)random()/RAND_MAX;      //gets a random y coordinate
            z = sqrt((x*x)+(y*y));          //Checks to see if number is inside unit circle
            if (z<=1) {
               ++count;            //if it is, consider it a valid random point
            }
         }
         //print the value of each thread/rank
     }

         gettimeofday(&end, NULL);    /* function to measure execution time */

     /* Print the execution time */
     printf("\n Execution Time: %fs \n", ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0));

     pi = ((double)count/(double)(niter*numthreads))*4.0;
     printf("Pi: %f\n", pi);

     return 0;
}
