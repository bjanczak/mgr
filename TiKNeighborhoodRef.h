/*
 ============================================================================
 Name        : TiKNeighborhoodRef.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-25
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class implementing TI-K-NEIGHBORHOOD-REF
               algorithm.
 ============================================================================
 */
#ifndef _TI_K_NEIGHBORHOOD_REF_H_
#define _TI_K_NEIGHBORHOOD_REF_H_

#include "Points.h"
#include "TiKNeighborhoodBase.h"

/**
 * Implements TI-K-NEIGHBORHOOD-REF algorithm.
 */
class TiKNeighborhoodRef: public TiKNeighborhoodBase{
	
public:

	TiKNeighborhoodRef();

	TiKNeighborhoodRef(const TiKNeighborhoodRef& object);

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
	 * Verifies found K neighbors of point given (pointIt) using many
	 * reference points. Inserts found neighbors to kNeighborhood collection.
	 *
	 * @dataset                Dataset of elements.
	 * @point                  Point for which 
	 *                         neighbors are verified.
	 * @pointBackwardIt        Iterator pointing to the point preceding
	 *                         a point pointIt is pointing at.
	 * @backwardSearch         Indicates whether backward search is possible. 
	 * @kNeighborhood          Collection holding pointers to
	 *                         the point (pointIt) neighbors.
	 * @k                      Parameter of every K-NEIGHBORHOOD variant
	 *                         algorithm. K is a number of negihbor points
	 *                         to be found for point given.
	 */
	static void indexVerifyKCandidateNeighborsBackward (
		const vector<vector<KNeighborhoodPoint>::iterator>& dataset
		, KNeighborhoodPoint& point
		, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointBackwardIt
		, bool& backwardSearch
		, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
		, unsigned long k
		, unsigned long& realDistanceCalculationsCounter);
	static void verifyKCandidateNeighborsBackward (
		const vector<KNeighborhoodPoint>& dataset
		, KNeighborhoodPoint& point
		, vector<KNeighborhoodPoint>::iterator& pointBackwardIt
		, bool& backwardSearch
		, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
		, unsigned long k
		, unsigned long& realDistanceCalculationsCounter);

	/**
	 * Verifies found K neighbors of point given (pointIt) using many
	 * reference points. Inserts found neighbors to kNeighborhood collection.
	 *
	 * @dataset                Dataset of elements.
	 * @point                  Point for which 
	 *                         neighbors are verified.
	 * @pointForwardIt         Iterator pointing to the point following
	 *                         a point pointIt is pointing at.
	 * @forwardSearch          Indicates whether forward search is possible. 
	 * @kNeighborhood          Collection holding pointers to
	 *                         the point (pointIt) neighbors.
	 * @k                      Parameter of every K-NEIGHBORHOOD variant
	 *                         algorithm. K is a number of negihbor points
	 *                         to be found for point given.
	 */
	static void indexVerifyKCandidateNeighborsForward (
		const vector<vector<KNeighborhoodPoint>::iterator>& dataset
		, KNeighborhoodPoint& point
		, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointForwardIt
		, bool& forwardSearch
		, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
		, unsigned long k
		, unsigned long& realDistanceCalculationsCounter);
	static void verifyKCandidateNeighborsForward (
		const vector<KNeighborhoodPoint>& dataset
		, KNeighborhoodPoint& point
		, vector<KNeighborhoodPoint>::iterator& pointForwardIt
		, bool& forwardSearch
		, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
		, unsigned long k
		, unsigned long& realDistanceCalculationsCounter);

	/**
	 * Verifies whether the query point given (queryPointIt) is a neighbor
	 * of the point given (pointIt).
	 *
	 * @point                  Point for which 
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
		const KNeighborhoodPoint& point
		, vector<vector<KNeighborhoodPoint>::iterator>::iterator queryPointIt
		, double eps);
	static bool isCandidateNeighborByAdditionalReferencePoints(
		const KNeighborhoodPoint& point
		, vector<KNeighborhoodPoint>::iterator queryPointIt
		, double eps);
};

#endif /* _TI_K_NEIGHBORHOOD_REF_H_ */