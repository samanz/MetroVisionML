/**
 * File: FeatureExtractorTest.h
 * Author: Sam
 *
 * Created on May 1st 11:46 PM
*/
#include "FeatureExtractor.h"

using namespace std;

#ifndef _FEATURE_EXTRACTOR_TEST_H
#define _FEATURE_EXTRACTOR_TEST_H

/**
 * Feature Extractor Test
 * This is a test feature extractor, dummy really.
 * The purpose is to test the functionality of the rest of the code without implementing actual feature extraction
 * Does not do anything with the image it gets, it just outputs a random vector when called
*/

class FeatureExtractorTest: public FeatureExtractor {
public:
	FeatureExtractorTest();
	int getSize();
	vector<float> extractFeatures(IplImage * image, IplImage * segmented);
};
#endif /* _FEATURE_EXTRACTOR_TEST_H */ 
