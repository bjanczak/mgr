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
	this->ultimateReportFile = new ofstream();
	this->cleanedUltimateReportFile = new ofstream();
	this->testRepeats = 0;
	this->alfa = 0;
	this->ultimateReport;
	this->cleanedUltimateReport;
}

AlgorithmsEngine::~AlgorithmsEngine(){

	this->parametersFilesNames.clear();
	this->ultimateReport.clear();
	this->cleanedUltimateReport.clear();
}

void AlgorithmsEngine::clear(){

	properties->clear();
	dataset->clear();
	timeReport.clear();

	if(reportFile->is_open()){
	
		reportFile->close();
	}

	if(ultimateReportFile->is_open()){
	
		ultimateReportFile->close();
	}

	if(cleanedUltimateReportFile->is_open()){
	
		cleanedUltimateReportFile->close();
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

	dataset->readProjectionDimensions(*properties);

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
				
				//avg = avg + tempValue;

				if(tempValue > max){
				
					max = tempValue;
				}

				if(tempValue < min && tempValue!=0){
				
					min = tempValue;
				}

				//counter++;
			}
		}
	}

	//avg = avg / counter;	
	cout<<"min = "<<min;
	//cout<<"avg = "<<avg;
	cout<<"max = "<<max;*/

	//karypis statystyki
	/*double temp_value;
	double avg = 0;
	double min = DBL_MAX;
	double max = DBL_MIN;
	vector<Point> datasetPoint = dataset->datasetPoint;
	vector<Point>::iterator it;
	vector<SparsePoint>::iterator it_2;
	vector<SparsePoint>::iterator end_2;
	vector<Point>::iterator end = datasetPoint.end();
	double counter = 0;
	ofstream* tempFile = new ofstream();
	tempFile->open("logs/karypis_stats.csv", ios_base::app);
	
	*tempFile<<"id;dimensions;"<<endl;
	
	for( it = datasetPoint.begin(); it != end; it++){
		*tempFile<<it->id<<";"<<it->sparseFormatPoint.size()<<";"<<endl;
		it_2 = it->sparseFormatPoint.begin();
		end_2 = it->sparseFormatPoint.end();

		while(it_2 != end_2) {
			temp_value = it_2->value;
			avg = avg + temp_value;
			
			if (temp_value < min) {
				min = temp_value;
			}

			if (temp_value > max) {
				max = temp_value;
			}

			counter++;
			it_2++;
		}
	}

	avg = avg / counter;

	*tempFile<<"min;"<<min<<";"<<endl;
	*tempFile<<"avg;"<<avg<<";"<<endl;
	*tempFile<<"max;"<<max<<";"<<endl;

	tempFile->close();*/
	//

	/*
	 * Dataset normalization if necessary.
	 */
	if(properties->useCosineSimilarity){
	
		properties->eps = Utils::getCosEps(properties->eps);

		normalizationStart = clock();
		
		dataset->normalize(this->alfa);
		
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
		case 6:

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
		if(properties->algorithmGroup == Properties::OTHER){
		
			dataset->calculateClassificationResultEps(properties->minEps, properties->avgEps, properties->maxEps);
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
	this->timeReport.realDistanceCalculationsCounters = vector<unsigned long>(tempTimeReport.realDistanceCalculationsCounters);
	this->timeReport.verificationRealDistanceCalculationsCounters = vector<unsigned long>(tempTimeReport.verificationRealDistanceCalculationsCounters);
	this->timeReport.vpTreeSearchRealDistanceCalculations = vector<unsigned long>(tempTimeReport.vpTreeSearchRealDistanceCalculations);
	this->timeReport.makeVpTreeRealDistanceCalculations = tempTimeReport.makeVpTreeRealDistanceCalculations;
	this->timeReport.makeVpTreeSelectVpRealDistanceCalculations = tempTimeReport.makeVpTreeSelectVpRealDistanceCalculations;
}

void AlgorithmsEngine::run(){
	
	string parameterFilePath;
	string reportName;
	string ultimateReportName = Utils::ultimateReportNameGenerator();
	string cleanedUltimateReportName = Utils::ultimateCleanedReportNameGenerator();	
	unsigned long testRepeatCounter = 1;
	Report report;

	readAlgorithmsEngineProperties();

	/*
	 * Read files names from properties/ directory.
	 */
	readParametersFilesNames();

	ultimateReportFile->open(ultimateReportName, ios_base::app);	
	cleanedUltimateReportFile->open(cleanedUltimateReportName, ios_base::app);
	
	Report::printHeader(*ultimateReportFile);
	Report::printHeader(*cleanedUltimateReportFile);

	ultimateReportFile->close();
	cleanedUltimateReportFile->close();

	try {

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
			//print ultimate report

			/*
			 * Save report.
			 */
			ultimateReportFile->open(ultimateReportName, ios_base::app);	
			
			report = Report(*properties, timeReport, reportName);
			report.print(*ultimateReportFile);
			ultimateReport.push_back(report);

			/*
			 * Clear engine variabples before next algorithm run.
			 */
			clear();
		
			/*
			 * Erase first parameter file name from collection.
			 */
			if(testRepeatCounter == this->testRepeats) {		
				parametersFilesNames.erase(parametersFilesNames.begin());
				testRepeatCounter = 1;
				cleanedUltimateReportFile->open(cleanedUltimateReportName, ios_base::app);
				printCleanedUltimateReport();
				cleanedUltimateReportFile->flush();
				cleanedUltimateReportFile->close();		
				ultimateReport.clear();
			} else {
				testRepeatCounter++;
			}
			
			ultimateReportFile->flush();			
			ultimateReportFile->close();
			
			/*
			 * Lets sleep for a second, for good report file name generation.
			 */
			Sleep(5000);
		}

	} catch (const std::exception &e) {
		
		reportFile->close();
		ultimateReportFile->close();
		cleanedUltimateReportFile->close();		
		throw e;
	} 

	reportFile->close();
	ultimateReportFile->close();
	cleanedUltimateReportFile->close();		
}

