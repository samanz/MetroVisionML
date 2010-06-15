#include "EdgeLengthHistogramExtractor.h"
#include "highgui.h"
#include "cv.h"
#include <iostream>
#include <math.h>

#define PI 3.14159265

/** This helper function find the length of a vector, Used to extract length data for features */
int length(CvPoint * pt1, CvPoint * pt2)
{
	int x = pow( (pt1->x-pt2->x), 2);
	int y = pow( (pt1->y-pt2->y), 2);
	return sqrt(x+y);
}

EdgeLengthHistogramExtractor::EdgeLengthHistogramExtractor() {
	g_image = NULL; g_gray = NULL; g_thresh = 100; g_storage = NULL;
}

int EdgeLengthHistogramExtractor::getSize() {
	return 8;	
}

vector<float> EdgeLengthHistogramExtractor::extractFeatures(IplImage * image, IplImage * segmented) {
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
		//cvShowImage( "View", view);
	
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
		IplImage * view2 = cvCreateImage( cvGetSize(sixforty), 8, 3);	
		cvZero(view2);
		cvDrawContours(view2, result, cvScalarAll(255), cvScalarAll(255), 0, 2, 8);
		//cvShowImage( "Result", view2 );
	
	//cvWaitKey();
	
		int bins = getSize();
		int * histogram = new int[bins];
		for(int i=0; i<bins; i++)
			histogram[i] = 0;

	/* Until we actually can send out a real feature vector: export a dummy */
 		vector<float> lengths;
		double s=0;
		int i=0;
		double per = cvContourPerimeter(result)/(result->total);
		for(i=0; i<result->total; i++){
			s = length(
                        	    (CvPoint*)cvGetSeqElem( result, i ),
                            	(CvPoint*)cvGetSeqElem( result, i-1 ) );
			s /= per;
			int bin_index = floor( (s*bins) );
			if(bin_index>(bins-1))
				bin_index=(bins-1);
			histogram[bin_index]++;
		}
 
		for(int i=0; i<bins; i++)
			features.push_back( histogram[i] );
	
		// Cleanup the temp files
		cvReleaseImage( &view2 );
		cvReleaseMemStorage( &storage );
	}	
	cvReleaseImage( &sixforty );
	cvReleaseImage( &view );
	cvReleaseMemStorage( &g_storage );
	return features;
}
