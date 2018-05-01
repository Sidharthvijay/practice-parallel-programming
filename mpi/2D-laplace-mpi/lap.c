/* 
 * Filename: lap.c
 * Date: April 28, 2018
 * Author: Pradeep Singh
 * Description: 2D domain decomposition using MPI is carried in two dimension 
 *              by solving Laplace equation. The size of the matrix is 12 x 12.
 * 
 *              This program decomposes this 12 x 12 matrix into 4 blocks and 4 processors are used. 
 *              The mesh is divided into 4 regions, each region is assigned to 1 processor. 
 * 
 *  usage: make
 *          qsub batch.lap 
 */

#include <stdio.h>
#include <math.h>
#include "mpi.h"

/* This program handles a 12 x 12 mesh, on 4 processors only. */
#define DomainSize 12

int main(int argc, char **argv)
{
    int        rank, size, i, j, count;
	int		   n, max_iteration;
    int        i_first, i_last, j_first, j_last;
    double     diffnorm, gdiffnorm, tolerence;
	double start=0.0, end=0.0;
	n = (DomainSize/2)+1;
	double x_local[n][n];
    double x_new[n][n];
	double send_buff1[n-1];
	double send_buff2[n-1];
    double recv_buff1[n-1];
    double recv_buff2[n-1];
	

    MPI_Init(&argc, &argv);

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

	
    if (size != 4) MPI_Abort( MPI_COMM_WORLD, 1 );

	i_first = 1;
    i_last  = n-2;
    j_first = 1;
    j_last  = n-2;

    /* =================== Filling initial data ======================*/
    for (i=i_first; i<=i_last; i++) 
		for (j=j_first; j<=j_last; j++) 
			x_local[i][j] = rank;
	
	if (rank==0){
		for (j=0; j<n-1; j++) {
			x_local[0][j] = -1;
			x_local[j][0] = -1;
			}
		}
	
	if (rank==1){
		for (j=1; j<n; j++) {
			x_local[0][j] 		 = -1;
			x_local[j][n-1] = -1;
			}
		}
	
	if (rank==2){
		for (j=0; j<n-1; j++) {
			x_local[n-1][j] = -1;
			x_local[j][0] 		 = -1;
			}
		}
	
	if (rank==3){
		for (j=1; j<n; j++) {
			x_local[n-1][j] = -1;
			x_local[j][n-1] = -1;
			}
		}
	/* ================ computation ======== */

	count = 0;
    max_iteration = 200;
    tolerence = 1.0e-2;
	start = MPI_Wtime();	//start time
    do {
		if (rank == 0){
			
			for (i=0;i<n-1;i++){
				send_buff1[i] = x_local[i][n-2];
				send_buff2[i] = x_local[n-2][i];
				}
				
			MPI_Send( send_buff1, n-1, MPI_DOUBLE, 1, 0, 
				  MPI_COMM_WORLD );
				  
			MPI_Send( send_buff2, n-1, MPI_DOUBLE, 2, 0, 
				  MPI_COMM_WORLD );
				  
			MPI_Recv( recv_buff1, n-1, MPI_DOUBLE, 1, 0, 
				  MPI_COMM_WORLD, MPI_STATUS_IGNORE );
			
			MPI_Recv( recv_buff2, n-1, MPI_DOUBLE, 2, 0, 
				  MPI_COMM_WORLD, MPI_STATUS_IGNORE );
			
			for (i=0;i<n-1;i++){
				x_local[i][n-1] = recv_buff1[i];
				x_local[n-1][i] = recv_buff2[i];
				}
			}
			
		if (rank == 1){
			for (i=0;i<n-1;i++){
				send_buff1[i] = x_local[i][1];
				send_buff2[i] = x_local[n-2][i+1];
				}
				
			MPI_Send( send_buff1, n-1, MPI_DOUBLE, 0, 0, 
				  MPI_COMM_WORLD );
			
			MPI_Send( send_buff2, n-1, MPI_DOUBLE, 3, 0, 
				  MPI_COMM_WORLD );
				  
			MPI_Recv( recv_buff1, n-1, MPI_DOUBLE, 0, 0, 
				  MPI_COMM_WORLD, MPI_STATUS_IGNORE );
				  
			MPI_Recv( recv_buff2, n-1, MPI_DOUBLE, 3, 0, 
				  MPI_COMM_WORLD, MPI_STATUS_IGNORE );
			
			for (i=0;i<n-1;i++){
				x_local[i][0] = recv_buff1[i];
				x_local[n-1][i+1] = recv_buff2[i];
				}
			}
		
		if (rank == 2){
			for (i=0;i<n-1;i++){
				send_buff1[i] = x_local[i+1][n-2];
				send_buff2[i] = x_local[1][i];
				}
				
			MPI_Send( send_buff1, n-1, MPI_DOUBLE, 3, 0, 
				  MPI_COMM_WORLD );
			
			MPI_Send( send_buff2, n-1, MPI_DOUBLE, 0, 0, 
				  MPI_COMM_WORLD );
				  
			MPI_Recv( recv_buff1, n-1, MPI_DOUBLE, 3, 0, 
				  MPI_COMM_WORLD, MPI_STATUS_IGNORE );

			MPI_Recv( recv_buff2, n-1, MPI_DOUBLE, 0, 0, 
				  MPI_COMM_WORLD, MPI_STATUS_IGNORE );
			
			for (i=0;i<n-1;i++){
				x_local[i+1][n-1] = recv_buff1[i];
				x_local[0][i] 		   = recv_buff2[i];
				}
			}
		
		if (rank == 3){
			for (i=0;i<n-1;i++){
				send_buff1[i] = x_local[i+1][1];
				send_buff2[i] = x_local[1][i+1];
				}
			
			MPI_Send( send_buff1, n-1, MPI_DOUBLE, 2, 0, 
				  MPI_COMM_WORLD );
				  
			MPI_Send( send_buff2, n-1, MPI_DOUBLE, 1, 0, 
				  MPI_COMM_WORLD );
				  
			MPI_Recv( recv_buff1, n-1, MPI_DOUBLE, 2, 0, 
				  MPI_COMM_WORLD, MPI_STATUS_IGNORE );
				  
			MPI_Recv( recv_buff2, n-1, MPI_DOUBLE, 1, 0, 
				  MPI_COMM_WORLD, MPI_STATUS_IGNORE );
				  
			for (i=0;i<n-1;i++){
				x_local[i+1][0] = recv_buff1[i];
				x_local[0][i+1] = recv_buff2[i];
				}
			}
		
		/* Compute new values (but not on boundary) */
		count ++;
		diffnorm = 0.0;
		for (i=i_first; i<=i_last; i++) 
			for (j=j_first; j<=j_last; j++) {
			x_new[i][j] = (x_local[i][j+1] + x_local[i][j-1] +
					  x_local[i+1][j] + x_local[i-1][j]) / 4.0;
			diffnorm += (x_new[i][j] - x_local[i][j]) * 
						(x_new[i][j] - x_local[i][j]);
			}
		
		/* Only transfer the interior points */
		for (i=i_first; i<=i_last; i++) 
			for (j=j_first; j<=j_last; j++) 
			x_local[i][j] = x_new[i][j];

		MPI_Allreduce( &diffnorm, &gdiffnorm, 1, MPI_DOUBLE, MPI_SUM,
				   MPI_COMM_WORLD );
		gdiffnorm = sqrt( gdiffnorm );
		if (rank == 0) printf( "At iteration %d, diff is %e\n", count, 
					   gdiffnorm );
		
		} while (gdiffnorm > tolerence && count < max_iteration);

	end = MPI_Wtime();	//end time
	
	if(rank == 0)
		printf("The elapsed time is: %lf\n", end-start);
	
    MPI_Finalize( );
    return 0;
}
