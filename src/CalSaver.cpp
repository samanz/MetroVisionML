#include "DBDataset.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>

int main(int argc, char** argv) {
	DBDataset * dataset = new DBDataset("metrovision", "localhost", "root", "piyrwq1!", 3306, "iPhone");
	for(int i=0; i< dataset->size(); i++) {
	IplImage * image = dataset->getNextImage();
		IplImage * resized = cvCreateImage( cvSize(208, 160), 8, 3);
		char  buffer[20];
		cvResize(image, resized);
		int n = sprintf(buffer, "rgbh%.4d.bmp", i);
		cvSaveImage(buffer, resized);
		IplImage * YUV_image = cvCreateImage( cvGetSize(resized), 8, 3);
		cvCvtColor(resized, YUV_image, CV_BGR2YCrCb);
		n = sprintf(buffer, "ycrcb%.3d.bmp", i);
		cvSaveImage(buffer, YUV_image);
	}	
}
