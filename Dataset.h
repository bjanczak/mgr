/*
 ============================================================================
 Name        : Dataset.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-05
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of Dataset class holding dataset elements.
 ============================================================================
 */

#ifndef _DATASET_H_
#define _DATASET_H_

#include "Points.h"
#include "Properties.h"

#include <string>
#include <vector>

using namespace std;

/**
 * Dataset class.
 */
class Dataset{

public:

	bool isDense;
	unsigned long dimension;
	unsigned long size;
	string algorithmGroup;
	string algorithmType;
	string algorithmName;
	string filePath;

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
	 * One of following vectors will be used by algorithms group.
	 */
	vector<Point> datasetPoint;
	vector<DbscanPoint> datasetDbscanPoint;
	vector<KNeighborhoodPoint> datasetKNeighborhoodPoint;

	/**
	 * Classification dataset
	 */
	vector<pair<Point, Point*>> classificationDataset;

	/**
	 * Classification result.
	 */
	vector<pair<pair<Point*, double>, list<Point*>>> classificationResult;

	/**
	 * Vps-tree index.
	 */
	VpsPoint* vpsTree;

	~Dataset();

	static Dataset& getInstance(){
		static Dataset instance;
		return instance;
	}

	/**
	 * Sets object properties.
	 *
	 * @properties      Application properties.
	 */
	void setProperties(const Properties& properties);

	/**
	 * Clears all the data.
	 */
	void clear();

	/**
	 * Reads elements data from dataset.
	 *
	 * @properties      Application properties.
	 */
	void readData(const Properties& properties);

	/**
	 * Calculates projection dimensions based on properties.
	 *
	 * @properties      Application properties.
	 *
	 */
	void readProjectionDimensions(Properties& properties);

	/**
	 * Returns number of widest dimension in dataset.
	 */
	unsigned long getMaxDimension();

	/**
	 * Returns number of narrowest dimension in dataset.
	 */
	unsigned long getMinDimension();

	/**
	 * Generates vector of reference points defined in given properties.
	 *
	 * @properties      Application properties.
	 *
	 * @return          Reference points as vector of Point objects.
	 */
	vector<Point> getReferencePoints(const Properties& properties);

	/**
	 * Normalizes dataset.
	 * Every dimension value becomes a result of division of the value
	 * by distance of a point from the begining of the scale.
	 */
	void normalize(double alfa);

	/**
	 * Prints clustering sum up int given os output stream.
	 * 
	 * @os Output stream to which report is written.
	 */
	void printClusteringSumUp(ofstream& os);

	/**
	 * Finds most similar point in datasetKNeighborhoodPoint by
	 * given criteria, which are compared to point's distance.
	 * Uses binary search.
	 *
	 * @point			Point to find nearest neighbor.
	 *
	 * @return          Iterator pointing to the point most similar to
	 *                  criteria given.
	 */
	vector<KNeighborhoodPoint>::iterator getPlacementBinary(const Point& point);

	/**
	 * Finds most similar point in datasetKNeighborhoodPoint by
	 * given criteria, which are compared to point's distance.
	 * Uses lineary search.
	 *
	 * @point			Point to find nearest neighbor.
	 *
	 * @return          Iterator pointing to the point most similar to
	 *                  criteria given.
	 */
	vector<KNeighborhoodPoint>::iterator getPlacementLineary(const Point& point);

	/**
	 * Finds most similar point in datasetKNeighborhoodPoint by
	 * given criteria, which are compared to point's distance.
	 * Uses binary search.
	 *
	 * @datasetIndex    Vector of indexes pointing on dataset.
	 * @point			Point to find nearest neighbor.
	 *
	 * @return          Iterator pointing to the point most similar to
	 *                  criteria given.
	 */
	static vector<vector<KNeighborhoodPoint>::iterator>::iterator indexGetPlacementBinary(vector<vector<KNeighborhoodPoint>::iterator>& datasetIndex, const Point& point, unsigned long& comparisonCounter);
	static vector<KNeighborhoodPoint>::iterator getPlacementBinary(vector<KNeighborhoodPoint>& datasetIndex, const Point& point, unsigned long& comparisonCounter);

