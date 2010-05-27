### DEFINITIONS ###

# Toolchain definitions
CC = g++
AR = ar
CP = cp

# DEFINITIONS
INCLUDE_DIR = include/
SRC_DIR = src/
MLTESTER_CFLAGS = -fPIC -g3 -O0 -I./include `pkg-config --cflags opencv`
MLTESTER_LIBS 	= `pkg-config --libs opencv`
MYSQL_CFLAGS 	= -I/usr/include/mysql -I/usr/include/mysql++
MYSQL_LIBS	= -L/usr/local/lib -lmysqlpp -lmysqlclient

all:
	$(MAKE) DBDataset
	$(MAKE) AngleTester
	$(MAKE) CalSaver
	$(MAKE) FileTester


DBDataset: DBDatasetTest.o DBDataset.o
	$(CC) -g -Wall $(MLTESTER_CFLAGS) $(MYSQL_CFLAGS) $(MLTESTER_LIBS) $(MYSQL_LIBS) DBDatasetTest.o DBDataset.o -o bin/DBDatasetTest

DBDataset.o: $(SRC_DIR)DBDataset.cpp $(INCLUDE_DIR)DBDataset.h $(INCLUDE_DIR)Dataset.h
	$(CC) -c $(SRC_DIR)DBDataset.cpp $(MLTESTER_CFLAGS) $(MYSQL_CFLAGS) $(MLTESTER_LIBS) $(MYSQL_LIBS) 

CalSaver.o: $(SRC_DIR)CalSaver.cpp $(INCLUDE_DIR)DBDataset.h
	$(CC) -c $(SRC_DIR)CalSaver.cpp $(MLTESTER_CFLAGS) $(MYSQL_CFLAGS) $(MLTESTER_LIBS) $(MYSQL_LIBS)

CalSaver: CalSaver.o DBDataset.o
	$(CC) -g -Wall $(MLTESTER_CFLAGS) $(MYSQL_CFLAGS) $(MLTESTER_LIBS) $(MYSQL_LIBS) DBDataset.o CalSaver.o -o bin/CalSaver

DBDatasetTest.o: $(SRC_DIR)DBDataset.cpp $(INCLUDE_DIR)DBDataset.h $(INCLUDE_DIR)Dataset.h
	$(CC) -c $(SRC_DIR)DBDatasetTest.cpp $(MLTESTER_CFLAGS) $(MYSQL_CFLAGS) $(MLTESTER_LIBS) $(MYSQL_LIBS)

AngleHistogramExtractor.o: $(SRC_DIR)AngleHistogramExtractor.cpp $(INCLUDE_DIR)AngleHistogramExtractor.h $(INCLUDE_DIR)FeatureExtractor.h
	$(CC) -c $(SRC_DIR)AngleHistogramExtractor.cpp $(MLTESTER_CFLAGS) $(MLTESTER_LIBS)

FeatureExtractorTest.o: $(SRC_DIR)FeatureExtractorTest.cpp $(INCLUDE_DIR)FeatureExtractorTest.h $(INCLUDE_DIR)FeatureExtractor.h 
	$(CC) -c $(SRC_DIR)FeatureExtractorTest.cpp $(MLTESTER_CFLAGS) $(MLTESTER_LIBS)

FeaturesExtractor.o: $(INCLUDE_DIR)FeatureExtractor.h FeatureExtractorTest.o $(INCLUDE_DIR)FeaturesExtractor.h
	$(CC) -c $(SRC_DIR)FeaturesExtractor.cpp $(MLTESTER_CFLAGS) $(MLTESTER_LIBS)

FeatureSet.o: $(SRC_DIR)FeatureSet.cpp $(INCLUDE_DIR)FeaturesExtractor.h $(INCLUDE_DIR)FeatureSet.h FeaturesExtractor.o
	$(CC) -c $(SRC_DIR)FeatureSet.cpp $(MLTESTER_CFLAGS) $(MLTESTER_LIBS) 

CvClassifyWrapper.o: $(INCLUDE_DIR)CvClassifyWrapper.h $(INCLUDE_DIR)Classifier.h FeatureSet.o
	$(CC) -c $(SRC_DIR)CvClassifyWrapper.cpp $(MLTESTER_CFLAGS) $(MLTESTER_LIBS)

NaiveBayes.o: $(INCLUDE_DIR)NaiveBayes.h $(INCLUDE_DIR)Classifier.h FeatureSet.o
	$(CC) -c $(SRC_DIR)NaiveBayes.cpp $(MLTESTER_CFLAGS) $(MLTESTER_LIBS)

Tester.o: $(INCLUDE_DIR)Tester.h FeatureSet.o 
	$(CC) -c $(SRC_DIR)Tester.cpp $(MLTESTER_CFLAGS) $(MLTESTER_LIBS)

TesterTester.o: Tester.o
	$(CC) -c $(SRC_DIR)TesterTester.cpp $(MLTESTER_CFLAGS) $(MLTESTER_LIBS)

AngleTester.o: Tester.o AngleHistogramExtractor.o DBDataset.o CvClassifyWrapper.o
	$(CC) -c $(SRC_DIR)AngleTester.cpp $(MLTESTER_CFLAGS) $(MYSQL_CFLAGS) $(MLTESTER_LIBS) $(MYSQL_LIBS)

TesterTester: TesterTester.o
	$(CC) -g -Wall $(MLTESTER_CFLAGS) $(MLTESTER_LIBS) DBDataset.o FeatureExtractorTest.o FeaturesExtractor.o FeatureSet.o CvClassifyWrapper.o Tester.o TesterTester.o -o bin/TesterTester

AngleTester: AngleTester.o 
	$(CC) -g -Wall $(MLTESTER_CFLAGS) $(MYSQL_CFLAGS) $(MLTESTER_LIBS) $(MYSQL_LIBS) DBDataset.o AngleHistogramExtractor.o FeaturesExtractor.o FeatureSet.o CvClassifyWrapper.o Tester.o AngleTester.o -o bin/AngleTester
clean:
	rm *.o

FileTester.o: Tester.o NaiveBayes.o
	$(CC) -c $(SRC_DIR)FileTester.cpp $(MLTESTER_CFLAGS) $(MLTESTER_LIBS)

FileTester: FileTester.o
	$(CC) -g -Wall $(MLTESTER_CFLAGS) $(MLTESTER_LIBS) FeaturesExtractor.o FeatureSet.o NaiveBayes.o Tester.o FileTester.o -o bin/FileTester
