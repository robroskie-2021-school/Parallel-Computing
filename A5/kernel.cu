
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define CHK(x){                  \
    cudaError_t e = x;           \
    if(e != cudaSuccess){        \
        printf("\nCUDA Error" + e);  \
        cudaDeviceReset();       \
        exit(1);                 \
}}                               \



__global__ void addKernel(double *b, double n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)
        b[i] = (double)i / n;
}

int main()
{   
    double start, end;
    int n = 10000000;
    double size = n * sizeof(double);
   
    //Serial code
    double* a;
    a = (double*)malloc(size);
    
    //Initialize array and compute time taken
    start = clock();
    for (int i = 0; i < n; i++)
        a[i] = (double)i / n;
    end = clock();

    //Print test results and time taken
    for (int j = 0; j < 5; j++)
        printf("a[%d]: %.7f\n", j, a[j]);
    printf("...\n");
    for (int j = n-5; j < n; j++)
        printf("a[%d]: %.7f\n", j, a[j]);

    printf("Serial code took %.2f ms to initialize array\n\n",(end - start) / CLOCKS_PER_SEC * 1000);

    //Repeat parallelized version of procedure 
    double* b;
    double* b_d;

    b = (double*)malloc(n * sizeof(double));
    
    CHK(cudaMalloc(&b_d, size));

    CHK(cudaMemcpy(b_d, b, size, cudaMemcpyHostToDevice));

    start = clock();
    //n = 10000 * 1000
    addKernel<<<10000,1000>>>(b_d, n);
    cudaDeviceSynchronize();
    end = clock();

    CHK(cudaMemcpy(b, b_d, size, cudaMemcpyDeviceToHost));
 
    for (int j = 0; j < 5; j++)
        printf("b[%d]: %.7f\n", j, b[j]);
    printf("...\n");
    for (int j = n - 5; j < n; j++)
        printf("b[%d]: %.7f\n", j, b[j]);

    printf("Parallel code took %.2f ms to initialize array", (end - start) / CLOCKS_PER_SEC * 1000);

    return 0;
}
