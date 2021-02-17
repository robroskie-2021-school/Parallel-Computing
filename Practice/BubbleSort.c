
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool isSorted(int* arr2,int n){
	for(int j = 0; j < n; j++){
			printf("%d,",arr2[j]);
		}
	printf("\n");
	for(int j = 0; j < n-1; j++){
		if(arr2[j+1] < arr2[j])
			return false;
		}
	return true;
}

void bubbleSort(int* arr, int numThreads, int n){

	int temp = 0;
	printf("The size of arr is: %d\n",n);
	bool sorted = false;

	#pragma omp parallel num_threads(numThreads) private(temp)
	{
		int tid = omp_get_thread_num();
		int my_a = tid * 2;
		int my_b = my_a+1;

		int my_a_odd = tid * 2+1;
		int my_b_odd = my_a_odd+1;

		while(sorted != true){
			if(arr[my_a]>arr[my_a+1]){
				temp = arr[my_a];
				arr[my_a] = arr[my_a+1];
				arr[my_a+1] = temp;
			}

			#pragma omp barrier
			if(arr[my_a_odd]>arr[my_b_odd]){
				temp = arr[my_a_odd];
				arr[my_a_odd] = arr[my_b_odd];
				arr[my_b_odd] = temp;
			}

			#pragma omp single
			sorted = isSorted(arr,n);
		}
	}

		printf("finished\n");
}


int main (int argc, char *argv[]) {

  int numThreads, tid;
  int testArray[20] = {10,9,8,7,6,239847,5,4,3,2,1,11,12,67,100,1000,4,4,69,44};
  int sizeOf = sizeof(testArray)/sizeof(testArray[0]);

   bubbleSort(testArray, sizeOf/2, sizeOf);

  return 0;
}


