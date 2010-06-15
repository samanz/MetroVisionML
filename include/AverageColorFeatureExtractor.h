/**
 * File: AverageColorFeatureExtractor.h
 * Author: Sam
 *
 * Created on June 4th, 11:11 AM
*/
#include "FeatureExtractor.h"
#include "cv.h"

using namespace std;

#ifndef _AVERAGE_COLOR_FEATURE_EXTRACTOR_H
#define _AVERAGE_COLOR_FEATURE_EXTRACTOR_H

/**
 * The Average Color Feature Extractor
 * The idea behind this one is that we want to destinguish the objects that we care about with object
 * That happen to be in the same YUV range as the objects that we are looking at. So we get the average
 * color of the object that we are looking for and we kinda want to reduce the volume (Correct terminology?)
 * or range of colors that can be used in order so that it can deal with invariance in lighting and other color
 * changing conditions. (Maybe we should forget about training on luminance? Test it out! K??
*/
class AverageColorFeatureExtractor: public FeatureExtractor {
private:
	IplImage * g_image;
	IplImage * g_gray;
	int g_thresh;
	CvMemStorage * g_storage;
public:
	/**
	 * Constructor
	*/
	AverageColorFeatureExtractor();

	/**
	 * Returns the size of the resulting feature vector 
	*/
	int getSize();

	/**
	 * Resturns the resulting feature vector containing the color features
	 * @param image an IplImage containing the image of the object we want to extract features from
	 * @param segmented an IplImage containing the segmentation using the YUV information we have
	*/
	vector<float> extractFeatures(IplImage * image, IplImage * segmented);
};
#endif /* AVERAGE_COLOR_FEATURE_EXTRACTOR_H */
