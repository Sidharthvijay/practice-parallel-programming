//Header files
#include<stdio.h>
#include<cuda.h>
#include<sys/time.h>

//Macros
#define ROWS 400	//No. of rows in orig image
#define COLS 640	//No. of cols in orig image
#define PLANES 3	//No. of planes in orig image
#define MASTER 0	//In Future, this code can be used with MPI

#define CUDA_SAFE_CALL(call)														 \
			do{                                                                      \
                 cudaError_t err = call;                                             \
                 if(err != cudaSuccess)                                              \
                 {                                                                   \
                        fprintf(stderr, "Cuda error in file '%s' in line %i : %s.\n",\
                         __FILE__, __LINE__, cudaGetErrorString( err) );             \
                         exit(1);                                                    \
                 }                                                                   \
               } while (0)   


/*This function converts pixel intensity from RGB value to Gray scale */
void RGB2GRAY(uchar4 *In, uint8_t *Gray){
	
	int ii=0;	
	for(ii=0; ii<ROWS*COLS; ii++){
		//Convert to Gray value
		Gray[ii] = (uint8_t)(0.3*In[ii].x + 0.59*In[ii].y + 0.11*In[ii].z);
		//printf("R = %d G = %d B = %d GR = %d \n", In[ii].x, In[ii].y, In[ii].z, Gray[ii]);
	}
}	//End of RGB2GRAY 


/*This function converts pixel intensity from RGB value to Gray scale */
__global__ void CUDA_RGB2GRAY(uchar4 *In, uint8_t *Gray){
	
	const long ii = threadIdx.x + blockDim.x*blockIdx.x;	//Thread initialization
	/*blockDim.x is no of threads per block */
	
	if(ii<ROWS*COLS){	//If thresads are too many
		//Convert to Gray value
		Gray[ii] = (uint8_t)(0.3*In[ii].x + 0.59*In[ii].y + 0.11*In[ii].z);
		//printf("R = %d G = %d B = %d GR = %d ii = %ld \n", In[ii].x, In[ii].y, In[ii].z, Gray[ii], ii);
	}
}	//End of CUDA_RGB2GRAY 


/*This function checks if the device (GPU) is available */
int CheckDevice(int rank)
{
        int DeviceCount, Device;
        struct cudaDeviceProp Properties;

        cudaGetDeviceCount(&DeviceCount);
        if(DeviceCount >= 1)
        {
                cudaGetDevice(&Device);
                cudaGetDeviceProperties(&Properties, Device);
                printf("Processor with rank %d has the Device by name %s and computation is done on this device \n",rank, Properties.name);
        }

        return(DeviceCount);
}//End of CheckDevice


int main(void){
	
	int ii=0, jj=0;	//Counter variables
	FILE *fptr;	//File pointer
	
	//Host variables
	uchar4 *InMat;
	uint8_t *OutMat;
	
	//Device variables
	uchar4 *d_InMat;
	uint8_t *d_OutMat;
	int DeviceStatus=0;
	int BLOCKSIZE, GRIDSIZE;
	
	//For timing
	struct timeval start, end;
	
	//Read the file
	if((fptr=fopen("./Input.txt","r")) == NULL){		
		printf("Input.txt file does not exist in the current folder. \n");
		return 0;
	}
	
	//Allocating memory
	InMat = (uchar4 *)malloc(ROWS*COLS*sizeof(uchar4));
	OutMat = (uint8_t *)malloc(ROWS*COLS*sizeof(uint8_t));
		
	printf("Reading matrix from Input.txt \n");
	for(ii=0; ii<ROWS; ii++){
		for(jj=0; jj<COLS; jj++){
						
			fscanf(fptr,"%d",&InMat[ii*COLS+jj].x);
			fscanf(fptr,"%d",&InMat[ii*COLS+jj].y);
			fscanf(fptr,"%d",&InMat[ii*COLS+jj].z);
			
		}		
	}
	printf("Input matrix is read! \n");
	fclose(fptr);	//Close the file
	
	//Check if device (GPU) is avialable
	DeviceStatus = CheckDevice(MASTER);
	
	gettimeofday(&start, NULL);	//Start time
	
	if(DeviceStatus == 0){	//If CPU does the work
		printf("GPU is not available. RGB2GRAY scale conversion done at the CPU. \n");
		RGB2GRAY(InMat, OutMat);
	}
	else{	//If GPU does the work
	
		CUDA_SAFE_CALL(cudaSetDevice(MASTER));	//Setting the device
		
		//Allocate memory
		CUDA_SAFE_CALL(cudaMalloc((void **)&d_InMat, ROWS*COLS*sizeof(uchar4)));
		CUDA_SAFE_CALL(cudaMalloc((void **)&d_OutMat, ROWS*COLS*sizeof(uint8_t)));
		
		//Copy matrix from Host (CPU) to device (GPU)
		CUDA_SAFE_CALL(cudaMemcpy(d_InMat, InMat, ROWS*COLS*sizeof(uchar4), cudaMemcpyHostToDevice));
		
		/*Calculate Gridsize (which is how many blocks per grid), 
		BLOCKSIZE (which is how many threads per block)	*/
		dim3 GRIDSIZE(250,1,1);
		dim3 BLOCKSIZE(1024,1,1);
		CUDA_RGB2GRAY<<<GRIDSIZE, BLOCKSIZE>>>(d_InMat, d_OutMat);
		
		//Syncronize tasks
		
		//Copy matrix from device (GPU) to host (CPU)
		CUDA_SAFE_CALL(cudaMemcpy(OutMat, d_OutMat, ROWS*COLS*sizeof(uint8_t), cudaMemcpyDeviceToHost));
		
		//Free device memory
		CUDA_SAFE_CALL(cudaFree(d_InMat));
		CUDA_SAFE_CALL(cudaFree(d_OutMat));
	}
	
	gettimeofday(&end, NULL);	//End time
	printf("Time elpased in multiplication: %fsec. \n", ((end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec)/1000000.0);
	
	//Store the values in Output.txt
	if((fptr=fopen("./Output.txt","w")) == NULL){
		printf("Problem occurred while writing in Output.txt file. \n");
		return 0;
	}
	
	printf("Writing matrix in Output.txt file \n");
	for(ii=0; ii<ROWS*COLS; ii++){
		fprintf(fptr,"%d ",OutMat[ii]);
		//printf("%d ", OutMat[ii]);
		//New line
		if((ii+1)%COLS == 0){
			fprintf(fptr,"\n");
			//printf("\n");
		}
	}
	printf("Output matrix is written! \n");
	fclose(fptr);
	printf("Use MATLAB or OpenCV to check it.\n");
	
	//Free memory
	free(InMat);
	free(OutMat);
	
	return 0;

}	//End of main

