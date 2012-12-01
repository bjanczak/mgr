/*
 ============================================================================
 Name        : AlgorithmEngine.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-10-11
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of AlgorithmEngine.h.
 ============================================================================
 */

#include "AlgorithmsEngine.h"
#include "Dataset.h"
#include "Dbscan.h"
#include "DbscanPointsElimination.h"
#include "KNeighborhood.h"
#include "TiDbscan.h"
#include "TiDbscanRef.h"
#include "TiDbscanRefProjection.h"
#include "TiKNeighborhood.h"
#include "TiKNeighborhoodRef.h"
#include "TiKNeighborhoodRefProjection.h"
#include "Utils.h"
#include "VpsTree.h"
#include "VpTree.h"

#include <algorithm>
#include <windows.h>
#include <time.h>
#include <tchar.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>

const string AlgorithmsEngine::LOGS_DIRECTORY_PATH = "logs";
const string AlgorithmsEngine::PARAMETERS_DIRECTORY_PATH = "properties/";
const string AlgorithmsEngine::ALGORITHM_ENGINE_PARAMETERS_DIRECTORY_PATH = "algorithms_engine_properties/algorithms_engine_properties.txt";

const string AlgorithmsEngine::ALFA_PARAMETER_NAME = "alfa";
const string AlgorithmsEngine::TEST_REPEATS_PARAMETER_NAME = "test_repeats";

AlgorithmsEngine::AlgorithmsEngine(){

	this->dataset = &Dataset::getInstance();
	this->properties = &Properties::getInstance();
	this->reportFile = new ofstream();
	this->testRepeats = 0;
	this->alfa = 0;
}

AlgorithmsEngine::~AlgorithmsEngine(){

	parametersFilesNames.clear();
}

void AlgorithmsEngine::clear(){

	properties->clear();
	dataset->clear();
	timeReport.clear();

	if(reportFile->is_open()){
	
		reportFile->close();
	}
}

void AlgorithmsEngine::readAlgorithmsEngineProperties(){

	string bufor;
	string parameterName;
	string parameterValue;
	size_t found;

	ifstream inputFileStream (ALGORITHM_ENGINE_PARAMETERS_DIRECTORY_PATH.c_str());

	if (!inputFileStream){
		cerr<<"Error: properties file: "<<ALGORITHM_ENGINE_PARAMETERS_DIRECTORY_PATH<<" could not be opened"<<endl;
	}
	else		
		if(inputFileStream.is_open()){
	
			while(inputFileStream.good()){
		
				getline(inputFileStream,bufor);
				
				found = bufor.find('=');
			
				if(found!=string::npos){
					parameterName = bufor.substr(0,found);
					parameterValue = bufor.substr(found+1,bufor.size());

					if(parameterName == AlgorithmsEngine::ALFA_PARAMETER_NAME){
					
						this->alfa = atof(parameterValue.c_str());
					}
					else
						if(parameterName == AlgorithmsEngine::TEST_REPEATS_PARAMETER_NAME){
					
							this->testRepeats = atoi(parameterValue.c_str());
					}
				}
			}

			inputFileStream.close();
		}
}

void AlgorithmsEngine::readParametersFilesNames(){

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	string propertiesFilesQuery = PARAMETERS_DIRECTORY_PATH + "*.txt";

	hFind = FindFirstFile(propertiesFilesQuery.c_str(), &FindFileData);
	
	while(hFind != INVALID_HANDLE_VALUE){
		
		parametersFilesNames.push_back(PARAMETERS_DIRECTORY_PATH + FindFileData.cFileName);
		
		if(FindNextFile(hFind, &FindFileData) == 0){
			break;
		}
	}
	
	FindClose(hFind);
}

void AlgorithmsEngine::readProperties(string parametersFileName){

	properties->readProperties(parametersFileName);
}

void AlgorithmsEngine::readData(){
	
	clock_t readingStart = 0;
	clock_t readingFinish = 0;
	clock_t calculateReferencePointsStart = 0;
	clock_t calculateReferencePointsFinish = 0;
	clock_t normalizationStart = 0;
	clock_t normalizationFinish = 0;

	dataset->setProperties(*properties);

	readingStart = clock();

	dataset->readData(*properties);

	readingFinish = clock();

	// Calculations
	/*double tempValue;
	double avg = 0;
	double min = DBL_MAX;
	double max = DBL_MIN;
	vector<Point> datasetPoint = dataset->datasetPoint;
	vector<Point>::iterator it;
	vector<Point>::iterator it_2;
	vector<Point>::iterator end = datasetPoint.end();
	double counter = 0;

	for( it = datasetPoint.begin(); it != end; it++){
	
		for( it_2 = datasetPoint.begin(); it_2 != end; it_2++){
	
			if(it != it_2){
				tempValue = Point::minkowskiDistance(*it, *it_2, 2);
				
				avg = avg + tempValue;

				if(tempValue > max){
				
					max = tempValue;
				}

				if(tempValue < min && tempValue!=0){
				
					min = tempValue;
				}

				counter++;
			}
		}
	}

	avg = avg / counter;	
	cout<<"min = "<<min;
	cout<<"avg = "<<avg;
	cout<<"max = "<<max;*/

	//

	/*
	 * Dataset normalization if necessary.
	 */
	if(properties->useCosineSimilarity){
	
		properties->eps = Utils::getCosEps(properties->eps);

		normalizationStart = clock();
		
		dataset->normalize();
		
		normalizationFinish = clock();
	}

	/*
	 * Reference points calculation.
	 */
	if((properties->algorithmNameId != Properties::DBSCAN_ID) && (properties->algorithmNameId != Properties::DBSCAN_POINTS_ELIMINATION_ID) && (properties->algorithmNameId != Properties::VP_TREE_ID)){
	
		calculateReferencePointsStart = clock();

		properties->referencePoints = dataset->getReferencePoints(*properties);

		calculateReferencePointsFinish = clock();

		timeReport.calculatingReferencePointsTime = ((double)(calculateReferencePointsFinish - calculateReferencePointsStart))/CLOCKS_PER_SEC;
	}

	/*
	 * Fill proper dataset structure.
	 */
	switch(properties->algorithmGroupId){
	
		case 1:

			Dataset::fillDbscanPointVector(dataset->datasetDbscanPoint, dataset->datasetPoint);
			break;
		case 5:

			Dataset::fillKNeighborhoodPointVector(dataset->datasetKNeighborhoodPoint, dataset->datasetPoint);
			break;
	}

	if(properties->algorithmType == Properties::CLASSIFICATION){
			
		Point* nullPointer;
		vector<Point> sample = Dataset::generateSample(dataset->datasetPoint, *properties);
		vector<Point>::iterator sampleIt;
		vector<Point>::iterator sampleEnd = sample.end();

		for(sampleIt = sample.begin();sampleIt != sampleEnd; sampleIt++){
			
			dataset->classificationDataset.push_back(pair<Point, Point*>(*sampleIt, nullPointer));
		}

		sample.clear();
	}
	
	timeReport.datafileReadingTime = ((double)(readingFinish - readingStart))/CLOCKS_PER_SEC;

	/*
	 * Calculating normalization time.
	 */
	if(normalizationStart != normalizationFinish){

		timeReport.normalizingDatasetExecutionTime = ((double)(normalizationFinish - normalizationStart))/CLOCKS_PER_SEC;
	}
}

