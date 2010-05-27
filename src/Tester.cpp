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

Tester::Tester(Classifier * theClassifier, FeatureSet * theFeatureSet) {
	 classifier = theClassifier;
	 featureSet = theFeatureSet; 
};

Tester::~Tester() {
	delete classifier;
	delete featureSet;
}

void Tester::test() {
	int CROSS = 5;
	cout << "Extracting Features:" << endl;
	cout << "Testing Matrix out" << endl;
	
	featureSet->saveMatrix("features.txt", "responses.txt");
	//PrintMat(featureSet->getFeatureMatrix());
	//PrintMat(featureSet->getClassMatrix());

	CvMat * featureMatrix = featureSet->getFeatureMatrix();
	CvMat * classMatrix = featureSet->getClassMatrix();

	CvMat trainData1, trainData2, testData, trainClasses1, trainClasses2, testClasses;
	int datasize = featureMatrix->rows;
	
	int testsize = datasize/CROSS;
	int trainsize = datasize-testsize;
	// Get the start and end indices of the test data for n-way cross-training
	int teststart=0;
	int testend=testsize;

	cout << "Size of the dataset: " << datasize << endl;	
	cout << "Size of the training data: " << trainsize << endl;	

	int * way_accuracy = new int[CROSS];
	for(int way=0; way<CROSS; way++) {
		

		// Segment the featureMatrix into training data, part 1
		cvGetRows(featureMatrix, &trainData1, 0, teststart);
		cvGetRows(classMatrix, &trainClasses1, 0, teststart);

		// Segment the featureMatrix into training data, part 2
		cvGetRows(featureMatrix, &trainData2, testend, datasize);
		cvGetRows(classMatrix, &trainClasses2, testend, datasize);

		// Segment into test data
		cvGetRows(featureMatrix, &testData, teststart, testend);
		cvGetRows(classMatrix, &testClasses, teststart, testend);

		int rows = trainData1.rows + trainData2.rows;		
		CvMat * allTrain = cvCreateMat(rows, featureMatrix->cols, CV_32FC1);
		CvMat * allResponses = cvCreateMat(rows, 1, CV_32FC1);

		cvZero(allTrain);
		cvZero(allResponses);
		
		int rowIndex = 0;
		for(int i=0; i< trainData1.rows; i++) {
			for(int j=0; j<trainData1.cols; j++)
				cvSetReal2D(allTrain, rowIndex, j, cvGetReal2D(&trainData1, i, j));
			cvSetReal2D(allResponses, rowIndex, 0, cvGetReal2D(&trainClasses1, i, 0));
			rowIndex++;
		}

		for(int i=0; i< trainData2.rows; i++) {
			for(int j=0; j< trainData2.cols; j++)
				cvSetReal2D(allTrain, rowIndex, j, cvGetReal2D(&trainData2, i, j));
			cvSetReal2D(allResponses, rowIndex, 0, cvGetReal2D(&trainClasses2, i, 0));
			rowIndex++;
		}
							
		/*
		// Train the model, the first segment of training data will be empty on first run
		if(way>0)
			classifier->train(&trainData1, &trainClasses1);
		else
			classifier->train(&trainData2, &trainClasses2);

		// Update the model, the last segment of traning data will be empty on last run
		if(way>0 && way<(CROSS-1))
			classifier->train(&trainData2, &trainClasses2, true);
		*/
		classifier->train(allTrain, allResponses);
		
		double num_correct = 0.0;
	
		for(int i=0; i<testsize-1; i++) {
			CvMat row;
			cvGetRow(&testData, &row, i);	
		
			float result = (float) classifier->classify(&row);
			float actual = (float) CV_MAT_ELEM( testClasses, float, i, 0); 
	
			cout << "For result " << i << ":" << endl;
			cout << "Result: " << result << endl;
			cout << "Actual: " << actual << endl;
	
			if( result == actual )
				num_correct++;
		}

		way_accuracy[way] = num_correct;

		cout << "For Iteration : " << way << " Accuracy: " << (num_correct/testsize) << endl;
		teststart += testsize;
		testend += testsize;
	}
	int total = 0;
	for(int i=0; i<CROSS; i++)
		total += way_accuracy[i];

	cout << "Total Accuracy: " << (float)((float)total/(float)CROSS)/((float)testsize) << endl;
}
