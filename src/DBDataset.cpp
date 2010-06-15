#include "DBDataset.h"
#include <cv.h>
#include <iostream>
#include <fstream>

DBDataset::DBDataset(char * db, char * server, char * user, char * password, unsigned int port, string dataset_name) {
	dataset = dataset_name;
	if(conn.connect(db, server, user, password)) {
		cout << "Connection Successful" << endl;
	} else {
		cerr << "DB connection failed: " << conn.error() << endl;
	}
	image_index=0;
	// Here just to make sure max_packet_length is high enough
	mysqlpp::Query query2 = conn.query();
	//query2.execute("set global max_allowed_packet=1000000000");

	// Temporary query to both get the dataset id and the size of the dataset
	// Probably want to replace this with specific query using COUNT(item.id) and a single row select
	mysqlpp::Query datasetQuery = conn.query();
	datasetQuery << "select dataset.id as data_id, item.id as item_id, type, width, height, classification.name from dataset, item, format, classification where dataset.name = " << mysqlpp::quote << dataset << "and dataset.id = item.dataset_id and format.id = item.format_id and item.classification_id = classification.id ORDER BY RAND()";
	if (res = datasetQuery.store()) {
	    num_images = res.num_rows();
	    dataset_id = res[0][0];
        }
        else {
            cerr << "Failed to get item list: " << datasetQuery.error() << endl;
        }
	storeYUVRanges();
}

int DBDataset::size() { return num_images; }

IplImage * DBDataset::getNextImage() {
	// If there are no more images, return a NULL pointer! (OMG Its FULL of images!)
	if(image_index >= num_images) return 0;	
	int image_id = res[image_index][1];
	mysqlpp::Query imageQuery = conn.query();
	imageQuery << "select image, classification.name, width, height, type, y_low, y_high, u_low, u_high, v_low, v_high from item, format, classification where format.id = item.format_id and item.classification_id = classification.id and item.id = " << mysqlpp::quote << image_id;
	mysqlpp::StoreQueryResult imageResult;
	mysqlpp::sql_blob data; // the actual image data from a longblob! (Just a mysqlpp::string ^_^ )
	if(imageResult = imageQuery.store()) {
		mysqlpp::sql_blob data = imageResult[0][0];
		cout << data.size() << endl;
		char * image_buffer = (char *)data.data();
		int image_width = imageResult[0][2];
		int image_height = imageResult[0][3];
		imageResult[0][1].to_string(current_classification);

		/* Store the YUV Range */
		_range.y_low = imageResult[0][5];
		_range.y_high = imageResult[0][6];
		_range.u_low = imageResult[0][7];
		_range.u_high = imageResult[0][8];
		_range.v_low = imageResult[0][9];
		_range.v_high = imageResult[0][10];
		_range.object = current_classification;
		/* End storing of YUV ranges */
		
		cout << image_width << " " << image_height << endl;
		cout << "Should be : " << image_width * image_height * 3 << endl;
		cout << "Index of: " << image_id << endl;
		cout << "Classification of: " << current_classification << endl;
		IplImage * frame = cvCreateImage(cvSize(image_width, image_height), 8, 3);
		// Copy data from the image buffer into frame & convert RGB to BGR
		for (int i = 0; i < image_width; i++){
			for (int j = 0; j < image_height; j++){
				frame->imageData[image_width * j*3 + i*3 + 0] =
			     	  (char)image_buffer[image_width * j*3 + i*3 + 2];
				frame->imageData[image_width * j*3 + i*3 + 1] =
			       	  (char)image_buffer[image_width * j*3 + i*3 + 1];
				frame->imageData[image_width * j*3 + i*3 + 2] =
			      	  (char)image_buffer[image_width * j*3 + i*3 + 0];
			}
		}
		image_index++;
		return frame;
	}
	//Else return a null pointer!
	cout << "Something bad happend :(" << endl;
	return 0;
}

YUVRange DBDataset::getYUVRange() {
	return _range;
} 

YUVRanges DBDataset::getAllYUVRanges() {
	return _ranges;
}

void DBDataset::storeYUVRanges() {
	mysqlpp::Query yuvQuery = conn.query();
	yuvQuery << "select classification.name, y_low, y_high, u_low, u_high, v_low, v_high from classification, dataset, item where dataset.name = " << mysqlpp::quote << dataset << " and dataset.id = item.dataset_id  and item.classification_id = classification.id";
	mysqlpp::StoreQueryResult imageResult;
	if(imageResult = yuvQuery.store()) {
		for(int i=0; i<res.num_rows(); i++) {
			YUVRange crange;
			/* Store the YUV Range */
			crange.y_low = imageResult[i][1];
			crange.y_high = imageResult[i][2];
			crange.u_low = imageResult[i][3];
			crange.u_high = imageResult[i][4];
			crange.v_low = imageResult[i][5];
			crange.v_high = imageResult[i][6];
			string cc;
			imageResult[i][0].to_string(cc);
			crange.object = cc;
			/* End storing of YUV ranges */
		
			addYUVRange(crange);
		}
	}	
}

void DBDataset::addYUVRange(YUVRange currentRange) {
	// Don't want to add useless ranges
	if(currentRange.y_low != 0 || currentRange.y_high != 0) {
		
		//First check if the objects range is already in there:
		bool alreadythere = false;
		for(int i=0;i<_ranges.ranges.size();i++) {
			if(_ranges.ranges[i].object == currentRange.object)
				alreadythere = true;
		}
	
		// If its not there yet, put it there!
		if(!alreadythere)
			_ranges.ranges.push_back(currentRange);
	} 
}

string DBDataset::getClassification() {
	// Get image classification!
	if(image_index>num_images) return 0; // Well we get the last index's classification 
	if(image_index==0) return 0;
	return current_classification;
}

DBDataset::~DBDataset() {
}

