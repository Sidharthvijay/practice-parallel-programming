/*
 * Filename: pi.c
 * Author: Pradeep Singh
 * Date: 3/12/2018
 * Description: OpenMP implementation of Monte Carlo pi-finding algorithm
 *
 * Usage:    gcc -Wall -Werror -fopenmp pi.c -o pi   // compile
 *           ./pi <threads>                          // run -- with # of threads
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>

int main (int argc, char *argv[])
{
  int i, count, samples, nthreads, seed;
  struct drand48_data drand_buf;
  double x, y;
  struct  timeval start, end;
  samples = 32000000;
  nthreads = atoi(argv[1]);
  omp_set_num_threads (nthreads);

  gettimeofday(&start, NULL);
  count = 0;

#pragma omp parallel private(i, x, y, seed, drand_buf) shared(samples)
{
  seed = 1202107158 + omp_get_thread_num() * 1999;
  srand48_r (seed, &drand_buf);

  #pragma omp for reduction(+:count)
  for (i=0; i<samples; i++) {
      drand48_r (&drand_buf, &x);
      drand48_r (&drand_buf, &y);
      if (x*x + y*y <= 1.0) count++;
  }
}

  gettimeofday(&end, NULL);

  printf("Estimate of pi: %7.5f\n", 4.0*count/samples);
  printf("\n Time Elapsed: %fs \n", ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0));

  return 0;
}
