#include "FeaturesExtractor.h"

FeaturesExtractor::FeaturesExtractor() {
}

void FeaturesExtractor::addFeatureExtractor(FeatureExtractor * extractor) {
	extractors.push_back(extractor);
}

vector<float> FeaturesExtractor::extractFeatures(IplImage * image, IplImage * segmented) {
	// Number of feature extractors
	int size = extractors.size();
	
	vector<float> features;
	
	// Iterates over all the feature extractors and assigns each extracted feature to the
	// Resulting feature vector
	for(int index=0; index<size; index++) {
		// POINT: I like java generics, not c++ templates.
		// So I don't know how to make sure that these extractors provide extractFeatures method
		// TODO: Figure out how to make sure extractFeatures work
		vector<float> tempVector = extractors.at(index)->extractFeatures(image, segmented);
		for(int i=0; i<tempVector.size(); i++)
			features.push_back(tempVector.at(i));	
	}
	return features;
}

int FeaturesExtractor::getFeatureVectorSize() {
	int size=0;
	for(int index=0;index<extractors.size();index++)
		size += extractors.at(index)->getSize();
	return size;
}
FeaturesExtractor::~FeaturesExtractor() {
	// Destructor deallocates all the feature extractors
	for(int index=0; index<extractors.size(); index++) {
		delete extractors.at(index);	
	}
}

