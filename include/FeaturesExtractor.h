/**
 * File: FeaturesExtractor.h
 * Author: Sam
 *
 * Created on April 21, 2010, 5:51 PM
*/

#include <vector>
 
#ifndef _CV_H
#define _CV_H 
#include <cv.h>
#endif

#include "FeatureExtractor.h"

using namespace std;

#ifndef _FEATURES_EXTRACTOR_H
#define _FEATURES_EXTRACTOR_H

/**
 * FeaturesExtractors class
 * This class is used to extract a feature vector from a an image. It accepts a set of feature extractors objects and uses them to do the actual feature extraction. It main use is to output a feature vector for an image for use in a Machine Learning context.
 * The feature vector can be as simple as the image itself, which the machine learning algorithm will probably take a very long time to train on (but the end classifier does not have to be machine learning based). 
*/ 

class FeaturesExtractor
{
private:
	/**
	 * Vector containing pointers to extractor objects
	*/
	vector<FeatureExtractor *> extractors;

public:
	
	/**
	 * Default Constructor
	 */
	FeaturesExtractor();
	
	/**
	 * Destructor
	 */
	~FeaturesExtractor();	
	
	/**
	 * Extract all features using feature extractors and save it as an OpenCV matrix 
	 * \param image the image that is extracted
	 * \param featureVector the extracted feature vector
	*/
	vector<float> extractFeatures(IplImage * image, IplImage * segmented);
	
	/**
	 * Adds a feature extrator to the list of feature extractors for the class
	 * \param extractor a Feature Extractor object that extracts features
	 */
	void addFeatureExtractor(FeatureExtractor * extractor);
	
	/**
	 * Gets the size of the resulting feature vector
	*/
	int getFeatureVectorSize();
};
#endif /* _FEATURES_EXTRACTOR_H */
