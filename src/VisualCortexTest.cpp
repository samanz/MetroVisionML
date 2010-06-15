#include "VisualCortex.h"
#include "cxcore.h"
#include "cv.h"
#include "highgui.h"

int main(int argc, char ** argv) {
	if(argc != 3) {
		cout << "Called like this: VisualCortexTest [model to load] [image to classify]" << endl;
		return 0;
	}
	FeatureExtractor * angleExtractor = new AngleHistogramExtractor();
	FeatureExtractor * edgeExtractor = new EdgeLengthHistogramExtractor();
	FeatureExtractor * colorExtractor = new AverageColorFeatureExtractor();
	FeaturesExtractor * extractor = new FeaturesExtractor();
	extractor->addFeatureExtractor(colorExtractor);
	extractor->addFeatureExtractor(angleExtractor);
	extractor->addFeatureExtractor(edgeExtractor);
	CvNormalBayesClassifier * normalBayes = new CvNormalBayesClassifier();
	Classifier * normalBayesWrapper = new CvClassifyWrapper(normalBayes);
	VisualCortex * cortex = new VisualCortex(normalBayesWrapper, extractor, argv[1]); 	
	IplImage * testImage = cvLoadImage(argv[2]);
	cout << cortex->classify(testImage) << endl;
}
