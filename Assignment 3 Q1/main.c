/**************************************************************

 The program reads a BMP image file and creates a new
 image that is the negative or desaturated of the input file.

 **************************************************************/

#include "qdbmp.h"
#include <stdio.h>
#include <omp.h>

typedef enum {desaturate, negative} ImgProcessing ;

/* Creates a negative image of the input bitmap file */
int main() {
	const char* inFile = "okanagan.bmp";
	const char* outFile = "processed.bmp";
	const ImgProcessing processingType = desaturate; //or negative

	UCHAR r, g, b;
	UINT width, height;
	BMP* bmp;

	/* Read an image file */
	bmp = BMP_ReadFile(inFile);
	BMP_CHECK_ERROR(stdout, -1);

	/* Get image's dimensions */
	width = BMP_GetWidth(bmp);
	height = BMP_GetHeight(bmp);

	double t = omp_get_wtime();



	// Parallelization will be applied to the outer loop(x pixel locations)
	/* Num threads */
	int x,y;
	int n = 4;

	int intervals = width / n;
	printf("intervals= %d\n",intervals);

	#pragma omp parallel num_threads(n) private(x,y)
	{
		int thread = omp_get_thread_num();
		int my_a = thread * intervals;
		int my_b = my_a + intervals;
		printf("T=%d my_a=%d my_b=%d\n",thread,my_a,my_b);

		//#pragma omp critical
		for (x = my_a; x < my_b; x++) {

			
			for (y = 0; y < height; y++) {

				//printf("T=%d is on x=%d y=%d\n",thread,x,y);

				/* Get pixel's RGB values */
				
				
					BMP_GetPixelRGB(bmp, x, y, &r, &g, &b);
				
				/* Write new RGB values */


					if(processingType == negative)
						BMP_SetPixelRGB(bmp, x, y, 255 - r, 255 - g, 255 - b);
				
					else if(processingType == desaturate){
						UCHAR gray = r * 0.3 + g * 0.59 + b * 0.11;
						BMP_SetPixelRGB(bmp, x, y, gray, gray, gray);
					}
				
				}
			}
	}
	



	/* calculate and print processing time*/
	t = 1000 * (omp_get_wtime() - t);
	printf("Finished image processing in %.1f ms.", t);

	/* Save result */
	BMP_WriteFile(bmp, outFile);
	BMP_CHECK_ERROR(stdout, -2);

	/* Free all memory allocated for the image */
	BMP_Free(bmp);

	return 0;
}
