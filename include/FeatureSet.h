/**
 * File: FeatureSet.h
 * Author: Sam
 *
 * Created on April 23, 2010, 5:18 PM
*/

#include <vector>
#include "FeaturesExtractor.h"
#include "Dataset.h"
#ifndef _CV_H
#define _CV_H
#include <cv.h>
#endif

using namespace std;

#ifndef _FEATURE_SET_H
#define _FEATURE_SET_H

/**
 * FeatureSet Class
 * This class will represent a set of feature vectors stored as a CvMat matrix. The feature set also takes in a FeaturesExtractor Object, and it will perform the feature extraction for the entire dataset and store it in the Matrix
 */
class FeatureSet {
private:
	FeaturesExtractor * extractor;
	vector<string> classifications;
	CvMat * featureVectors;
	CvMat * responses;
public:
	/**
	 * Default Constructor 
 	*/
	FeatureSet();
	/**
	 * Destructor
	*/
	~FeatureSet();
	/**
  	 * Constructor takes in the FeaturesExtractor
	*/
	FeatureSet(FeaturesExtractor * theExtractor);
	/**
	 * initializes the matricies
	 * @param datasetSize the size of each featureVector
	*/
	void initializeMatrix(int datasetSize);
	/**
	 * Actually extracts all the features from the dataset
 	 * This function has the real potential to take
	 * Huuuuuuuge amounts of time to run.
	 * @param dataset The dataset that we want to extract features from
	*/
	void extractFeatures(Dataset * dataset);
	
	/**
	 * Sets the features extractor that one want to use to do the
	 * Feature Extraction
	 * @param theExtractor a FeaturesExtractor object
	*/ 
	void setFeaturesExtractor(FeaturesExtractor * theExtractor);

	/**
	 * Gets the classification id of the string of classification 
	 * @param classification string 
	*/	
	short getClassification(string classification);
	
	/**
	 * Returns the Feature Matrix 
	*/
	CvMat * getFeatureMatrix();
	
	/**
	 * Returns the Class Matrix
	*/
	CvMat * getClassMatrix();
};
#endif 
