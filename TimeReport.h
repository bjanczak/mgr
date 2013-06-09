/*
 ============================================================================
 Name        : TimeReport.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-20
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class holding time measure results.
 ============================================================================
 */
#ifndef _TIME_REPORT_H_
#define _TIME_REPORT_H_

#include <fstream>
#include <vector>

using namespace std;

/**
 * Class holding time measure resuts.
 */
class TimeReport{

public:

	/*
	 * Algorithm execution time.
	 */
	double algorithmExecutionTime;
	
	/*
	 * Dataset normalization execution time.
	 */
	double normalizingDatasetExecutionTime;

	/*
	 * Dataset clustering execution time.
	 */
	double clusteringExecutionTime;

	/*
	 * Dataset sorting execution time.
	 */
	double sortingPointsExecutionTime;

	/*
	 * Calculating distance from reference points to each dataset point 
	 * execution time.
	 */
	double distanceCalculationExecutionTime;

	/*
	 * Positioning dataset points execution time.
	 */
	double positioningExecutionTime;

	/*
	 * Index building execution time;
	 */
	double indexBuildingExecutionTime;

	/*
	 * Datafile reading time.
	 */
	double datafileReadingTime;

	/*
	 * Calculating reference points time.
	 */
	double calculatingReferencePointsTime;

	vector<unsigned long> realDistanceCalculationsCounters;

	vector<unsigned long> verificationRealDistanceCalculationsCounters;
	
	vector<unsigned long> vpTreeSearchRealDistanceCalculations;

	unsigned long makeVpTreeRealDistanceCalculations;

	unsigned long makeVpTreeSelectVpRealDistanceCalculations;

	TimeReport();

	~TimeReport();

	TimeReport( const TimeReport& object);

	/**
	 * Clears data.
	 */
	void clear();

	/*
	 * Prints TimeReport report into os output stream.
	 *
	 * @os Output stream to which report is written.
	 */
	void print(ofstream &os);
};
#endif /* _TIME_REPORT_H_ */