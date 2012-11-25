/*
 ============================================================================
 Name        : Algorithm.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-20
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of basic class for algorithms implementation.
 ============================================================================
 */
#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include "Dataset.h"
#include "Properties.h"
#include "TimeReport.h"

#include <string>

/**
 * Basic class for algorithm implementation.
 * Every class that implements an algorithm must inherit Algorithm class.
 */
class Algorithm{

public:
	
	/**
	 * Algorithm name.
	 */
	string algorithmName;

	/**
	 * Runs algorithm.
	 *
	 * @properties      Application properties.
	 * @dataset         Dataset of elements.
	 *
	 * @return          Execution times report as TimeReport object.
	 */
	virtual TimeReport run(const Properties& properties, Dataset& dataset);

	Algorithm();

	Algorithm(const Algorithm& object);
};
#endif /* _ALGORITHM_H_ */