/*
 ============================================================================
 Name        : Dbscan.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-20
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class implementing basic DBSCAN algorithm.
 ============================================================================
 */
#ifndef _DBSCAN_H_
#define _DBSCAN_H_

#include "DbscanBase.h"
#include "Points.h"

#include <list>

/**
 * Implements basic DBSCAN algorithm.
 */
class Dbscan: public DbscanBase{
	
public:

	Dbscan();

	Dbscan(const Dbscan& object);

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
	 * Expands cluster.
	 *
	 * @setOfPoints     Set of poits taken into consideration while expanting
	 *                  a cluster.
	 * @point           Points that is supposed to be cluster core point.
	 * @clusterId       Expended cluster id.
	 *
	 * @return          false if point belongs to NOICE, otherwise
	 *                  returns true.
	 */
	bool expandCluster(
		vector<DbscanPoint>& setOfPoints
		, DbscanPoint& point
		,const unsigned int clusterId);

	/**
	 * Generates the Eps-Neighborhood of point in setOfPoints.
	 *
	 * @setOfPoints     Set of poits taken into consideration while
	 *                  calculating Eps-Neighborhood of point.
	 * @point           Points for which Eps-Neighborhood is calculated.
	 *
	 * @return          Eps-Neighborhood of the point given as list of
	 *                  DbscanPoint*.
	 */
	list<DbscanPoint*> regionQuery(
		vector<DbscanPoint>& setOfPoints
		, const DbscanPoint& point);
};

#endif /* _DBSCAN_H_ */