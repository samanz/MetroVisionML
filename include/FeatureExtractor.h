/**
 * File: FeatureExtractor.h
 * Author: Sam
 * 
 * Created on April 23, 2010, 4:34 PM
*/

#include <vector>

#ifndef _CV_H
#define _CV_H
#include <cv.h>
#endif

using namespace std;

#ifndef _FEATURE_EXTRACTOR_H
#define _FEATURE_EXTRACTOR_H

/**
 * FeatureExtractor class
 * This class will be an abstract class, whose purpose is to provide an interface for performing Feature Extraction on an image. Any class that inherits this class must implement the extractFeatures function
 */
class FeatureExtractor
{
public:
	virtual int getSize() = 0;
	virtual vector<float> extractFeatures(IplImage * image, IplImage * segmented) = 0;
};
#endif /* _FEATURE_EXTRACTOR_H */
