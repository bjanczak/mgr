/*
 ============================================================================
 Name        : Utils.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-20
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class Utils containing useful methods.
 ============================================================================
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>

using namespace std;

/**
 * Class containing useful methods.
 */
class Utils{

public:

	/**
	 * Calculates cosinus Eps.
	 *
	 * @eps 
	 *
	 * @return          Calculated value.
	 */
	static double getCosEps(double eps);

	/**
	 * Generates report name based on current date and time.
	 *
	 * @return          report name as string.
	 */
	static string reportNameGenerator();

	/**
	 * Generates ultimate report name based on current date and time.
	 *
	 * @return          report name as string.
	 */
	static string ultimateReportNameGenerator();

	/**
	 * Calculates median for given vector of values.
	 *
	 * @values          Vector of values for which median is calculated.
	 *
	 * @return          Median calculated for given values.
	 */
	static double median(const vector<double> values);

	/**
	 * Calculates median for given vector of values as well as, medians 
	 * grater and lesser neighbors.
	 *
	 * @values          Vector of values for which median and neighbors are calculated.
	 *
	 * @return          Three element vector where:
	 *						first element is median,
	 *                      second element is median lesser neighbor,
	 *                      third element is median greater neighbor.
	 */
	static vector<double> medianWithNeighbors(const vector<double> values);

	/**
	 * Calculates variance for given vector of values and median.
	 *
	 * @values          Vector of values for which variance is
	 *                  calculated.
	 * @mediane         median for which variance is calculated.
	 *
	 * @return          Variance calculated for given values and
	 *                  median.
	 */
	static double variance(const vector<double> values,const double median);
};
#endif /* _UTILS_H_ */
