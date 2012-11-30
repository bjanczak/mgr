/*
 ============================================================================
 Name        : TiDbscanRef.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-23
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class implementing basic TI-DBSCAN-REF
               algorithm.
 ============================================================================
 */
#ifndef _TI_DBSCAN_REF_H_
#define _TI_DBSCAN_REF_H_

#include "Points.h"
#include "TiDbscanBase.h"

#include <list>

/**
 * Implements basic TI-DBSCAN-REF algorithm.
 */
class TiDbscanRef: public TiDbscanBase{
	
public:

	TiDbscanRef();

	TiDbscanRef(const TiDbscanRef& object);

	/**
	 * Runs algorithm.
	 *
	 * @properties      Application properties.
	 * @dataset         Dataset of elements.
	 *
	 * @return          Execution times report as TimeReport object.
	 */
	virtual TimeReport run(const Properties& properties, Dataset& dataset);

protected:

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

	/**
	 * Verifies whether the query point given (queryPointIt) is a neighbor
	 * of the point given (pointIt).
	 *
	 * @pointIt                Point for which 
	 *                         query point neighborhood is verified.
	 * @queryPointIt           Iterator pointing to the point verified
	 *                         to be the point (pointIt) neighbor.
	 * @eps                    Neighborhood radius.
	 *
	 * @return                 True if the query point (queryPointIt)
	 *                         is a neighbor of the point given (pointIt),
	 *                         or false otherwise.
	 */
	static bool indexIsCandidateNeighborByAdditionalReferencePoints(
		list<vector<DbscanPoint>::iterator>::iterator pointIt
		, list<vector<DbscanPoint>::iterator>::iterator queryPointIt
		, double eps);
	static bool isCandidateNeighborByAdditionalReferencePoints(
		list<DbscanPoint>::iterator pointIt
		, list<DbscanPoint>::iterator queryPointIt
		, double eps);
};

#endif /* _TI_DBSCAN_REF_H_ */