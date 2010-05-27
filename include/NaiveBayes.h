/**
 * File: NaiveBayes.h
 * Author: Sam
 *
 * Created on May 20th, 2010, 3:49 PM
*/
#include "Classifier.h"
#include "cv.h"

using namespace std;

#ifndef _NAIVE_BAYES_H
#define _NAIVE_BAYES_H

/**
 * NaiveBayes class
 * This class implements a Naive Bayes algorithm. This algorithm is currently only implemented for discrete features.
 * TODO: Add gaussian distribution for continous features.
 * TODO: Add a loadModel method so that we can load models we previously saved.
 * It is created as a interface for the generic Classifier
*/
class NaiveBayes : public Classifier
{
private:
	// The Parameters of the Naive Bayes Algorithm:

	// The theta array (i,j,k) contains the probabiltiy that feature i has attribute j given classification k
	double *** theta; 

	// The pi array (i) contains the probabiliy of classification i 
	double * pi;

	// Size of multinomial for each feature
	int posValues;

	// Amount of classifications we are training on
	int posClassifications;

public:
	/**
	 * Default constructor
	*/
	NaiveBayes();
	
	/**
	 * Default destructor
	*/
	~NaiveBayes();

	/**
	 * The train function takes in the featureMatrix and classifications and trains the parameters of the
	 * Naive Bayes classifier.
	 * @param featuresMatrix the features matrix
	 * @param responses the classifications assocaited with each row of the featureMatrix
	 * @param update optional parameter that determines if the classifier is updates or retrained on featureMatrix
	*/
	void train(CvMat * featureMatrix, CvMat * responses, bool update = false);

	/**
	 * Classify takes in a featureMatrix and updates the returnMatrix with classifications for each row
	 * If the featureMatrix has only one row, classify will return the classification
	 * @param featureMatrix a matrix of features that will be classified
	 * @param returnMatrix a matrix of response classifications for each row of featureMatrix
	*/
	float classify(CvMat * featureMatrix, CvMat * returnMatrix = 0);

	/**
	 * Save the model! Because we don't want to always retrain the data to classify, right?
	 * @param filename the filename of the file the model is saved to
	 * @param name the name of the model 0=default
	*/
	void saveModel(const char * filename, const char* name);
};
#endif /* _NAIVE_BAYES_H */
