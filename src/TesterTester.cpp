#include "DBDataset.h"
#include "FeatureExtractorTest.h"
#include "FeaturesExtractor.h"
#include "CvClassifyWrapper.h"
#include "Tester.h"
#include "cv.h"

int main(int argc, char ** argv) {
	if(argc != 7) {
		cout << "Usage: DBDatasetTest [database name] [server address] [database username] [database password] [database port] [dataset name]" << endl;
		return 0;
	}
	DBDataset * db_handle = new DBDataset(argv[1], argv[2], argv[3], argv[4], atoi(argv[5]), argv[6]);
	FeatureExtractor * featureTest = new FeatureExtractorTest();
	FeaturesExtractor * extractor = new FeaturesExtractor();
	extractor->addFeatureExtractor(featureTest);
	FeatureSet * featureSet = new FeatureSet(extractor);
	featureSet->extractFeatures(dataset);
	CvNormalBayesClassifier * normalBayes = new CvNormalBayesClassifier();
	Classifier * normalBayesWrapper = new CvClassifyWrapper(normalBayes);	
	Tester * tester = new Tester(normalBayesWrapper, featureSet);
	tester->test();
}

