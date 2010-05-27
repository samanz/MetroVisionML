/**
 * File: CvClassifyWrapper.h
 * Author: Sam
 *
 * Created on April 30, 2010, 6:16 PM
*/
#include "Classifier.h"
#include "cv.h"
#include "ml.h"

using namespace std;

#ifndef _CV_CLASSIFY_WRAPPER_H
#define _CV_CLASSIFY_WRAPPER_H

/**
 * CvClassifyWrapper class 
 * This class wraps the CvStatModel and provides it as an interface for the generic Classifier
*/
class CvClassifyWrapper : public Classifier
{
private:
	/** 
	 * The stored CvStatModel that we will be wrapping and training on! 
	*/
	CvNormalBayesClassifier * StatModel;
public:
	/**
	 * Constructor takes in the CvStatModel to train on
	*/
	CvClassifyWrapper(CvNormalBayesClassifier * theStatModel);	
	/**
	 * Destructor
	*/
	~CvClassifyWrapper();	
	/**
	 * Train is a wrapper for the CvStatModel train method implemented on the Classifier interface
	 * @param featuresMatrix the features matrix
	 * @param responses the classifications associated with each row of the featureMatrix
	*/
	void train(CvMat * featureMatrix, CvMat * responses, bool update = false);
	
	/**
	 * Classify is a wrapper for the CvStatModel predict method implemented on the Classifer interface
	 * @param featureVector the feature vector that we are classifying 
	*/
	float classify(CvMat * featureMatrix, CvMat * returnMatrix = 0);

	/**
	 * Save the model!
	 * @param filename the filename of the file the model is saved to
	 * @param name the name of the model 0=default
	*/
	void saveModel(const char* filename, const char* name=0);	
};
#endif /* _CV_CLASSIFY_WRAPPER_H */
