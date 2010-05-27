#include "NaiveBayes.h"
#include <iostream>

NaiveBayes::NaiveBayes() {}

NaiveBayes::~NaiveBayes() {
	if(theta)
		delete [] theta;
	if(pi)
		delete [] pi;
}

void NaiveBayes::train(CvMat * featureMatrix, CvMat * responses, bool update) {
	// Smoothing Value
	int l = 4;	

	// Okay, here we go.
	// Since we don't really know all the possible values for our featureVector
	// We need to find the largest value for all features, but our data never exceeds 15.. so hackish.. for now?
	posValues = 19;

	// We will start with getting the count of the classifications seen
	vector<int> classifications;
	for(int i=0; i<responses->rows; i++) {
		int klass = (int)cvGetReal2D(responses, i, 0);
		// Add vector indecies for the classes we have seen
		for(int j=(klass+1); j>classifications.size();)
			classifications.push_back(0);
		// Increment the count for the class we have just seen
		classifications[klass] = classifications.at(klass)+1;
	}
	posClassifications = classifications.size();
	
	theta = new double**[featureMatrix->rows];
	// Initialize our theta three dimentional array:
	for( int i=0; i<featureMatrix->rows; i++) {
		theta[i] = new double*[posValues];
		for(int j=0; j<posValues; j++) {
			theta[i][j] = new double[posClassifications];
			for(int k=0; k<posClassifications; k++)
				theta[i][j][k] = l;
		}
	}

	// So um... we want to interate over every row in the featureMatrix first.
	// Then we want to count how many times we see an feature in the vector with the classification
	for( int i=0; i<featureMatrix->rows; i++) {
		for(int j=0; j<featureMatrix->cols; j++)
			theta[j][(int)cvGetReal2D(featureMatrix, i, j)][(int)cvGetReal2D(responses, i, 0)]++;
	}

	// Divide each element by the amount of its classification we have seen
	for( int i=0; i<featureMatrix->rows; i++) {
		for( int j=0; j<posValues; j++) {
			for( int k=0; k<posClassifications; k++)
				theta[i][j][k] /= ((double)classifications[k] + (l*posValues));
		}
	}

	// Initial pi value
	pi = new double[posClassifications];
	
	// Train the pi value -> #D{Y=yk}/|D|
	for(int i=0; i<posClassifications; i++)
		pi[i] = ((double)classifications[i])/((double)featureMatrix->rows);
		
	cout << "i: " << featureMatrix->rows << " j: " << posValues << " k: " << posClassifications << endl;
}

float NaiveBayes::classify(CvMat * featureMatrix, CvMat * returnMatrix) {
	int klass = 0;
	double estimation;
	double highestEstimation = -1;
	for( int i=0; i<posClassifications; i++) {
		estimation = pi[i]; 
		for( int j=0; j<featureMatrix->cols; j++ ) 
			estimation *= theta[j][(int)cvGetReal2D(featureMatrix, 0, j)][i]; 
		
		if(estimation > highestEstimation) {
			klass = i;
			highestEstimation = estimation;
		}
	}
	return (float)klass;
	//return 1;
}	

void NaiveBayes::saveModel(const char * filename, const char * name) {}
