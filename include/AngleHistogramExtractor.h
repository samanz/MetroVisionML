/**
 * File: AngleHistogramExtractor.h
 * Author: Sam
 * 
 * Created on May 4th, 10:27 PM
*/
#include "FeatureExtractor.h"
#include "cv.h"

using namespace std;

#ifndef _ANGLE_HISTOGRAM_EXTRACTOR_H
#define _ANGLE_HISTOGRAM_EXTRACTOR_H

/**
 * The Angle Histogram Extractor
 * This is the real deal here. In theory (My hypothesis until tested: kinda the point of this program) is that
 * this class should get an IplImage and return a histogram of angle distributions of the object within the image.
 * The idea is that this histogram should certainly be rotationaly invariant within the first 4 Degrees of Freedom
 * of a rigid body (We are not dealing with object recognition of non-rigid bodies (wounld not ever be able to
 * think about where to start with that one)). For the other two, the angles should only change at the extremes,
 * from which the shapes of many of the object are the same anyways, and other feature vectors such as template
 * matching and color features should be added. Besides for invariance in modification of DOF, the requirement
 * is, I think, kept that these angles should be distict for each object. :-)
*/
class AngleHistogramExtractor: public FeatureExtractor {
private:
	IplImage * g_image;
	IplImage * g_gray;
	int g_thresh;
	CvMemStorage * g_storage;
public:
	/**
	 * Constructor
	*/
	AngleHistogramExtractor();
	
	/**
	 * Returns the size of the resulting feature vector
	*/
	int getSize();

	void on_trackbar(int);
	/**
	 * Returns the resulting feature vector containing the angle histogram
	 * @param image an IplImage containing the image of the object we want to extract features from
	*/
	vector<float> extractFeatures(IplImage * image, IplImage * segmented);
};
#endif /* ANGLE_HISTOGRAM_EXTRACTOR_H */