	/**
	 * Finds most similar point in datasetKNeighborhoodPoint by
	 * given criteria, which are compared to point's distance.
	 * Uses lineary search.
	 *
	 * @datasetIndex    Vector of indexes pointing on dataset.
	 * @point			Point to find nearest neighbor.
	 *
	 * @return          Iterator pointing to the point most similar to
	 *                  criteria given.
	 */
	static vector<vector<KNeighborhoodPoint>::iterator>::iterator indexGetPlacementLineary(vector<vector<KNeighborhoodPoint>::iterator>& datasetIndex, const Point& point);
	static vector<KNeighborhoodPoint>::iterator getPlacementLineary(vector<KNeighborhoodPoint>& datasetIndex, const Point& point);

	/**
	 * Copies dataset to result.
	 *
	 * @result          Vector to be filled with points from dataset.
	 * @dataset         Dataset from which points are to be copied.
	 * @isDense         Flag indicating whether dataset is in dense or sparse format.
	 */
	static void fillKNeighborhoodPointVector(vector<KNeighborhoodPoint>& result, const vector<Point>& dataset);
	// TODO bjanczak delete
	//static void fillKNeighborhoodPointVectorPoint(vector<KNeighborhoodPoint>* result, const vector<Point*>& dataset, const bool isDense);
	static void fillDbscanPointVector(vector<DbscanPoint>& result, const vector<Point>& dataset);
	static void fillVpsPointVector(vector<VpsPoint>& result, const vector<Point>& dataset, const bool isDense);

	/**
	 * Copies dataset to result.
	 *
	 * @result          List to be filled with points from dataset.
	 * @dataset         Dataset from which points are to be copied.
	 * @isDense         Flag indicating whether dataset is in dense or sparse format.
	 */
	static void fillVpsPointList(list<VpsPoint>& result, const vector<Point>& dataset, const bool isDense);

	/**
	 * Generates subset of dataset.
	 *
	 * @dataset         Dataset from which points are to be copied.
	 * @properties      Application properties.
	 *
	 * @return          Subset of dataset as vector of Point objects.
	 */
	static vector<Point> generateSample(const vector<Point>& dataset, const Properties& properties);

	/**
	 * Calculates aggregation values for K-NEIGHBORHOOD algorithms results eps, stores them in internal values as well as in parameters.
	 */
	void calculateKNeighborhoodEps(double& minEps, double& avgEps, double& maxEps);

	/**
	 * Calculates aggregation values for VP_TREE algorithms results eps, stores them in internal values as well as in parameters.
	 */
	void calculateClassificationResultEps(double& minEps, double& avgEps, double& maxEps);
	
	/**
	 * Generates Point object that has zero value for each
	 * dimension.
	 *
	 * @dataset         Dataset of elements.
	 *
	 * @return          Generated point as Point object.
	 */
	static Point getZeroPoint(const Dataset& dataset);

private:

	Dataset();

	/**
	 * Creates dense point from string representation.
	 *
	 * @line            String defining dense point.
	 * @properties      Application properties.
	 *
	 * @return          Created dense point as vector of doubles.
	 */
	vector<double> getDensePoint(string line, const Properties& properties);

	/**
	 * Creates sprase point from string representation.
	 *
	 * @line            String defining sparse point.
	 * @properties      Application properties.
	 *
	 * @return          Created sparse point as vector of SparsePoint objects.
	 */
	vector<SparsePoint> getSparsePoint(string line, const Properties& properties);

	/**
	 * Generates new string without white characters.
	 *
	 * @str             Case string.
	 *
	 * @return          Generated string.
	 */
	static string deleteWhiteChars(const string& str);

	/**
	 * Generates new string without multiple space characters.
	 *
	 * @str             Case string.
	 *
	 * @return          Generated string.
	 */
	static string deleteMultipleSpaceCharacters(const string& str);