Algorithm* AlgorithmsEngine::getAlgorithm(){

	switch(properties->algorithmNameId){

		case 1:		
			return new Dbscan();
		
		case 2:		
			return new DbscanPointsElimination();
		
		case 3:
			return new TiDbscan();
		
		case 4:
			return new TiDbscanRef();
		
		case 5:
			return new TiDbscanRefProjection();
		
		case 6:
			return new KNeighborhood();
		
		case 7:
			return new TiKNeighborhood();

		case 8:		
			return new TiKNeighborhoodRef();
		
		case 9:
			return new TiKNeighborhoodRefProjection();
		
		case 10:
			return new VpTree();
		
		case 11:
			return new VpsTree();
		
		default:
			return new Algorithm();
	}
}

void AlgorithmsEngine::printReport(){

	if(reportFile->is_open()){
		
		if(properties->algorithmGroup == Properties::K_NEIGHBORHOOD){
		
			dataset->calculateKNeighborhoodEps(properties->minEps, properties->avgEps, properties->maxEps);
		}
		properties->print(*reportFile);
		timeReport.print(*reportFile);
		dataset->printClusteringSumUp(*reportFile);
	
		reportFile->close();
	}
}

void AlgorithmsEngine::runAlgorithm(){
	
	TimeReport tempTimeReport;

	algorithm = getAlgorithm();

	tempTimeReport = algorithm->run(*properties, *dataset);

	this->timeReport.algorithmExecutionTime = tempTimeReport.algorithmExecutionTime;
	this->timeReport.clusteringExecutionTime = tempTimeReport.clusteringExecutionTime;
	this->timeReport.distanceCalculationExecutionTime = tempTimeReport.distanceCalculationExecutionTime;
	this->timeReport.indexBuildingExecutionTime = tempTimeReport.indexBuildingExecutionTime;
	this->timeReport.positioningExecutionTime = tempTimeReport.positioningExecutionTime;
	this->timeReport.sortingPointsExecutionTime = tempTimeReport.sortingPointsExecutionTime;
}

void AlgorithmsEngine::run(){
	
	string parameterFilePath;
	string reportName;
	string ultimateReportName;
	unsigned long size;
	unsigned long testRepeatCounter = 1;

	readAlgorithmsEngineProperties();

	/*
	 * Read files names from properties/ directory.
	 */
	readParametersFilesNames();

	while(parametersFilesNames.size()>0){
		
		/*
		 * Generate report name.
		 */
		reportName = Utils::reportNameGenerator();

		/*
		 * Create and open report file.
		 */
		reportFile->open(reportName);

		/*
		 * Get next parameter file name.
		 */
		parameterFilePath = parametersFilesNames[0];
		
		/*
		 * Read parameter file to properties object.
		 */
		readProperties(parameterFilePath);
		
		/*
		 * Read dataset.
		 */
		readData();

		/*
		 * Run algorithm.
		 */
		runAlgorithm();

		/*
		 * Print run report.
		 */
		printReport();

		/*
		 * Save report.
		 */
		ultimateReport.push_back(Report(*properties, timeReport, reportName));

		/*
		 * Clear engine variabples before next algorithm run.
		 */
		clear();
		
		/*
		 * Erase first parameter file name from collection.
		 */
		if(testRepeatCounter == this->testRepeats){
		
			parametersFilesNames.erase(parametersFilesNames.begin());
			testRepeatCounter = 1;
		}
		else{

			testRepeatCounter++;
		}

		/*
		 * Lets sleep for a second, for good report file name generation.
		 */
		Sleep(10000);
	}

	/*
	 * Print ultimate report.
	 */
	ultimateReportName = Utils::ultimateReportNameGenerator();
	size = ultimateReport.size();
	reportFile->open(ultimateReportName);

	Report::printHeader(*reportFile);

	for(unsigned long i = 0; i <size; i++){
		
		ultimateReport[i].print(*reportFile);
	}

	reportFile->close();
}
