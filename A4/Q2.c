#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NRA 6  /* number of rows in A */
#define NCA 5  /* number of columns in A = number of rows in B */
#define NCB 4  /* number of columns in matrix B */

int main (int argc, char *argv[]) {

	//Initialize empty arrays from defined dimensions
	int a[NRA][NCA];
	int b[NCA][NCB];
	int C[NRA][NCB];
	int j, d;

	//Start clock
	double start = clock();

	//Multiply arrays and output result
	#pragma omp parallel for private(j,d)
	for(int i = 0; i < NRA; i++){
		for(j = 0; j < NCB; j++){
				C[i][j] = 0;
				for(d = 0; d < NCA; d++){
					a[i][d] = i+1;
					b[d][i] = d+2;
					C[i][j] += a[i][d] * b[d][i];
				}
		}
	}
	double end = clock();

	//Output time
	printf("Time elapsed: %f ms",(end-start)/CLOCKS_PER_SEC*1000);
	//fflush(stdout);

	//Print array
	for(int z = 0; z < NRA; z++){
		printf("\n");
		for(int h = 0; h < NCB; h++){
			{
				printf("C[%d][%d]=%3d    ",z,h,C[z][h]);
			}
		}
	}

 return 0;
}