	/**
	 * Generates vector of instructions from custom point definition.
	 * Definition is a vector of pairs <dimension, value selector>.
	 * Value selector can have values: min, max, 0.
	 * Value selector chooses value for given dimension from dataset.
	 *
	 * @definition      String definition of the point.
	 * @isDense         flag describing whether definition has dense
	 *                  or sparse format.
	 *
	 * @return          Vector of instructions as vector of pairs <dimension, value selector>.
	 */
	static vector<pair<unsigned long, string>> customPointDefinitionToInstructionVector(const string& definition, bool isDense);

	/**
	 * Generates unsigned long value from string definition.
	 *
	 * @definition      Value definition.
	 *
	 * @return          Generated value.
	 */
	static unsigned long generateNFromDefinition(const string definition);

	/**
	 * Generates Point object that has n value for each
	 * dimension.
	 *
	 * @dataset         Dataset of elements.
	 * @n               Every dimension value.
	 *
	 * @return          Generated point as Point object.
	 */
	static Point getNPoint(const Dataset& dataset, const unsigned int n);

	/**
	 * Verifies, whether point definition pointDefinition is
	 * function point definition or not.
	 *
	 * @pointDefinition Point definition.
	 *
	 * @returns         True is pointDefinition indicates that point
	 *                  is function point, or false otherwise.
	 */
	static bool isFunctionReferencePoint(const string pointDefinition);

	/**
	 * Verifies, whether point definition pointDefinition is
	 * custom point definition or not.
	 *
	 * @pointDefinition Point definition.
	 *
	 * @returns         True is pointDefinition indicates that point
	 *                  is custom point, or false otherwise.
	 */
	static bool isCustomReferencePoint(const string pointDefinition);

	/**
	 * Verifies, whether point definition pointDefinition is
	 * pattern point definition or not.
	 *
	 * @pointDefinition Point definition.
	 *
	 * @returns         True is pointDefinition indicates that point
	 *                  is pattern point, or false otherwise.
	 */
	static bool isPatternReferencePoint(const string pointDefinition);

	/**
	 * Generates Point object of properties defined by definition string.
	 *
	 * @definition      String definition of the point.
	 *
	 * @return          Generated point as Point object.      
	 */
	Point getCustomPoint(string definition);

	/**
	 * Generates Point object of properties defined by definition string.
	 *
	 * @definition      String definition of the point.
	 *
	 * @return          Generated point as Point object.      
	 */
	Point getReferenceCustomPoint(string definition);

	/**
	 * Generates Point object of properties defined by definition string.
	 *
	 * @definition      String definition of the point.
	 * @isDense         flag describing whether definition has dense
	 *                  or sparse format.
	 * @dimension       Number of dimensions.
	 *
	 * @return          Generated point as Point object.      
	 */
	static Point getPatternReferenceCustomPoint(string definition, bool isDense, unsigned long dimension);

	/**
	 * Generates Point object that has random dimension value for each
	 * dimension in dataset not greater tham maximal value in each dimension.
	 *
	 * @return          Generated point as Point object.
	 */
	Point getRandomPoint();

	/**
	 * Generates Point object that has maxium dimension value for each
	 * dimension in dataset.
	 *
	 * @return          Generated point as Point object.
	 */
	Point getMaxPoint();

	/**
	 * Generates Point object that has maxium dimension value for each
	 * odd dimension in dataset and minimum dimension value for each even
	 * dimension value.
	 *
	 * @return          Generated point as Point object.
	 */
	Point getMaxMinPoint();
	
	/**
	 * Calculates maximum dimension value for given dimension.
	 *
	 * @dimension      Dimension fof which maximal value will be found.
	 *
	 * @return          Calculated value as double.
	 */
	double maxDimensionValue(const unsigned long dimension);

	/**
	 * Generates Point object that has minimum dimension value for each
	 * dimension in dataset.
	 *
	 * @return          Generated point as Point object.
	 */
	Point getMinPoint();

	/**
	 * Calculates minimum dimension value for given dimension.
	 *
	 * @dimension       Dimension fof which minimal value will be found.
	 *
	 * @return          Calculated value as double.
	 */
	double minDimensionValue(const unsigned long dimension);
};
#endif /*_DATASET_H_*/