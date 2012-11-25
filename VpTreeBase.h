/*
 ============================================================================
 Name        : VpTreeBase.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-10-02
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class implementing algorithm building Vantage
               Point tree index.
 ============================================================================
 */
#ifndef _VP_TREE_BASE_H_
#define _VP_TREE_BASE_H_

#include "Algorithm.h"
#include "Points.h"

#include <vector>
#include <list>


using namespace std;

/**
 * Basic class for classes implementing an algorithm building Vantage Point
 * tree index. Every class implementing an algorithm building Vantage Point
 * tree index must inherit VpTreeBase class.
 */
class VpTreeBase: public Algorithm{

public:

	/**
	 * Maximal size of pRandomSample.
	 */
	unsigned long pSampleLimit;

	/**
	 * Maximal size of dRandomSample.
	 */
	unsigned long dSampleLimit;

	VpTreeBase();

	VpTreeBase(const VpTreeBase& object);

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
	 * Searches k neighbors of query point in vantage point tree index.
	 *
	 * @query           Query point for which k neighbors are searched.
	 * @eps             Search radius.
	 * @point           Currently examined vantage point.
	 * @neighbors       List of pointers to found neighbors.
	 * @neighborsSize   Size of neighbors list.
	 * @k               Number of neighbors searched.
	 */
	virtual void search(
		Point* query
		, double& eps
		, VpsPoint* point
		, list<Point*>& neighbors
		, unsigned long& neighborsSize
		, unsigned long k);

protected :

	/**
	 * Selects best vanage point from list of candidates.
	 *
	 * @candidates      Candidates from which vantage point is selected.
	 *
	 * @return          Iterator pointing on selected vantage point.
	 */
	list<VpsPoint>::iterator selectVp(list<list<VpsPoint>::iterator>& candidates);

	/**
	 * Selects random subset of given candidates set.
	 *
	 * @candidates      Candidates from which subset is selected.
	 * @sampleLimit     Maximal sample size.
	 *
	 * @return          Selected subset as new list of VpsPoint.
	 */
	vector<list<list<VpsPoint>::iterator>::iterator> randomSample(
		list<list<VpsPoint>::iterator>& candidates
		, unsigned long sampleLimit);
};
#endif /* _VP_TREE_BASE_H_ */