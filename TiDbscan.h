/*
 ============================================================================
 Name        : TiDbscan.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-21
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class implementing basic TI-DBSCAN algorithm.
 ============================================================================
 */
#ifndef _TI_DBSCAN_H_
#define _TI_DBSCAN_H_

#include "Points.h"
#include "TiDbscanBase.h"

#include <list>

/**
 * Implements basic TI-DBSCAN algorithm.
 */
class TiDbscan: public TiDbscanBase{
	
public:

	TiDbscan();

	TiDbscan(const TiDbscan& object);

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
	 * Runs algorithm using index access to dataset.
	 *
	 * @properties      Application properties.
	 * @dataset         Dataset of elements.
	 *
	 * @return          Execution times report as TimeReport object.
	 */
	TimeReport runDatasetIndexAccess(const Properties& properties, Dataset& dataset);

	/**
	 * Runs algorithm using direct access to dataset.
	 *
	 * @properties      Application properties.
	 * @dataset         Dataset of elements.
	 *
	 * @return          Execution times report as TimeReport object.
	 */
	TimeReport runDatasetDirectAccess(const Properties& properties, Dataset& dataset);

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
	static list<list<DbscanPoint>::iterator> tiNeighborhood(
		list<DbscanPoint>& setOfPoints
		, list<DbscanPoint>::iterator pointIt
		, const double eps);

	/**
	 * Generates the forward Ti-Neighborhood of point in setOfPoints.
	 *
	 * @setOfPoints     Set of poits taken into consideration while
	 *                  calculating forward Ti-Neighborhood of point.
	 * @point           Iterator pointing on the point for which
	 *                  Ti-Neighborhood is calculated.
	 * @eps             Eps parameter.
	 *
	 * @return          Forward Ti-Neighborhood of the point given as 
	 *                  list of DbscanPoint*.
	 */
	static list<list<vector<DbscanPoint>::iterator>::iterator> indexTiForwardNeighborhood(
		list<vector<DbscanPoint>::iterator>& setOfPoints
		, list<vector<DbscanPoint>::iterator>::iterator pointIt
		, const double eps);
	static list<list<DbscanPoint>::iterator> tiForwardNeighborhood(
		list<DbscanPoint>& setOfPoints
		, list<DbscanPoint>::iterator pointIt
		, const double eps);

	/**
	 * Generates the backward Ti-Neighborhood of point in setOfPoints.
	 *
	 * @setOfPoints     Set of poits taken into consideration while
	 *                  calculating backward Ti-Neighborhood of point.
	 * @point           Iterator pointing on the point for which 
	 *                  Ti-Neighborhood is calculated.
	 * @eps             Eps parameter.
	 *
	 * @return          Backward Ti-Neighborhood of the point given as 
	 *                  list of DbscanPoint*.
	 */
	static list<list<vector<DbscanPoint>::iterator>::iterator> indexTiBackwardNeighborhood(
		list<vector<DbscanPoint>::iterator>& setOfPoints
		, list<vector<DbscanPoint>::iterator>::iterator pointIt
		, const double eps);
	static list<list<DbscanPoint>::iterator> tiBackwardNeighborhood(
		list<DbscanPoint>& setOfPoints
		, list<DbscanPoint>::iterator pointIt
		, const double eps);
};

#endif /* _TI_DBSCAN_H_ */