#include "FeatureSet.h"
#include "highgui.h"
#include <iostream>

IplImage * segmentImage(IplImage * image, YUVRange range) {
	
	IplImage * smaller = cvCreateImage( cvSize(208, 160), 8, 3);
	cvResize(image, smaller);
	
	IplImage * YUV_image = cvCreateImage( cvGetSize(smaller), 8, 3);
	cvCvtColor(smaller, YUV_image, CV_BGR2YCrCb);
	IplImage * y = cvCreateImage( cvGetSize(YUV_image), 8, 1);
	IplImage * u = cvCreateImage( cvGetSize(YUV_image), 8, 1);
	IplImage * v = cvCreateImage( cvGetSize(YUV_image), 8, 1);
	
	cvSplit( YUV_image, v, u, y, NULL);	

	IplImage * yUpThreshold = cvCreateImage( cvGetSize(y), 8, 1);
	IplImage * yDownThreshold = cvCreateImage( cvGetSize(y), 8, 1);
	cvThreshold(y, yUpThreshold, range.y_low, 255, CV_THRESH_BINARY);
	cvThreshold(y, yDownThreshold, range.y_high, 255, CV_THRESH_BINARY_INV);
	IplImage * yAnd = cvCreateImage( cvGetSize(y), 8, 1);
	cvAnd(yUpThreshold, yDownThreshold, yAnd);

	IplImage * uUpThreshold = cvCreateImage( cvGetSize(u), 8, 1);
	IplImage * uDownThreshold = cvCreateImage( cvGetSize(u), 8, 1);
	cvThreshold(u, uUpThreshold, range.u_low, 255, CV_THRESH_BINARY);
	cvThreshold(u, uDownThreshold, range.u_high, 255, CV_THRESH_BINARY_INV);
	IplImage * uAnd = cvCreateImage( cvGetSize(u), 8, 1);
	cvAnd(uUpThreshold, uDownThreshold, uAnd);

	IplImage * vUpThreshold = cvCreateImage( cvGetSize(v), 8, 1);
	IplImage * vDownThreshold = cvCreateImage( cvGetSize(v), 8, 1);
	cvThreshold(v, vUpThreshold, range.v_low, 255, CV_THRESH_BINARY);
	cvThreshold(v, vDownThreshold, range.v_high, 255, CV_THRESH_BINARY_INV);
	IplImage * vAnd = cvCreateImage( cvGetSize(v), 8, 1);
	cvAnd(vUpThreshold, vDownThreshold, vAnd);

	IplImage * total = cvCreateImage( cvGetSize(vAnd), 8, 1);
	cvAnd(yAnd, uAnd, total);
	cvAnd(total, vAnd, total);	

	return total;
}

FeatureSet::FeatureSet(FeaturesExtractor * theExtractor) : extractor(theExtractor) {}
FeatureSet::~FeatureSet() {
	if(extractor)
		delete extractor;
	if(featureVectors)
		delete featureVectors;
	if(responses)
		delete responses;
}

void FeatureSet::setFeaturesExtractor(FeaturesExtractor * theExtractor) {
	extractor = theExtractor;	
}

void FeatureSet::initializeMatrix(int datasetSize) {
	// Features matrix is size of datasetSize x vector size
	featureVectors = cvCreateMat(datasetSize, extractor->getFeatureVectorSize(), CV_32FC1);
	// Classification responses, 1 per dataset so datasetSize x 1
	responses = cvCreateMat(datasetSize, 1, CV_32SC1);
}

short FeatureSet::getClassification(string classification) {
	for(short i=0; i < classifications.size(); i++) {
		if(classification.compare(classifications.at(i)) == 0) return i;
	}
	classifications.push_back(classification);
	return (short)(classifications.size()-1);
}

void FeatureSet::extractFeatures(Dataset * dataset) {
	// Initalize the matrix
	int dataSize = dataset->size();
	initializeMatrix(dataSize);
	
	// Here we get each image, run the features extractor and store the matrix of features and classifications
	
	int image_index = 0;
	while(image_index < dataset->size()) {
		IplImage * image = dataset->getNextImage();
		string classification = dataset->getClassification();
		YUVRange range = dataset->getYUVRange();
		IplImage * segmented = segmentImage(image, range);
		vector<float> featureVector = extractor->extractFeatures(image, segmented);
		for(int i=0; i < featureVector.size(); i++) {
			// I'm sure there is a better way of doing here what I wanna do, but if it works?!	
			*( (float*)CV_MAT_ELEM_PTR( *featureVectors, image_index, i ) ) = featureVector.at(i);	
		}
		cout << "classification # of: " << getClassification(classification) << endl;
		*( (short*)CV_MAT_ELEM_PTR(*responses, image_index, 0) ) = getClassification(classification);
		image_index++;
		cout << image_index << " of " << dataset->size() << endl;
		cvReleaseImage(&image);
	}
}

CvMat * FeatureSet::getFeatureMatrix() { return featureVectors; }
CvMat * FeatureSet::getClassMatrix()   { return responses; } 
