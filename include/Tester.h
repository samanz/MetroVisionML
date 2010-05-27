/**
 * File: Tester.h
 * Author: Sam
 * 
 * Created on May 1st, 2010, 10:41 PM
*/

#include "FeatureSet.h"
#include "FeaturesExtractor.h"
#include "Classifier.h"

using namespace std;

#ifndef _TESTER_H
#define TESTER_H

/**
 * Tester Class
 * This class will actually perform the classification using the dataset and will eventually perform some predefined
 * Level of cross-validation. Ideally, this class will output f-measure and ROC curve performance measures!
 * Currently, it outputs the accuracy value after cross-validation.
*/
class Tester {
private:
	Classifier * classifier;
	FeatureSet * featureSet;
public:
	/**
	 * Constructor accepts the classifier and the featureSet to train and test on
  	 * @param theClassifier the classifer used to do the training
	 * @param theFeatureSet the featureset we are training on
	*/
	Tester(Classifier * theClassifer, FeatureSet * theFeatureSet);
	/**
	 * Destructor
	*/
	~Tester();
	
	/**
	 * Test starts the testing process by training the model and testing on a portion of the training data
	 * TODO: Add a param for the amount of cross-validations
	*/
	void test();
};
#endif /* _TESTER_H */
