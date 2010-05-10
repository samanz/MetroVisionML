/**
 * File: DBDataset.h
 * Author: Sam
 * 
 * Created on April 26, 2010, 2:56 PM
*/
#include <mysql++.h>
#include <iostream>
#include "Dataset.h"

#ifndef _DBDATASET_H
#define _DBDATASET_H

/**
 * DB Dataset class
 * This class implements the Dataset interface
 * The underlying database used here is the mysql database, and it is dependent on mysql++ libraries
*/
class DBDataset: public Dataset {
	private:
		int image_index;
		string dataset;
		int dataset_id;
		mysqlpp::Connection conn;
		mysqlpp::StoreQueryResult res;
		int num_images;
		string current_classification;
		YUVRange _range;

	public:
		/*
		 * Gets next image, returns in openCV format of IplImage
		*/
		IplImage * getNextImage();
		/*
		 * Returns the classification string of the element just returned from getNextImage()
		*/
		string getClassification();
		/**
		 * Returns the YUVRange for the object in the image
		*/
		YUVRange getYUVRange();
		/*
		 * Returns the size of the dataset
		*/
		int size();
		/*
		 * Constructor recieves the database, server, user, password, port of the mysql server, and the dataset
		 * Name used
		*/ 
		DBDataset(char * db, char * server, char * user, char * password, unsigned int port, string dataset_name);
		/*
		 * Destructor
		*/
		~DBDataset();
};
#endif /* _DBDATASET_H */
