/*
 ============================================================================
 Name        : AlgorithmsEngine.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-10-11
 Version     : 2.0
 Copyright   : Your copyright notice
 Description : Declaration of class executing user algorithms runs requests.
 ============================================================================
 */

#ifndef _ALGORITHMS_ENGINE_H_
#define _ALGORITHMS_ENGINE_H_

#include "Algorithm.h"
#include "Dataset.h"
#include "Properties.h"
#include "Report.h"

#include <fstream>
#include <string>
#include <vector>

using namespace std;

/**
 * Executes algorithms as defined in parameters files.
 */
class AlgorithmsEngine{
	
public: 

	~AlgorithmsEngine();

	static AlgorithmsEngine getInstance(){
	
		static AlgorithmsEngine instance;

		return instance;
	}

	/**
	 * Runs the engine.
	 */
	void run();

private:

	static const string LOGS_DIRECTORY_PATH;
	static const string PARAMETERS_DIRECTORY_PATH;
	static const string ALGORITHM_ENGINE_PARAMETERS_DIRECTORY_PATH;

	static const string ALFA_PARAMETER_NAME;
	static const string TEST_REPEATS_PARAMETER_NAME;

	/**
	 * Report output file.
	 */
	ofstream* reportFile;

	/**
	 * Parameters file names.
	 */
	vector<string> parametersFilesNames;

	/**
	 * Dataset.
	 */
	Dataset* dataset;

	/**
	 * Algorithm.
	 */
	Algorithm *algorithm;

	/**
	 * Properties;
	 */
	Properties* properties;

	/**
	 * Time report.
	 */
	TimeReport timeReport;

	/**
	 * Alfa value to calculate cosinus distance.
	 */
	double alfa;

	/**
	 * Number of repeats for each algorithm run.
	 */
	unsigned int testRepeats;

	vector<Report> ultimateReport;

	AlgorithmsEngine();

	/**
	 * Read algorithms engine properties
	 */
	void readAlgorithmsEngineProperties();

	/**
	 * Read parameters file names from
	 */
	void readParametersFilesNames();

	/**
	 * Reads properties from parameters file name.
	 *
	 * @parametersFileName    Name of parameters file from which properties
	 *                        is read.
	 */
	void readProperties(string parametersFileName);

	/**
	 * Reads data from dataset file specified in properties.
	 * Rewrties data to aprioprate structure.
	 * Normalizes data if necessary.
	 * Calculates reference points.
	 *
	 * Measures:
	 *	-dataset reading time,
	 *	-normalization time,
	 *	-calculatin regerence points time.
	 */
	void readData();

	/**
	 * Runs algorithm.
	 *
	 * Measures algorithm execution time.
	 */
	void runAlgorithm();

	/**
	 * Gets algorithm.
	 */
	Algorithm* getAlgorithm();

	/**
	 * Prints report into a *.txt file in logs directory.
	 */
	void printReport();

	/**
	 * Prints ultimate report into a *.csv file in  logs directory.
	 */
	void printUltimateReport();

	/**
	 * Clears data.
	 */
	void clear();
};

#endif /* _ALGORITHMS_ENGINE_H_ */