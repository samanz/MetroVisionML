#include "NaiveBayes.h"
#include "FeaturesExtractor.h"
#include "FeatureSet.h"
#include "Tester.h"
#include "cv.h"
#include <iostream>

int main(int argc, char ** argv) {
	if(argc != 3) {
		cout << "Usage: FileTest [features filename] [classifications filename]" << endl;
		return 0;
	}
	FeatureSet * featureSet = new FeatureSet(argv[1], argv[2]);
	NaiveBayes * naiveBayes = new NaiveBayes();
	Tester * tester = new Tester(naiveBayes, featureSet);
	tester->test();
}
