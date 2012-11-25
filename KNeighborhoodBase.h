/*
 ============================================================================
 Name        : KNeighborhoodBase.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-24
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class for classes implementing K-NEIGHBORHOOD
               algorithm variants.
 ============================================================================
 */
#ifndef _K_NEIGHBORHOOD_BASE_H_
#define _K_NEIGHBORHOOD_BASE_H_

#include "Algorithm.h"

/**
 * Basic class for K-NEIGHBORHOOD algorithm implementations
 * and it's variants.
 * Every class that implements a variant of K-NEIGHBORHOOD algorithm
 * must inherit DbscanBase class.
 */
class KNeighborhoodBase: public Algorithm{

public:

	/**
	 * K parameter of every K-NEIGHBORHOOD variant algorithm.
	 * K is a number of negihbor points to be found for point given.
	 */
	unsigned int k;

	KNeighborhoodBase();

	KNeighborhoodBase(const KNeighborhoodBase& object);
};

#endif /* _K_NEIGHBORHOOD_BASE_H_ */