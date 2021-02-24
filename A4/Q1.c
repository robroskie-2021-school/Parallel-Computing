/**************************************************************

Q1) By testing parallelizing the other loop, inner and both the inner/outer loop,
    it was found that parallelizing only the inner loop was slower than the other two configurations.
    Parallelizing the other and both loops took a similar amount of time to execute,
    so it would be a advisable to only do this to the outer loop,
    to avoid overhead for negligible performance gains.

 **************************************************************/

#include "qdbmp.h"
#include <stdio.h>
#include <omp.h>

typedef enum {desaturate, negative} ImgProcessing ;

/* Creates a negative image of the input bitmap file */
int main() {
	const char* inFile = "src/okanagan.bmp";
	const char* outFile = "src/okanagan_processed.bmp";
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

	int x,y;

	//Number of threads
	int nt = 16;

	//Start clock
	double t = omp_get_wtime();

	/* Iterate through all the image's pixels */
	#pragma omp parallel for num_threads(nt) private(r,g,b,y)
	for (x = 0; x < width; ++x) {

	//#pragma omp parallel for num_threads(nt) private(y)
		for (y = 0; y < height; ++y) {
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


