/* Assignment #3 Q2 a) */
/*
Output
    Enter a, b, and n
    0 50000000 5000000

    Total time used by Thread 0 is 13.1010ms
    Total time used by Thread 1 is 14.7770ms
    Total time used by Thread 3 is 12.9650ms
    Total time used by Thread 2 is 15.0960ms

    Total time used by all parallel regions is 55.9390ms

    With n = 5000000 trapezoids, our estimate of the integral
    from 0.000000 to 50000000.000000 = 4.16666666666764e+22   
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void Usage(char* prog_name);
double f(double x);    /* Function we're integrating */
double Trap(double a, double b, int n);

int main(int argc, char* argv[]) {
    double  global_result = 0.0;  /* Store result in global_result */
    double  a, b;                 /* Left and right endpoints      */
    int     n;                    /* Total number of trapezoids    */
    int     thread_count;
    double elapsed = 0;

    if (argc != 2) Usage(argv[0]);
        thread_count = strtol(argv[1], NULL, 10);
    printf("Enter a, b, and n\n"); fflush(stdout);
    scanf("%lf %lf %d", &a, &b, &n);
    if (n % thread_count != 0) Usage(argv[0]);

    #pragma omp parallel num_threads(thread_count) reduction(+ : global_result, elapsed)
    {
        double start = omp_get_wtime();
        
        double local_area = Trap(a, b, n);
        #pragma omp critical
        global_result += local_area;

        double end = omp_get_wtime();
        double total = (end - start) * 1000;
        printf("Total time used by Thread %d is %.4fms\n",omp_get_thread_num(),total);
        elapsed += total;  
    }
    printf("Total time used by all parallel regions is %.4fms\n",elapsed);
    
    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.14e\n", a, b, global_result);
    return 0;
}
 /* main */

/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "   number of trapezoids must be evenly divisible by\n");
   fprintf(stderr, "   number of threads\n");
   exit(0);
}  /* Usage */

/*------------------------------------------------------------------
 * Function:    f
 * Purpose:     Compute value of function to be integrated
 * Input arg:   x
 * Return val:  f(x)
 */
double f(double x) {
   double return_val;

   return_val = x*x;
   return return_val;
}  /* f */

/*------------------------------------------------------------------
 * Function:    Trap
 * Purpose:     Use trapezoidal rule to estimate definite integral
 * Input args:  
 *    a: left endpoint
 *    b: right endpoint
 *    n: number of trapezoids
 * Output arg:
 *    integral:  estimate of integral from a to b of f(x)
 */
double Trap(double a, double b, int n) {
   double  h, x, my_result;
   double  local_a, local_b;
   int  i, local_n;
   int my_rank = omp_get_thread_num();
   int thread_count = omp_get_num_threads();

   h = (b-a)/n; 
   local_n = n/thread_count;  
   local_a = a + my_rank*local_n*h; 
   local_b = local_a + local_n*h; 
   my_result = (f(local_a) + f(local_b))/2.0; 
   for (i = 1; i <= local_n-1; i++) {
     x = local_a + i*h;
     my_result += f(x);
   }
   my_result = my_result*h; 
   
   return my_result;
}
