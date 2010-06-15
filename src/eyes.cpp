#include "VisualCortex.h"
#include "cxcore.h"
#include "cv.h"
#include "highgui.h"

int main( int argc, char** argv ) {
    if(argc != 2) {
	cout << "Usage: eyes [model to load] " << endl;
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
	
    cvNamedWindow( "Example2", CV_WINDOW_AUTOSIZE );
    CvCapture* capture = cvCreateCameraCapture(-1);
    IplImage* frame;
    int i =0;
    while(1) {
        frame = cvQueryFrame( capture );
        if( !frame ) break;
        cvShowImage( "Example2", frame );
	cout << cortex->classify(frame) << endl;
        char c = cvWaitKey(33);
        if( c == 27 ) break;
    }
    cvReleaseCapture( &capture );
    cvDestroyWindow( "Example2" );
}

