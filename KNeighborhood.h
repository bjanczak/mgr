/*
 ============================================================================
 Name        : KNeighborhoodRef.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-29
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class implementing basic K-NEIGHBORHOOD
               algorithm.
 ============================================================================
 */
#ifndef _K_NEIGHBORHOOD_H_
#define _K_NEIGHBORHOOD_H_

#include "Points.h"
#include "KNeighborhoodBase.h"

/**
 * Implements basic K-NEIGHBORHOOD algorithm.
 */
class KNeighborhood: public KNeighborhoodBase{
	
public:

	KNeighborhood();

	KNeighborhood(const KNeighborhood& object);

	/**
	 * Runs algorithm.
	 *
	 * @properties      Application properties.
	 * @dataset         Dataset of elements.
	 *
	 * @return          Execution times report as TimeReport object.
	 */
	virtual TimeReport run(const Properties& properties, Dataset& dataset);

	/**
	 * Calculates K-Neighborhood for point given.
	 *
	 * @dataset                Dataset of elements.
	 * @pointIt                Iterator pointing to the point for which
	 *                         K-Neighborhood is calculated.
	 *
	 * @return                 Returns map of iterators to K neighbors
	 *                         of point given.
	 */
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator> indexTiKNeighborhood(
		vector<vector<KNeighborhoodPoint>::iterator>& dataset
		, vector<vector<KNeighborhoodPoint>::iterator>::iterator pointIt
		, unsigned long & realDistanceCalculationsCounter);
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator> tiKNeighborhood(
		vector<KNeighborhoodPoint>& dataset
		, vector<KNeighborhoodPoint>::iterator pointIt
		, unsigned long & realDistanceCalculationsCounter);

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
};


#endif /* _K_NEIGHBORHOOD_H_ */