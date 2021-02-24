#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Return random double -1 to 1
double RandomNumber(){
	return (-1 + 2 * (double)rand() / RAND_MAX);
}

double MonteCarlo(int number_of_tosses){
	long int number_in_circle = 0;

	//Calculate number of tosses that are in circle
	#pragma omp parallel for reduction(+ : number_in_circle)
	for(long int toss = 0; toss < number_of_tosses; toss++) {
		double x = RandomNumber();
		double y = RandomNumber();
		double distance_squared = x * x + y * y;

		if (distance_squared <= 1)
			number_in_circle++;
	}

	//Calculate and return result
	double pi_estimate = 4 * number_in_circle / ((double) number_of_tosses);
	return pi_estimate;
}

int main (int argc, char *argv[]) {
	//Seed rand() seed
	srand(time(NULL));

	//Default toss value
	int num_toss = 100000;

	//Get input
	printf("Enter number of tosses: ");
	fflush(stdout);
	scanf("%d",&num_toss);

	printf("On %d tosses the estimate of pi was: %f",num_toss,MonteCarlo(num_toss));
	return 0;
}


