/*
 ============================================================================
 Name        : TiKNeighborhoodBase.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-21
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class used for TI-K-NEIGHBORHOOD algorithm
               implementations.
 ============================================================================
 */
#ifndef _TI_K_NEIGHBORHOOD_BASE_H_
#define _TI_K_NEIGHBORHOOD_BASE_H_

#include "KNeighborhoodBase.h"
#include "Points.h"

#include <map>

/**
 * Declaration of backward neighborhood verification function type.
 * Function verifies found K neighbors of point given (pointIt).
 * Inserts found neighbors to kNeighborhood collection.
 *
 * @dataset                Dataset of elements.
 * @pointIt                Iterator pointing to the point for which 
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
typedef void verifyKCandidateNeighborsBackwardFunction (
	const vector<vector<KNeighborhoodPoint>::iterator>& dataset
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator pointIt
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointBackwardIt
	, bool& backwardSearch
	, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
	, unsigned long k);

/**
 * Declaration of forward neighborhood verification function type.
 * Function verifies found K neighbors of point given (pointIt).
 * Inserts found neighbors to kNeighborhood collection.
 *
 * @dataset                Dataset of elements.
 * @pointIt                Iterator pointing to the point for which 
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
typedef void verifyKCandidateNeighborsForwardFunction (
	const vector<vector<KNeighborhoodPoint>::iterator>& dataset
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator pointIt
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointForwardIt
	, bool& forwardSearch
	, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
	, unsigned long k);

/**
 * Basic class for TI-K-NEIGHBORHOOD algorithm implementation.
 * Every class that implements an TI-K-NEIGHBORHOOD algorithm 
 * must inherit TiKNeighborhoodBase class.
 */
class TiKNeighborhoodBase: public KNeighborhoodBase{
	
public:

	TiKNeighborhoodBase();

	TiKNeighborhoodBase(const TiKNeighborhoodBase& object);

protected:

	/**
	 * Calculates K-Neighborhood for point given.
	 *
	 * @dataset                Dataset of elements.
	 * @pointIt                Iterator pointing to the point for which
	 *                         K-Neighborhood is calculated.
	 * @backwardVerification   Function verifying K neighborhood searching
	 *                         backward.
	 * @forwardVerification    Function verifying K neighborhood searching
	 *                         forward.
	 *
	 * @return                 Returns map of iterators to K neighbors
	 *                         of point given.
	 */
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator> tiKNeighborhood(
		const vector<vector<KNeighborhoodPoint>::iterator>& dataset
		, vector<vector<KNeighborhoodPoint>::iterator>::iterator pointIt
		, verifyKCandidateNeighborsBackwardFunction backwardVerification
		, verifyKCandidateNeighborsForwardFunction forwardVerification);

	/**
	 * Changes position of pointIt from the point, that pointIt
	 * is currently pointing to a point immediately preceding the point,
	 * if such a point exists.
	 *
	 * @dataset                Dataset of elements.
	 * @pointIt                Iterator pointing to the point for which
	 *                         preceding points is searched.
	 *
	 * @return                 True if preceding point exists, or false
	 *                         otherwise.
	 */
	static bool precedingPoint(
		const vector<vector<KNeighborhoodPoint>::iterator>& dataset
		, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointIt);

	/**
	 * Changes position of pointIt from the point, that pointIt
	 * is currently pointing to a point immediately following the point,
	 * if such a point exists.
	 *
	 * @dataset                Dataset of elements.
	 * @pointIt                Iterator pointing to the point for which 
	 *                         following points is searched.
	 *
	 * @return                 True if following point exists, or false 
	 *                         otherwise.
	 */
	static bool followingPoint(
		const vector<vector<KNeighborhoodPoint>::iterator>& dataset
		, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointIt);

	/**
	 * Finds first K neighbors of point given (pointIt), searching forward
	 * and backward. Inserts found neighbors to kNeighborhood collection.
	 *
	 * @dataset                Dataset of elements.
	 * @pointIt                Iterator pointing to the point for which 
	 *                         neighbors are found.
	 * @pointBackwardIt        Iterator pointing to the point preceding
	 *                         a point pointIt is pointing at.
	 * @pointForwardIt         Iterator pointing to the point following
	 *                         a point pointIt is pointing at.
	 * @backwardSearch         Indicates whether backward search is possible.
	 * @forwardSearch          Indicates whether forward search is possible.
	 * @kNeighborhood          Collection holding pointers to
	 *                         the point (pointIt) neighbors.
	 * @foundNeighboursCounter Indicates how many neighbors were already
	 *                         found for the point (pointIt).
	 */
	void findFirstKCandidateNeighborsForwardAndBackward(
		const vector<vector<KNeighborhoodPoint>::iterator>& dataset
		, vector<vector<KNeighborhoodPoint>::iterator>::iterator pointIt
		, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointBackwardIt
		, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointForwardIt
		, bool &backwardSearch
		, bool &forwardSearch
		, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
		, unsigned long& foundNeighboursCounter);

	/**
	 * Finds first K neighbors of point given (pointIt), searching backward.
	 * Inserts found neighbors to kNeighborhood collection.
	 *
	 * @dataset                Dataset of elements.
	 * @pointIt                Iterator pointing to the point for which 
	 *                         neighbors are found.
	 * @pointBackwardIt        Iterator pointing to the point preceding
	 *                         a point pointIt is pointing at.
	 * @backwardSearch         Indicates whether backward search is possible. 
	 * @kNeighborhood          Collection holding pointers to
	 *                         the point (pointIt) neighbors.
	 * @foundNeighboursCounter Indicates how many neighbors were already
	 *                         found for the point (pointIt).
	 */
	void findFirstKCandidateNeighborsBackward(
		const vector<vector<KNeighborhoodPoint>::iterator>& dataset
		, vector<vector<KNeighborhoodPoint>::iterator>::iterator pointIt
		, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointBackwardIt
		, bool &backwardSearch
		, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
		, unsigned long& foundNeighboursCounter);
	
	/**
	 * Finds first K neighbors of point given (pointIt), searching forward.
	 * Inserts found neighbors to kNeighborhood collection.
	 *
	 * @dataset                Dataset of elements.
	 * @pointIt                Iterator pointing to the point for which 
	 *                         neighbors are found.
	 * @pointForwardIt         Iterator pointing to the point following
	 *                         a point pointIt is pointing at.
	 * @forwardSearch          Indicates whether forward search is possible.
	 * @kNeighborhood          Collection holding pointers to
	 *                         the point (pointIt) neighbors.
	 * @foundNeighboursCounter Indicates how many neighbors were already
	 *                         found for the point (pointIt).
	 */
	void findFirstKCandidateNeighborsForward(
		const vector<vector<KNeighborhoodPoint>::iterator>& dataset
		, vector<vector<KNeighborhoodPoint>::iterator>::iterator pointIt
		, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointForwardIt
		, bool &forwardSearch
		, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
		, unsigned long& foundNeighboursCounter);

	/**
	 * Finds maximum distance between neighbor and point.
	 * Seaarches among kNeighborhood neighbors.
	 *
	 * @kNeighborhood          Collection holding pointers to
	 *                         the point neighbors.
	 *
	 * @return                 Found value.
	 */
	static double getMaxDistance(const multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood);

	/**
	 * Calculates number of elements in kNeighborhood of key equal the key given.
	 *
	 * @kNeighborhood          Collection holding pointers to
	 *                         the point neighbors.
	 * @key                    
	 *
	 * @return                 Calculated value.
	 */
	static unsigned long getKeysNr(const multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood, const double key);
};
#endif /* _TI_K_NEIGHBORHOOD_BASE_H_ */