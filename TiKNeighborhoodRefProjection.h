/*
 ============================================================================
 Name        : TiKNeighborhoodRefProjection.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-25
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class implementing TI-K-NEIGHBORHOOD-REF
               with projection algorithm.
 ============================================================================
 */
#ifndef _TI_K_NEIGHBORHOOD_REF_PROJECTION_H_
#define _TI_K_NEIGHBORHOOD_REF_PROJECTION_H_

#include "Points.h"
#include "TiKNeighborhoodRef.h"

/**
 * Implements TI-K-NEIGHBORHOOD-REF with projection algorithm.
 */
class TiKNeighborhoodRefProjection: public TiKNeighborhoodRef{
	
public:

	TiKNeighborhoodRefProjection();

	TiKNeighborhoodRefProjection(const TiKNeighborhoodRefProjection& object);

	/**
	 * Runs algorithm.
	 *
	 * @properties      Application properties.
	 * @dataset         Dataset of elements.
	 *
	 * @return          Execution times report as TimeReport object.
	 */
	virtual TimeReport run(const Properties& properties, Dataset& dataset);
};

#endif /* _TI_K_NEIGHBORHOOD_REF_PROJECTION_H_ */