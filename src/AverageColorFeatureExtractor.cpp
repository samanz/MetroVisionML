#include "AverageColorFeatureExtractor.h"
#include "highgui.h"
#include "cv.h"
#include <iostream>
#include <math.h>

AverageColorFeatureExtractor::AverageColorFeatureExtractor() {
	g_image = NULL; g_gray = NULL; g_thresh = 100; g_storage = NULL;
}

int AverageColorFeatureExtractor::getSize() {
	return 3;
}

vector<float> AverageColorFeatureExtractor::extractFeatures(IplImage * image, IplImage * segmented) {
	g_image = image;
	
	/* For Debugging Purposes: Show the window with the images in them */
	cvNamedWindow( "Images", 2);	
	//cvShowImage("Images", g_image);
	//cvShowImage("Segmentation", segmented);

	/* We'll create some storage structures to store the contours we get later */
	
	IplImage * sixforty = cvCreateImage( cvGetSize(image), 8 , 1);
	cvResize(segmented, sixforty);
	
	CvSeq * first_contour = NULL;
	CvMemStorage * g_storage = cvCreateMemStorage();
	
	/* Perform the contour finding */
	cvFindContours( sixforty, g_storage, &first_contour, sizeof(CvContour), CV_RETR_LIST );
	
	/* Find the contour with the largest area
	   This contour has the highest likelyhood of surrounding the object we care about */
	CvSeq * largest = 0;
	int l_area = 0;

	for(CvSeq * c=first_contour; c!=NULL; c=c->h_next ){
	CvRect rect = cvBoundingRect( c );
		if(rect.width*rect.height > l_area) {
			l_area = rect.width*rect.height;
			largest = c;
		}
	}

	/* For Debugging purposes: create image to see resulting contour */
	IplImage * view = cvCreateImage( cvGetSize(sixforty), 8, 3);	
	cvZero(view);
	
	vector<float> features;
	
	if(largest) {
		cvDrawContours(view, largest, cvScalarAll(255), cvScalarAll(255), 0, 2, 8);
		cvShowImage( "View", view);
	
		/* Polygonal Approximation */
		CvSeq * result; // Will hold approx
		CvMemStorage * storage = cvCreateMemStorage();
		result = cvApproxPoly( 	largest, 
				sizeof(CvContour),
				storage,
				CV_POLY_APPROX_DP,
				cvContourPerimeter(largest)*0.015 
				);
	
	/*
  	   The parameter value above (set to perimeter * 0.01 ) found by experimentation
	   The value is smaller than the one used for L shape or the square finder
	   Because we wan't some element of noisyness. (It determines when the Algorithm stops adding points)
	*/

	/* For Debugging purposes: create image to see resulting contour */
		IplImage * mask = cvCreateImage( cvGetSize(sixforty), IPL_DEPTH_8U, 1);	
		cvZero(mask);
		cvDrawContours(mask, result, cvScalarAll(255), cvScalarAll(255), 0, -1, 8);
		IplImage * sendMask = cvCreateImage (cvGetSize(image), IPL_DEPTH_8U, 1);
		cvResize(mask, sendMask);
		//cvShowImage( "Result", sendMask );

		cout << image->nChannels << " " << image->imageSize << " " << sendMask->imageSize << " " << sendMask->depth << endl;	
	
		CvScalar avg = cvAvg( image, sendMask );	
	
		//cvWaitKey();

		/* Until we actually can send out a real feature vector: export a dummy */
	
	
		//for(int i=0; i<bins; i++)
		//	features.push_back( histogram[i] );
	
		features.push_back(floor((19*avg.val[0])/255));
		features.push_back(floor((19*avg.val[1])/255));
		features.push_back(floor((19*avg.val[2])/255));

		// Cleanup the temp files
		cvReleaseImage( &mask );
		cvReleaseImage( &sendMask );
		cvReleaseMemStorage( &storage );
	}
	
	cvReleaseImage( &view );
	cvReleaseImage( &sixforty );
	cvReleaseMemStorage( &g_storage );
	return features;
}

