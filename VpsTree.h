/*
 ============================================================================
 Name        : VpsTree.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-10-01
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration algorithm building VPS-tree index.
 ============================================================================
 */

#ifndef _VPS_TREE_H_
#define _VPS_TREE_H_

#include "VpTreeBase.h"
#include "Points.h"

#include <vector>
#include <list>


using namespace std;

/**
 * Implements algorithm building Vps-tree index. Saves the index 
 * in dataset under vpsTree parameter.
 */
class VpsTree: public VpTreeBase{

public:

	VpsTree();

	VpsTree(const VpsTree& object);

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

private :

	/**
	 * Recursively builds Vps Tree from given candidates.
	 *
	 * @candidates      List of points from which Vps Tree is build.
	 *
	 * @return          Pointer to created Vps Tree node.
	 */
	VpsPoint* recurseVpsTree(list<VpsPoint>& candidates);
	
	/**
	 * Builds points boundaries list if point has left and right child.
	 * 
	 * @point           Point for which boundaries list is build.
	 */
	void buildBoundariesList(VpsPoint& point);
};

#endif /* _VPS_TREE_H_ */