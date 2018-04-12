/*
 * File name: ping-pong.c
 * Author: Pradeep Singh
 * Date: 11/8/2018
 * Description: Play Ping Pong on Tuckoo.
 *
 *              o Program usage:
 *                            - mpirun -np 2 ./ping-pong 10000                      // run
 *                            - make                                                // compile
 *
 *              o PBS Usage: - qsub -v T=100 machine batch.ping-pong                // submit job
 *                           - qstat                                                // check job status
 *                           - cat mat.ojob_id                                      // check o/p
 */

#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define proc_A 0
#define proc_B 1
#define ping 101
#define pong 101

//double buffer[10001];

int processor_A (void);
int processor_B (void);

int main ( int argc, char *argv[] )
{
  int rank;
  int size;
  int ii;
 // int sdouble;
  MPI_Status status;
  double tic, toc, elapsed_time;
  //double buffer[10001];
  int length;
 // sdouble = sizeof(double);

  if (argc != 2){
     printf("Usage: <number_of_packets> Eg: 10,000 \n");
     return 0;
  }
char* end_ptr_for_strtol;
  double N = strtol(argv[1], &end_ptr_for_strtol, 10);
  double* x;
  x = (double*)malloc(N*sizeof(double));

  /* Initialize MPI */
  MPI_Init(&argc, &argv);

  /* Get rank */
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  /* keep the example to a simple pairwise exchange */
  if (size != 2) {
     fprintf(stderr,"Error: wrong number of processes (use 2 processes)\n");
      MPI_Abort(MPI_COMM_WORLD,1);
  }

  /* Call ping-pong functions */
  if (rank == proc_A) {
     printf("length    transfer rate (Gbit/sec)\n");

     /* Process A sets the message size */
     for (length=1; length<=N; length+=1000) {
         /* Get the start time for the pingpong message passing */
         tic = MPI_Wtime();

         /* Process A sends and then receives the message back 1000 times */
        for (ii=0; ii<1000; ii++){
            MPI_Ssend(x, length, MPI_DOUBLE, proc_B, ping, MPI_COMM_WORLD);
            MPI_Recv(x, length, MPI_DOUBLE, proc_B, pong, MPI_COMM_WORLD, &status);
        }

        /* Get the finish time for the pingpong message passing */
        toc = MPI_Wtime();
        elapsed_time = toc - tic;
        printf("%d\t %lf\n", length, (double)(2*sizeof(double)*1000*length*8)/(elapsed_time*1000000000));

        }
   }
else if (rank == proc_B) {
        /* Process B sets the message size */
        for (length=1; length<=N; length+=1000) {
            /* Process B receives and then sends the message back 1000 times */
            for (ii=0; ii<1000; ii++) {
                MPI_Recv(x, length, MPI_DOUBLE, proc_A, ping, MPI_COMM_WORLD, &status);
                MPI_Ssend(x, length, MPI_DOUBLE, proc_A, pong, MPI_COMM_WORLD);
            }
        }
   }

   free(x);

   /* Finalize MPI */
   MPI_Finalize();

   return 0;
}

   
  
