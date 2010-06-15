/*
 * File playerClassify.cpp
 * Author: Sam
 * Description: Classifies the image that arrived via player
 *
 * Created on April 16th 2010, 5:11 PM
*/

#ifndef _PLAYER_
#define _PLAYER_
#include <libplayerc++/playerc++.h>
#include <libplayerjpeg/playerjpeg.h>
#include "VisualCortex.h"
#endif

#include <cstdio>
#include "highgui.h"
#include "cv.h"
#include "cxcore.h"
using namespace PlayerCc;


PlayerClient	robot("localhost", 6665);
CameraProxy	cp(&robot,0);

int main(int argc, char **argv)
{
    if(argc != 2) {
	cout << "Usage: classify [model to load] " << endl;
	return 0;
    }
    FeatureExtractor * angleExtractor = new AngleHistogramExtractor();
    FeatureExtractor * edgeExtractor = new EdgeLengthHistogramExtractor();
    FeatureExtractor * colorExtractor = new AverageColorFeatureExtractor();
    FeaturesExtractor * extractor = new FeaturesExtractor();
    extractor->addFeatureExtractor(colorExtractor);
    extractor->addFeatureExtractor(angleExtractor);
    extractor->addFeatureExtractor(edgeExtractor);
    CvNormalBayesClassifier * normalBayes = new CvNormalBayesClassifier();
    Classifier * normalBayesWrapper = new CvClassifyWrapper(normalBayes);
    VisualCortex * cortex = new VisualCortex(normalBayesWrapper, extractor, argv[1]); 	  
	cvNamedWindow( "Example2", CV_WINDOW_AUTOSIZE );
	int num=0;
	while(1) {
	 	robot.ReadIfWaiting();
		if(cp.IsFresh()) {
			int image_count = cp.GetImageSize();
			printf("image_count: %d\n", image_count);
				
			uint8_t * buffer = new uint8_t[image_count];
	
			// Set width and height
			int image_width = cp.GetWidth();
			int image_height = cp.GetHeight();
			
			uint8_t * b2 = new uint8_t[image_width*image_height*3];
 	
	
			IplImage * frame = cvCreateImage(cvSize(image_width, image_height), 8, 3);
		
		cp.GetImage(buffer);
	
		jpeg_decompress(b2, (image_width*image_height*3), buffer, image_count);
		// Copy data from the image buffer into frame & convert RGB to BGR
		// Look around for faster dense matrix twist transformation
		for (int i = 0; i < image_width; i++){
			for (int j = 0; j < image_height; j++){
				frame->imageData[image_width * j*3 + i*3 + 0] =
			       	(char)b2[image_width * j*3 + i*3 + 2];
				frame->imageData[image_width * j*3 + i*3 + 1] =
			       	(char)b2[image_width * j*3 + i*3 + 1];
				frame->imageData[image_width * j*3 + i*3 + 2] =
			       	(char)b2[image_width * j*3 + i*3 + 0];
			}
		}

	        cvShowImage( "Example2", frame );     		
		char c = cvWaitKey(33);
        	if( c == 27 ) break;
		int n;
		switch (c) {
			case 'c':
				char  buffer[20];
				n = sprintf(buffer, "rgbh%.4d.bmp", num);
				cvSaveImage(buffer, frame);
				num++; 
				break;
			case 'w':
				cout << cortex->classify(frame) << endl;
				break;
		}
		cp.NotFresh();
		delete [] b2;
		delete [] buffer;
		cvReleaseImage( &frame ); 
	}
	}
    	cvDestroyWindow( "Example2" );
}
