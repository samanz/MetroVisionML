#include "FeatureExtractorTest.h"

FeatureExtractorTest::FeatureExtractorTest() {
}

int FeatureExtractorTest::getSize() {
	return 2;
}

vector<float> FeatureExtractorTest::extractFeatures(IplImage * image, IplImage * segmented) {
	vector<float> featureVector;
	featureVector.push_back(0.00);
	featureVector.push_back(1.00);
	return featureVector; 
}
