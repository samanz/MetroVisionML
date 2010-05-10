#include "highgui.h"
#include "cv.h"
#include <iostream>
#include "DBDataset.h"

int main(int argc, char** argv) {
	if(argc != 7) {
		cout << "Usage: DBDatasetTest [database name] [server address] [database username] [database password] [database port] [dataset name]" << endl;
		return 0;
	}
	DBDataset * db_handle = new DBDataset(argv[1], argv[2], argv[3], argv[4], atoi(argv[5]), argv[6]);
	IplImage * img = db_handle->getNextImage();
	cvNamedWindow("DatabaseTest", CV_WINDOW_AUTOSIZE );
	cvShowImage("DatabaseTest", img );
	cvWaitKey(0);
	cvReleaseImage( &img );
	cvDestroyWindow( "DatabaseTest" );
}
