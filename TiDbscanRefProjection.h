/*
 ============================================================================
 Name        : TiDbscanRefProjection.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-23
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class implementing basic TI-DBSCAN-REF
               with projection algorithm.
 ============================================================================
 */
#ifndef _TI_DBSCAN_REF_PROJECTION_H_
#define _TI_DBSCAN_REF_PROJECTION_H_

#include "Points.h"
#include "TiDbscanRef.h"

/**
 * Implements basic TI-DBSCAN-REF with projection algorithm.
 */
class TiDbscanRefProjection: public TiDbscanRef{
	
public:

	TiDbscanRefProjection();

	TiDbscanRefProjection(const TiDbscanRefProjection& object);

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
};

#endif /* _TI_DBSCAN_REF_PROJECTION_H_ */