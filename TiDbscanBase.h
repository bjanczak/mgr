/*
 ============================================================================
 Name        : TiDbscanBase.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-21
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class for TI-DBSCAN algorithm implementations.
 ============================================================================
 */
#ifndef _TI_DBSCAN_BASE_H_
#define _TI_DBSCAN_BASE_H_

#include "DbscanBase.h"
#include "Points.h"

#include <list>

/**
 * Declaration of neighborhood function type.
 *
 * @setOfPoints     Set of poits taken into consideration while calculating
 *                  neighborhood of point.
 * @point           Points for which neighborhood is calculated.
 * @eps             Eps parameter.
 *
 * @return          neighborhood of the point given as list of DbscanPoint*.
 */
typedef list<list<vector<DbscanPoint>::iterator>::iterator> indexNeighborhoodFunction (
	list<vector<DbscanPoint>::iterator>& setOfPoints
	, list<vector<DbscanPoint>::iterator>::iterator pointIt
	, const double eps);
typedef list<list<DbscanPoint>::iterator> neighborhoodFunction (
	list<DbscanPoint>& setOfPoints
	, list<DbscanPoint>::iterator pointIt
	, const double eps);

/**
 * Basic class for TI-DBSCAN algorithm implementation.
 * Every class that implements an TI-DBSCAN algorithm must inherit
 * TiDbscanBase class.
 */
class TiDbscanBase: public DbscanBase{
	
public:

	TiDbscanBase();

	TiDbscanBase(const TiDbscanBase& object);

	/**
	 * Rewrites given vector to list memory efficient.
	 *
	 * @dataset         vector of poits.
	 *
	 * @return          Generated list.
	 */
	static list<DbscanPoint> vectorToList(vector<DbscanPoint>& dataset);

	/**
	 * Rewrites given list to list vector efficient.
	 *
	 * @dataset         list of poits.
	 *
	 * @return          Generated list.
	 */
	static vector<DbscanPoint> listToVector(list<DbscanPoint>& dataset);

protected:

	/**
	 * Expands cluster.
	 *
	 * @dataset         Set of poits taken into consideration while expanting
	 *                  a cluster.
	 * @pointIt         Iterator pointing to the point that is supposed to be 
	 *                  cluster core point.
	 * @clusterId       Expended cluster id.
	 * @neighborhood    Function returning points neighborhood.
	 *
	 * @return          false if point belongs to NOICE, otherwise
	 *                  returns true.
	 */
	bool indexExpandCluster(
		list<vector<DbscanPoint>::iterator>& dataset
		, list<vector<DbscanPoint>::iterator>::iterator& pointIt
		, unsigned int clusterId
		, indexNeighborhoodFunction *neighborhood);
	bool expandCluster(
		list<DbscanPoint>& dataset
		, list<DbscanPoint>::iterator& pointIt
		, unsigned int clusterId
		, neighborhoodFunction *neighborhood);
};
#endif /* _TI_DBSCAN_BASE_H_ */