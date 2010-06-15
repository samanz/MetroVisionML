/**
 * File: EdgeLengthHistogramExtractor.h
 * Author: Sam
 * 
 * Created on June 2nd, 10:27 PM
*/
#include "FeatureExtractor.h"
#include "cv.h"

using namespace std;

#ifndef _EDGE_LENGTH_HISTOGRAM_EXTRACTOR_H
#define _EDGE_LENGTH_HISTOGRAM_EXTRACTOR_H

/**
 * The Edge Length Histogram Extractor
 * This is the real deal here. In theory (My hypothesis until tested: kinda the point of this program) is that
 * this class should get an IplImage and return a histogram of edge distributions of the object within the image.
 * WHY? Because I'd like to test it... Thing is this computes the polygonal estimation again, but I want to keep it
 * Seperate for now for testing sake. Maybe, maybe, maybe... I can chain these togather so that I can develop 
 * a system that can extract features and pass intermediate steps to other feature extractors?
*/
class EdgeLengthHistogramExtractor: public FeatureExtractor {
private:
	IplImage * g_image;
	IplImage * g_gray;
	int g_thresh;
	CvMemStorage * g_storage;
public:
	/**
	 * Constructor
	*/
	EdgeLengthHistogramExtractor();
	
	/**
	 * Returns the size of the resulting feature vector
	*/
	int getSize();
	/**
	 * Returns the resulting feature vector containing the angle histogram
	 * @param image an IplImage containing the image of the object we want to extract features from
	*/
	vector<float> extractFeatures(IplImage * image, IplImage * segmented);
};
#endif /* _EDGE_LENGTH_HISTOGRAM_EXTRACTOR_H */
