#include "Tester.h"
#include <stdio.h>
#include <iostream>

void PrintMat(CvMat *A)
{
int i, j;
for (i = 0; i < A->rows; i++)
{
printf("\n"); 
switch (CV_MAT_DEPTH(A->type))
{
case CV_32F:
case CV_64F:
for (j = 0; j < A->cols; j++)
printf ("%8.3f ", (float)cvGetReal2D(A, i, j));
break;
case CV_8U:
case CV_16U:
for(j = 0; j < A->cols; j++)
printf ("%6d",(int)cvGetReal2D(A, i, j));
break;
case CV_32S:
for(j = 0; j < A->cols; j++)
printf("%hd", CV_MAT_ELEM(*A, short, i, j));
default:
break;
}
}
printf("\n");
}

Tester::Tester(Classifier * theClassifier, FeatureSet * theFeatureSet, Dataset * theDataset) {
	 classifier = theClassifier;
	 featureSet = theFeatureSet; 
	 dataset = theDataset;
};

Tester::~Tester() {
	delete classifier;
	delete featureSet;
}

void Tester::test() {
	cout << "Extracting Features:" << endl;
	featureSet->extractFeatures(dataset);
	cout << "Testing Matrix out" << endl;
	PrintMat(featureSet->getFeatureMatrix());
	PrintMat(featureSet->getClassMatrix());
	classifier->train(featureSet->getFeatureMatrix(), featureSet->getClassMatrix());
	// TODO: Add prediction and testing methods	
}
