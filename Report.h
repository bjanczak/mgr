/*
 ============================================================================
 Name        : Report.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-10-14
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class holding:
				* properties describing algorithm to run,
				* TimeReport execution times.
 ============================================================================
 */
#ifndef _REPORT_H_
#define _REPORT_H_

#include "Points.h"
#include "Properties.h"
#include "TimeReport.h"

#include<iostream>
#include <string>
#include <vector>
#include <utility>


using namespace std;

class Report{
public:

	~Report();
	
	Report();

	Report(const Report& object);

	Report(const Properties& properties, const TimeReport& timeReport, const string reportFileName);

	/**
	 * Prints report as text to given output stream.
	 *
	 *                                   - @os Output stream.
	 */
	void print(ofstream& os);

	/**
	 * Prints report Header as text to given output stream.
	 *
	 *                                   - @os Output stream.
	 */
	static void printHeader(ofstream& os);
	
	/**
	 * Column separator.
	 */
	static const string columnSeparator;

	/**
	 * Report file name.
	 */
	string reportFileName;

	/**
	 * Properties file name.
	 */
	string propertiesFileName;

	/**
	 * Name of algorithm type.
	 * It may take following values:
	 * *grouping - grouping algorithms,
	 * *classification - classification algorithms,
	 */
	string algorithmType;

	/**
	 * Name of group of algorithms.
	 * Indicates what type of point structure use.
	 * It may take following values:
	 * *dbscan
	 * *k_neighborhood
	 * *other
	 */
	string algorithmGroup;

	/**
	 * Numeric ID of algorithm group.
	 */
	unsigned long algorithmGroupId;

	/**
	 * Name of algorithm variant.
	 * It may take following values:
	 * *dbscan                           - basic DBSCAN algorithm,
	 * *ti_dbscan                        - TI_DBSCAN algorithm,
	 * *ti_dbscan_ref                    - TI_DBSCAN_REF algorithm
	 *                                     (multiple reference points),
	 * *ti_dbscan_ref_projection         - TI_DBSCAN_REF with dimension
     *                                     projection.
	 * *k_neighborhood                   - basic K-NEIGHBORHOOD algorithm,
	 * *ti_k_neighborhood                - TI-K-NEIGHBORHOOD algorithm
	                                       (using triangle inequality),
     * *ti_k_neighborhood_ref            - TI-K-NEIGHBORHOOD-REF algorithm
	                                       (multiple reference points),
	 * *ti_k_neighborhood_ref_projection - TI-K-NEIGHBORHOOD-REF with 
	                                       dimension projection,
     * *vp_tree                          - VP-TREE algorithm.,
     * *vps_tree                         - VPS-TREE algorithm.,
	 * *vp_constant - variant does use vantage point value readed from
	 * parameters file (used by VPS_TREE algorithms  group),
	 * *vp_calculated - variant does use vantage point value calculated from
	 * dataset file (used by VPS_TREE algorithms  group),
	 */
	string algorithmName;

	/**
	 * Numeric ID of algorithm.
	 */
	unsigned long algorithmNameId;

	/**
	 * DBSCAN algorithms group EPS parameter.
	 */
	double eps;

	/**
	 * Minimal Eps calculated from K-NEIGHBORHOOD algorithms results.
	 */
	double minEps;

	/**
	 * Maximal Eps calculated from K-NEIGHBORHOOD algorithms results.
	 */
	double maxEps;

	/**
	 * Average Eps calculated from K-NEIGHBORHOOD algorithms results.
	 */
	double avgEps;

	/**
	 * DBSCAN algorithms group MINPTS parameter. 
	 */
	unsigned long minPts;

	/**
	 * K_NEIGHBORHOOD algorithms group K parameter - number of neighbours. 
	 */
	unsigned long k;

	/**
	 * Flag indicating whether algorithm should use cosine similarity.
	 */
	bool useCosineSimilarity;

	/**
	 * Flag indicating whether dataset file format is dense or sparse.
	 */
	bool isDatasetFileFormatDense;

	/**
	 * Dataset file path.
	 */
	string datasetFilePath;

	/**
	 * Dataset dimension.
	 */
	unsigned long datasetDimension;

	/**
	 * Dimension value below which it will be replaced by 0 value.
	 */
	unsigned long datasetDimensionValueTreshold;

	/**
	 * Number of dataset elements.
	 */
	unsigned long datasetElementsNumber;

	/**
	 * Flag indicating whether dataset internal format
	 * is dense or sprase.
	 */
	bool isDatasetInternalFormatDense;

	/**
	 * Reference points collection string definition.
	 */
	string referencePointsString;

	/**
	 * Projection dimensions collection string definition.
	 */
	string projectionDimensionsString;

	/*
	 * Projection sorting criteria definition.
	 */
	string projectionSortingCriteriaString;

	/**
	 * Reference points collection.
	 */
	vector<Point> referencePoints;

	/**
	 * Flag indicating whether reference point definition in properties file
	 * has dense or sparce format.
	 */
	bool isReferencePointFormatDense;

	/**
	 * Number of dimensions on which parameters will be projected.
	 */
	vector<unsigned long> projectionDimensions;

	/**
	 * Defines sequence of pruning priorities.
	 */
	vector<pair<char, unsigned long>> projectionSourceSequence;

	/**
	 * Describes which element of dataset goes into subset collection.
	 */
	unsigned long classificationSubsetFactor;

	/**
	 * Flag indicating whether bianry or lineary placement should be used
	 * during classification.
	 */
	bool useBinaryPlacement;

	/**
	 * vantage point algorithms P sample index.
	 */
	unsigned long pSampleIndex;

	/**
	 * vantage point algorithms S sample index.
	 */
	unsigned long sSampleIndex;

	/**
	 *  Vantage point defined in properties file string definition.
	 */
	string constantVantagePointString;

	/**
	 * Vantage point tree search method.
	 * It may take following values:
	 * *range
	 * *k_neighborhood
	 */
	string searchMethod;

	/**
	 * Flag indicating whether dataset should be accessed by index.
	 */
	bool isUseDatasetIndexAcess;

	/**
	 * Flag indicating whether to use boundaries while VP_TREE searching.
	 */
	bool isUseBoundaries;

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
};

#endif /* _REPORT_H_ */