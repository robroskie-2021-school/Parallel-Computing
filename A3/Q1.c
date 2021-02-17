//2 Threads: Finished image processing in 2050.0 ms.
//4 Threads: Finished image processing in 3893.6 ms.
//8 Threads: Finished image processing in 4118.5 ms.
//16 Threads: Finished image processing in 4346.8 ms.

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
	const char* outFile = "okanagan_processed1.bmp";
	const ImgProcessing processingType = desaturate; //or negative

	UCHAR r, g, b;
	int width, height;
	int x;

	BMP* bmp;

	/* Read an image file */
	bmp = BMP_ReadFile(inFile);
	BMP_CHECK_ERROR(stdout, -1);

	/* Get image's dimensions */
	width = BMP_GetWidth(bmp);
	height = BMP_GetHeight(bmp);

	int nt = 4;
	int intervals = height / nt;
	int thread;
	int my_a;
	int my_b;
	int y;
	double t = omp_get_wtime();

		for (x = 0; x < width; ++x) {
			/* Iterate through all the image's pixels */
            #pragma omp parallel num_threads(nt) private(thread,my_a,my_b,y,r,g,b)
            {
                thread = omp_get_thread_num();
                my_a = thread * intervals;
                my_b = my_a + intervals;

                for (y = my_a; y < my_b; ++y) {
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