void AlgorithmsEngine::printCleanedUltimateReport(){
	unsigned long size = ultimateReport.size();	
	unsigned int counter = 0;
	unsigned long index;
	vector<double> algorithmExecutionTime;
	vector<double> clusteringExecutionTime;
	vector<double> indexBuildingExecutionTime;
	vector<double> distanceCalculationExecutionTime;
	vector<double> sortingPointsExecutionTime;
	vector<double> positioningExecutionTime;
	vector<double> normalizingDatasetExecutionTime;
	vector<double> datafileReadingTime;
	vector<double> calculatingReferencePointsTime;
	vector<double> makeVpTreeRealDistanceCalculations;
	vector<double> makeVpTreeSelectVpRealDistanceCalculations;

	vector<unsigned long> realDistanceCalculationsCounter;
	vector<unsigned long> verificationRealDistanceCalculationsCounter;
	vector<unsigned long> vpTreeSearchRealDistanceCalculations;

	Report report;

	for(unsigned long i = 0; i <this->testRepeats; i++){
		index = i;
		algorithmExecutionTime.push_back(ultimateReport[index].algorithmExecutionTime);
		clusteringExecutionTime.push_back(ultimateReport[index].clusteringExecutionTime);
		indexBuildingExecutionTime.push_back(ultimateReport[index].indexBuildingExecutionTime);
		distanceCalculationExecutionTime.push_back(ultimateReport[index].distanceCalculationExecutionTime);
		sortingPointsExecutionTime.push_back(ultimateReport[index].sortingPointsExecutionTime);
		positioningExecutionTime.push_back(ultimateReport[index].positioningExecutionTime);
		normalizingDatasetExecutionTime.push_back(ultimateReport[index].normalizingDatasetExecutionTime);
		datafileReadingTime.push_back(ultimateReport[index].datafileReadingTime);
		calculatingReferencePointsTime.push_back(ultimateReport[index].calculatingReferencePointsTime);
		makeVpTreeRealDistanceCalculations.push_back(ultimateReport[index].makeVpTreeRealDistanceCalculations);
		makeVpTreeSelectVpRealDistanceCalculations.push_back(ultimateReport[index].makeVpTreeSelectVpRealDistanceCalculations);

		report = ultimateReport[index];	
	}

	report.algorithmExecutionTime = Utils::getCleanValue(algorithmExecutionTime);
	report.clusteringExecutionTime = Utils::getCleanValue(clusteringExecutionTime);
	report.indexBuildingExecutionTime = Utils::getCleanValue(indexBuildingExecutionTime);
	report.distanceCalculationExecutionTime = Utils::getCleanValue(distanceCalculationExecutionTime);
	report.sortingPointsExecutionTime = Utils::getCleanValue(sortingPointsExecutionTime);
	report.positioningExecutionTime = Utils::getCleanValue(positioningExecutionTime);
	report.normalizingDatasetExecutionTime = Utils::getCleanValue(normalizingDatasetExecutionTime);
	report.datafileReadingTime = Utils::getCleanValue(datafileReadingTime);
	report.calculatingReferencePointsTime = Utils::getCleanValue(calculatingReferencePointsTime);
	report.makeVpTreeRealDistanceCalculations = Utils::getCleanValue(makeVpTreeRealDistanceCalculations);
	report.makeVpTreeSelectVpRealDistanceCalculations = Utils::getCleanValue(makeVpTreeSelectVpRealDistanceCalculations);

	report.print(*cleanedUltimateReportFile);	
}