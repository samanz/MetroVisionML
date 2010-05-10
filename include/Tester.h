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
*/
class Tester {
private:
	Classifier * classifier;
	FeatureSet * featureSet;
	Dataset * dataset;
public:
	/**
	 * Constructor accepts the classifier and the featureSet to train and test on
  	 * @param theClassifier the classifer used to do the training
	 * @param theFeatureSet the featureset we are training on
	*/
	Tester(Classifier * theClassifer, FeatureSet * theFeatureSet, Dataset * theDataset);
	/**
	 * Destructor
	*/
	~Tester();
	
	/**
	 * Test starts the testing process by training the model and testing on a portion of the training data
	 * TODO: Add a number for the cross-validation and perform the training.
	*/
	void test();
};
#endif /* _TESTER_H */
