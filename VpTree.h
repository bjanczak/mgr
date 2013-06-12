/*
 ============================================================================
 Name        : VpTree.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-10-02
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration algorithm building VP-tree index.
 ============================================================================
 */

#ifndef _VP_TREE_H_
#define _VP_TREE_H_

#include "VpTreeBase.h"
#include "Points.h"

#include <vector>
#include <list>


using namespace std;

/**
 * Implements algorithm building Vp-tree index. Saves the index 
 * in dataset under vpsTree parameter.
 */
class VpTree: public VpTreeBase{

public:

	VpTree();

	VpTree(const VpTree& object);

	/**
	 * Runs algorithm.
	 *
	 * @properties      Application properties.
	 * @dataset         Dataset of elements.
	 *
	 * @return          Execution times report as TimeReport object.
	 */
	virtual TimeReport run(const Properties& properties
		, Dataset& dataset);

	/**
	 * Searches k neighbors of query point in vantage point tree index.
	 *
	 * @query           Query point for which k neighbors are searched.
	 * @point           Currently examined vantage point.
	 * @neighbors       Map of pointers to found neighbors.
	 */
	virtual void kNeighborhoodSearch(
		Point* query
		, VpsPoint* point
		, multimap<double, Point*, DistanceComparator>& neighbors
		, unsigned long& realdDistanceCalculations);
	virtual void boundariesKNeighborhoodSearch(
		Point* query
		, VpsPoint* point
		, multimap<double, Point*, DistanceComparator>& neighbors
		, unsigned long& realdDistanceCalculations);

	/**
	 * Searches neighbors in tau neighborhood of query point in vantage
	 * point tree index.
	 *
	 * @query           Query point for which k neighbors are searched.
	 * @tau             Search radius.
	 * @point           Currently examined vantage point.
	 * @neighbors       Map of pointers to found neighbors.
	 */
	virtual void rangeSearch(
		Point* query
		, double& tau
		, VpsPoint* point
		, multimap<double, Point*, DistanceComparator>& neighbors);
	virtual void boundariesRangeSearch(
		Point* query
		, double& tau
		, VpsPoint* point
		, multimap<double, Point*, DistanceComparator>& neighbors);
	
	/**
	 * Calculates height.
	 */
	unsigned long calculateHeight(VpsPoint* point);

	/**
	 * Calculates leafs.
	 */
	unsigned long calculatesLeafs(VpsPoint* point);

private :

	/**
	 * Recursively builds Vp Tree from given candidates.
	 *
	 * @candidates      List of iterators pointing to points from which
	 *                  Vp Tree is build.
	 * @dataset         Points dataset on which candidates iterators points.
	 *
	 * @return          Pointer to created Vp Tree node.
	 */
	VpsPoint* makeVpTree(list<list<VpsPoint>::iterator>& candidates, list<VpsPoint>& dataset , unsigned long& realDistanceCalculations, unsigned long& selectVpRealDistanceCalculations);
};

#endif /* _VP_TREE_H_ */