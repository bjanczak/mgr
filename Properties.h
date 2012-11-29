/*
 ============================================================================
 Name        : Properties.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-03
 Version     : 2.0
 Copyright   : Your copyright notice
 Description : Declarations of class providing application properties values.
 ============================================================================
 */

#ifndef _PROPERTIES_H_
#define _PROPERTIES_H_

#include <string>
#include <vector>
#include <utility>

#include "Points.h"

using namespace std;

/**
 * Provides access to application properties values.
 * Implements Singleton pattern.
 */
class Properties{

public:
	
	~Properties();

	static Properties& getInstance(){
		static Properties instance;
		return instance;
	}

	/**
	 * Clears all the data.
	 */
	void clear();

	/**
	 * 
	 *
	 * @propertiesFileName               - Name of properties file.
	 */
	void readProperties(string propertiesFileName);

	/**
	 * Prints properties as text to given output stream.
	 *
	 * @os Output stream.
	 */
	void print( ofstream& os);

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
     * *vp_tree                          - VP-TREE algorithm,
     * *vps_tree                         - VPS-TREE algorithm.
	 */
	string algorithmName;

	/**
	 * Numeric ID of algorithm.
	 */
	unsigned long algorithmNameId;

	/**
	 * EPS parameter.
	 */
	double eps;

	/**
	 * MINPTS parameter. 
	 */
	unsigned long minPts;

	/**
	 * K parameter - number of neighbours. 
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
	 * Flag indicating whether binary or lineary placement should be used
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
	 * Flag indicating whether dataset should be accessed by index.
	 */
	bool isUseDatasetIndexAcess;

	/**
	 * Public constants.
	 */
	static const string GROUPING;

	static const string CLASSIFICATION;

	static const string DBSCAN;

	static const string TI_DBSCAN;

	static const string TI_DBSCAN_REF;

	static const string TI_DBSCAN_REF_PROJECTION;

	static const string K_NEIGHBORHOOD;

	static const string TI_K_NEIGHBORHOOD;

	static const string TI_K_NEIGHBORHOOD_REF;

	static const string TI_K_NEIGHBORHOOD_REF_PROJECTION;

	static const string VP_TREE;

	static const string VPS_TREE;

	static const unsigned long DBSCAN_ID;

	static const unsigned long TI_DBSCAN_ID;

	static const unsigned long TI_DBSCAN_REF_ID;

	static const unsigned long TI_DBSCAN_REF_PROJECTION_ID;

	static const unsigned long K_NEIGHBORHOOD_ID;

	static const unsigned long TI_K_NEIGHBORHOOD_ID;

	static const unsigned long TI_K_NEIGHBORHOOD_REF_ID;

	static const unsigned long TI_K_NEIGHBORHOOD_REF_PROJECTION_ID;

	static const unsigned long VP_TREE_ID;

	static const unsigned long VPS_TREE_ID;

	static const unsigned long OTHER_ID;

	static const string VP_CONSTANT;

	static const string VP_CALCULATED;

	static const string TRUE;

	static const string FALSE;

	static const string OTHER;

	static const string RANGE;

private:

	/**
	 * Default constructor, that reads properties values from properties.txt
	 * file and stores them in class attributes.
	 */
	Properties();
	
	/**
	 * Generates aprioprate algorithm ID for given algorithm name.
	 *
	 * @algorithmName   
	 *
	 * @return          Algorithm ID.
	 */
	unsigned long getAlgorithmNameId(string algorithmName);

	/**
	 * Generates aprioprate algorithm group name for given algorithm name.
	 *
	 * @algorithmName   
	 *
	 * @return          Algorithm group name.
	 */
	string getAlgorithmGroupName(string algorithmName);

	/**
	 * Generates aprioprate algorithm group name ID for given algorithm name.
	 *
	 * @algorithmGroupName   
	 *
	 * @return          Algorithm group name ID.
	 */
	unsigned long getAlgorithmGroupNameId(string algorithmGroupName);

	/**
	 * Generates aprioprate algorithm type name for given algorithm name.
	 *
	 * @algorithmName   
	 *
	 * @return          Algorithm type name.
	 */
	string getAlgorithmType(string algorithmName);

	/**
	 * Prints reference points as text to given output stream.
	 *
	 * @os Output stream.
	 */
	void printReferencePoint(ofstream& os);

	/**
	 * Prints points as text to given output stream.
	 *
	 * @point           Point.
	 * @os              Output stream.
	 */
	void printPoint(Point& point, ofstream& os);

	/**
	 * Parses reference points string.
	 *
	 * @return     Vector of reference points definitions.
	 */
	vector<string> parseReferencePointsString();

	static const string ALGORITHM_GROUP_PARAMETER_NAME;

	static const string ALGORITHM_NAME_PARAMETER_NAME;

	static const string ALGORITHM_TYPE_PARAMETER_NAME;

	static const string EPS_PARAMETER_NAME;

	static const string MIN_PTS_PARAMETER_NAME;

	static const string K_PARAMETER_NAME;

	static const string USE_COSINE_SIMILARITY_PARAMETER_NAME;

	static const string IS_DATASET_FILE_FORMAT_DENSE_PARAMETER_NAME;

	static const string DATASET_FILE_PATH_PARAMETER_NAME;

	static const string DATASET_DIMENSION_PARAMETER_NAME;

	static const string DATASET_DIMENSION_VALUE_TRESHOLD_PARAMETER_NAME;

	static const string DATASET_ELEMENTS_NUMBER_PARAMETER_NAME;

	static const string IS_DATASET_INTERNAL_REPRESENTATION_DENSE_PARAMETER_NAME;

	static const string REFERENCE_POINT_PARAMETER_NAME;
	
	static const string IS_REFERENCE_POINT_FORMAT_DENSE_PARAMETER_NAME;

	static const string PROJECTION_DIMENSIONS_PARAMETER_NAME;

	static const string PROJECTION_SOURCE_SEQUENCE_PARAMETER_NAME;

	static const string CLASSIFICATION_SUBSET_FACTOR_PARAMETER_NAME;

	static const string USE_BINARY_PLACEMENT_PARAMETER_NAME;

	static const string P_SAMPLE_INDEX_PARAMETER_NAME;

	static const string S_SAMPLE_INDEX_PARAMETER_NAME;

	static const string Properties::SEARCH_METHOD_PARAMETER_NAME;

	static const string DENSE;

	static const string SPARSE;

	static const string IS_USE_DATASET_USE_INDEX;
};

#endif /* _PROPERTIES_H_ */