#include "DBDataset.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>

int main(int argc, char** argv) {
	if(argc != 7) {
		cout << "Usage: DBDatasetTest [database name] [server address] [database username] [database password] [database port] [dataset name]" << endl;
		return 0;
	}
	DBDataset * dataset = new DBDataset(argv[1], argv[2], argv[3], argv[4], atoi(argv[5]), argv[6]);
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
		cvReleaseImage( &resized );
		cvReleaseImage( &image );
	}	
}
