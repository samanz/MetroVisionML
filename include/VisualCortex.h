/*
 * File: VisualCortex.h
 * Author: Sam
 *
 * Created: June 8th 2010
*/

#include "AngleHistogramExtractor.h"
#include "AverageColorFeatureExtractor.h"
#include "EdgeLengthHistogramExtractor.h"
#include "Dataset.h"
#include "Classifier.h"
#include "CvClassifyWrapper.h"
#include "cv.h"
#include "ml.h"
#include "cxcore.h"	
#include <iostream>
#include <fstream>
#include "yaml-cpp/yaml.h"

using namespace std;

#ifndef _VISUAL_CORTEX_H
#define _VISUAL_CORTEX_H

/**
  * The Visual Cortex Class
  * So well, this class is the end result of the program.
  * If you made it this far looking at the classes, trying to understand everything:
  * Congratulations! This actually does something interesting!
  * As the name suggests, this is the brain, the smarts, the pathways of heaven of the visual
  * system for a robot. This should actually classify the objects contained in a photo.
  * This system takes in the model as a filename, and builds a classifer. It also takes in photos, 
  * creates the feature vector for the photo, and tries to classify it.
*/
class VisualCortex {
private:
	/* 
	   The list of feature extractors used for the classification. Should be exactly like the
	   extractors used to create the model
 	*/
	FeaturesExtractor * _extractor;
	
	// The classifier used to create the model for the classification. Should be the same one used to create model.
	Classifier * _classifier;

	//The YUV ranges used in the model
	YUVRanges _ranges;
public:
	/**
	 * Constructor
	 * @param classifier the classifier we want our visual cortex to use
  	 * @param extractor the features extractor we want to use for this cortex
	 * @param theModel the model file to load
	*/
	VisualCortex(Classifier * classifier, FeaturesExtractor * extractor, const char * theModel);
	
	/** 
	 * Classify classifies an actual photo
	 * @param image IplImage that we want to classify
	 * @returns the string of the classification
	*/
	string classify(IplImage * image);
}; 
#endif /* _VISUAL_CORTEX_H */
