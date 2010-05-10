#include "CvClassifyWrapper.h"
CvClassifyWrapper::CvClassifyWrapper(CvNormalBayesClassifier * theStatModel)  {
	StatModel = theStatModel;
}
CvClassifyWrapper::~CvClassifyWrapper() {
	delete StatModel;
}

void CvClassifyWrapper::train(CvMat * featureMatrix, CvMat * responses) {
	StatModel->train(featureMatrix, responses);
}

float CvClassifyWrapper::classify(CvMat * featureVector) {
	return StatModel->predict(featureVector);
} 

void CvClassifyWrapper::saveModel(const char * filename, const char * name) {
	StatModel->save(filename, name);
}
