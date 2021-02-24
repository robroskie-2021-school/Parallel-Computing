// a) Both count and j should be private. temp should be shared
// b) Since elements do not depend on values from other iterations there are no loop carried dependencies

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void count_sort(int a[], int n) {
	int i, j;
	//count;
	int* temp = malloc(n * sizeof(int));

	for (i = 0; i < n; i++){
		//count all elements < a[i]
		int count = 0;
	#pragma omp parallel for num_threads(8) private(j) reduction(+ : count)
		for (j = 0; j < n; j++){
			if(a[j]<a[i] ||(a[j]==a[i] && j<i))
				count++;
		}
		//place a[i] at right order
		temp[count] = a[i];
	}

	for(int i = 0; i < 20; i++)
		printf("%d, ",temp[i]);

	free(temp);
}

int main (int argc, char *argv[]) {
	//Test array
	 int A[20] = {9,8,7,2,3,10,0,9,5,1,11,9,8,7,2,3,10,0,9,5};

	 count_sort(A,20);

	 return 0;
}


