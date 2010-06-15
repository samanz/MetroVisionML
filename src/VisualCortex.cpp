#include "VisualCortex.h"

IplImage * segmentImage(IplImage * image, YUVRange range) {
	
	IplImage * smaller = cvCreateImage( cvSize(208, 160), 8, 3);
	cvResize(image, smaller);
	
	IplImage * YUV_image = cvCreateImage( cvGetSize(smaller), 8, 3);
	cvCvtColor(smaller, YUV_image, CV_BGR2YCrCb);
	IplImage * y = cvCreateImage( cvGetSize(YUV_image), 8, 1);
	IplImage * u = cvCreateImage( cvGetSize(YUV_image), 8, 1);
	IplImage * v = cvCreateImage( cvGetSize(YUV_image), 8, 1);
	
	cvSplit( YUV_image, v, u, y, NULL);	

	IplImage * yUpThreshold = cvCreateImage( cvGetSize(y), 8, 1);
	IplImage * yDownThreshold = cvCreateImage( cvGetSize(y), 8, 1);
	cvThreshold(y, yUpThreshold, range.y_low, 255, CV_THRESH_BINARY);
	cvThreshold(y, yDownThreshold, range.y_high, 255, CV_THRESH_BINARY_INV);
	IplImage * yAnd = cvCreateImage( cvGetSize(y), 8, 1);
	cvAnd(yUpThreshold, yDownThreshold, yAnd);

	IplImage * uUpThreshold = cvCreateImage( cvGetSize(u), 8, 1);
	IplImage * uDownThreshold = cvCreateImage( cvGetSize(u), 8, 1);
	cvThreshold(u, uUpThreshold, range.u_low, 255, CV_THRESH_BINARY);
	cvThreshold(u, uDownThreshold, range.u_high, 255, CV_THRESH_BINARY_INV);
	IplImage * uAnd = cvCreateImage( cvGetSize(u), 8, 1);
	cvAnd(uUpThreshold, uDownThreshold, uAnd);

	IplImage * vUpThreshold = cvCreateImage( cvGetSize(v), 8, 1);
	IplImage * vDownThreshold = cvCreateImage( cvGetSize(v), 8, 1);
	cvThreshold(v, vUpThreshold, range.v_low, 255, CV_THRESH_BINARY);
	cvThreshold(v, vDownThreshold, range.v_high, 255, CV_THRESH_BINARY_INV);
	IplImage * vAnd = cvCreateImage( cvGetSize(v), 8, 1);
	cvAnd(vUpThreshold, vDownThreshold, vAnd);

	IplImage * total = cvCreateImage( cvGetSize(vAnd), 8, 1);
	IplImage * final = cvCreateImage( cvGetSize(image), 8, 1);

	cvAnd(yAnd, uAnd, total);
	cvAnd(total, vAnd, total);
	
	cvResize(total, final);

	// Major Major cleanup needed here!
	cvReleaseImage(&smaller);
	cvReleaseImage(&YUV_image);
	cvReleaseImage(&y);
	cvReleaseImage(&u);
	cvReleaseImage(&v);
	
	cvReleaseImage(&yUpThreshold);
	cvReleaseImage(&yDownThreshold);
	cvReleaseImage(&yAnd);
	
	cvReleaseImage(&uUpThreshold);
	cvReleaseImage(&uDownThreshold);
	cvReleaseImage(&uAnd);

	cvReleaseImage(&vUpThreshold);
	cvReleaseImage(&vDownThreshold);
	cvReleaseImage(&vAnd);

	cvReleaseImage(&total);	

	return final;
}



void operator >> (const YAML::Node& node, YUVRange& r)
{
    node["object"] >> r.object;
    node["y_low"] >> r.y_low;
    node["y_high"] >> r.y_high;
    node["u_low"] >> r.u_low;
    node["u_high"] >> r.u_high;
    node["v_low"] >> r.v_low;
    node["v_high"] >> r.v_high;
    node["id"] >> r.id;
}

void operator >> (const YAML::Node & ranges, YUVRanges & rs) {
	for(int i=0; i<ranges.size(); i++) {
		YUVRange r;
		ranges[i] >> r;
		rs.ranges.push_back(r);
	}
}

VisualCortex::VisualCortex(Classifier * classifier, FeaturesExtractor * extractor, const char * theModel) : _classifier(classifier), _extractor(extractor) {
	/* 
	 We are just calling some functions here
	 Nah, just kidding... Actual Documentation:
	 So we open the file up and we parse it using YAML, k?
	*/
	ifstream fin(theModel);
	YAML::Parser parser(fin);
	YAML::Node doc;
	parser.GetNextDocument(doc);

	doc["YUVRanges"] >> _ranges;

	string filename;
	doc["model_filename"] >> filename;
	// So we need to load the classifier model information	
	_classifier->loadModel(filename.c_str());

}

string VisualCortex::classify(IplImage * image) {
	CvMat * featureVectors = cvCreateMat(1, _extractor->getFeatureVectorSize(), CV_32FC1);
	vector<float> features;
	for(int i=0; i<_ranges.ranges.size(); i++) {
		IplImage * segmented = segmentImage(image, _ranges.ranges.at(i));
		vector<float> tempFeatures = _extractor->extractFeatures(image, segmented);
		for(int j=0; j < tempFeatures.size(); j++) {
			*( (float*) CV_MAT_ELEM_PTR( *featureVectors, 0, j ) ) = tempFeatures.at(j);	
		}
		int klass = _classifier->classify(featureVectors);
		if(klass == _ranges.ranges.at(i).id) 
			return _ranges.ranges.at(i).object;
		cvReleaseImage(&segmented);	
	}
	// It should not get here
	return "Nothing";
}
