/**
 * File: Classifier.h
 * Author: Sam
 *
 * Created on April 30, 2010, 5:49 PM
*/
#include <cv.h>
#include "FeatureSet.h"

using namespace std;

#ifndef _CLASSIFIER_H
#define _CLASSIFIER_H

/**
 * Classifier class
 * This class is a abstract classifier because I love the concept of polymorphism
 * Mostly, the purpose of this class is to provide an interface for supervised classification algorithms
*/

class Classifier
{
public:
	/**
	 * Trains the classifier algorithm.
 	 * @param featuresMatrix the features matrix
	 * @param responses the classifications associated with each row of the featureMatrix
	*/
	virtual void train(CvMat * featuresMatrix, CvMat * responses) {};
	virtual short classify(vector<float> * featureVector) {};
	virtual void saveModel(const char* filename, const char* name=0) {};
};
#endif
