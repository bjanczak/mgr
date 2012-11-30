/*
 ============================================================================
 Name        : DbscanPointsElimination.h
 Author      : Bart�omiej Ja�czak
 Date        : 2012-11-30
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class implementing DBSCAN algorithm
               with points elimination
 ============================================================================
 */
#ifndef _DBSCAN_POINTS_ELIMINATION_H_
#define _DBSCAN_POINTS_ELIMINATION_H_

#include "Points.h"
#include "TiDbscanBase.h"

#include <list>

/**
 * Implements DBSCAN algorithm with points elimination.
 */
class DbscanPointsElimination: public TiDbscanBase{
	
public:

	DbscanPointsElimination();

	DbscanPointsElimination(const DbscanPointsElimination& object);

	/**
	 * Runs algorithm.
	 *
	 * @properties      Application properties.
	 * @dataset         Dataset of elements.
	 *
	 * @return          Execution times report as TimeReport object.
	 */
	virtual TimeReport run(const Properties& properties, Dataset& dataset);

private:

	/**
	 * Generates the Ti-Neighborhood of point in setOfPoints.
	 *
	 * @setOfPoints     Set of poits taken into consideration while
	 *                  calculating Ti-Neighborhood of point.
	 * @pointIt         Iterator pointing on the point for which 
	 *                  Ti-Neighborhood is calculated.
	 * @eps             Eps parameter.
	 *
	 * @return          Ti-Neighborhood of the point given as list of
	 *                  DbscanPoint*.
	 */
	static list<list<vector<DbscanPoint>::iterator>::iterator> indexTiNeighborhood(
		list<vector<DbscanPoint>::iterator>& setOfPoints
		, list<vector<DbscanPoint>::iterator>::iterator pointIt
		, const double eps);
};

#endif /* _DBSCAN_POINTS_ELIMINATION_H_ */