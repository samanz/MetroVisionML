#include "AngleHistogramExtractor.h"
#include "highgui.h"
#include "cv.h"
#include <iostream>

/** This helper function finds the cosine of an angle between two vectors
   from pt0 to pt1 and then pt0 to pt2. Used by extractFeatures to get angle feature vector
   From OpenCV examples.
 */
double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 )
{
    double dx1 = pt1->x - pt0->x;
    double dy1 = pt1->y - pt0->y;
    double dx2 = pt2->x - pt0->x;
    double dy2 = pt2->y - pt0->y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

AngleHistogramExtractor::AngleHistogramExtractor() {
	g_image = NULL; g_gray = NULL; g_thresh = 100; g_storage = NULL;
}

int AngleHistogramExtractor::getSize() {
	return 9;	
}

vector<float> AngleHistogramExtractor::extractFeatures(IplImage * image, IplImage * segmented) {
	g_image = image;
	
	/* For Debugging Purposes: Show the window with the images in them */
	cvNamedWindow( "Images", 2);	
	cvShowImage("Images", g_image);
	cvShowImage("Segmentation", segmented);

	/* We'll create some storage structures to store the contours we get later */
	CvMemStorage * store = cvCreateMemStorage(0);
	CvSeq * comp = NULL;
	
	IplImage * sixforty = cvCreateImage( cvGetSize(image), 8 , 1);
	cvResize(segmented, sixforty);
	
	CvSeq * first_contour = NULL;
	CvMemStorage * g_storage = cvCreateMemStorage();
	
	/* Perform the contour finding */
	cvFindContours( sixforty, g_storage, &first_contour, sizeof(CvContour), CV_RETR_LIST );
	
	/* Find the contour with the largest area
	   This contour has the highest likelyhood of surrounding the object we care about */
	CvSeq * largest;
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
	cvDrawContours(view, largest, cvScalarAll(255), cvScalarAll(255), 0, 2, 8);
	cvShowImage( "View", view);
	
	/* Polygonal Approximation */
	CvSeq * result; // Will hold approx
	CvMemStorage * storage = cvCreateMemStorage();
	result = cvApproxPoly( 	largest, 
				sizeof(CvContour),
				storage,
				CV_POLY_APPROX_DP,
				cvContourPerimeter(largest)*0.01 
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
	cvShowImage( "Result", view2 );
	
	cvWaitKey();
	
	/* Until we actually can send out a real feature vector: export a dummy */
	vector<float> features;
	features.push_back(result->total);
	double s=0;
	int i=0;
	for(i=0; i<8 && i<result->total; i++){
		s= fabs(angle(
                            (CvPoint*)cvGetSeqElem( result, i ),
                            (CvPoint*)cvGetSeqElem( result, i-2 ),
                            (CvPoint*)cvGetSeqElem( result, i-1 )));
		features.push_back(s);
	}
	for(; i<8; i++)
		features.push_back(0.00);	
	cout << features.size() << " << feature size " << endl;	
	return features;
}
