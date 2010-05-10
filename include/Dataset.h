/**
 * File: Dataset.h
 * Author: Sam
 * 
 * Created on April 23, 2010, 5:06 PM
*/
#include <cv.h>
#include <vector>

using namespace std;

#ifndef _DATASET_H
#define _DATASET_H

/**
 * Dataset class
 * This class will represent a dataset of images and their classification.
 * This is an abstract class. Any class that inherits this class must implement the getNextImage()
 * function, the getClassification() function, and the size(). The getNextImage() returns the next image in the set,
 * the getClassification() function returns the classification of the previously returned image.
*/
struct YUVRange {
	int y_low;
	int y_high;
	int u_low;
	int u_high;
	int v_low;
	int v_high;
};

class Dataset
{
public:
	virtual IplImage * getNextImage() {};
	virtual string getClassification() {};
	virtual int size() {};
	virtual YUVRange getYUVRange() {};
};
#endif
