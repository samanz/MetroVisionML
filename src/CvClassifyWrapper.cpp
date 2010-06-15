#include "CvClassifyWrapper.h"
CvClassifyWrapper::CvClassifyWrapper(CvNormalBayesClassifier * theStatModel)  {
	StatModel = theStatModel;
}
CvClassifyWrapper::~CvClassifyWrapper() {
	delete StatModel;
}

void CvClassifyWrapper::train(CvMat * featureMatrix, CvMat * responses, bool update) {
	StatModel->train(featureMatrix, responses, NULL, NULL, update);
}

float CvClassifyWrapper::classify(CvMat * featureMatrix, CvMat * returnMatrix) {
	return StatModel->predict(featureMatrix, returnMatrix);
} 

void CvClassifyWrapper::saveModel(const char * filename, const char * name) {
	StatModel->save(filename, name);
}

void CvClassifyWrapper::loadModel(const char * filename, const char * name) {
	StatModel->load(filename, name);
}
