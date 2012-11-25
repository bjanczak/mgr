/*
 ============================================================================
 Name        : DbscanBase.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-23
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of class for classes implementing DBSCAN algorithm
               variants.
 ============================================================================
 */
#ifndef _DBSCAN_BASE_H_
#define _DBSCAN_BASE_H_

#include "Algorithm.h"

/**
 * Basic class for DBSCAN algorithm implementations and it's variants.
 * Every class that implements a variant of DBSCAN algorithm must inherit 
 * DbscanBase class.
 */
class DbscanBase: public Algorithm{

public:

	/**
	 * DBSCAN MinPts parameter.
	 */
	unsigned int minPts;

	/**
	 * DBSCAN Eps aparameter.
	 */
	double eps;

	DbscanBase();

	DbscanBase(const DbscanBase& object);
};

#endif /* _DBSCAN_BASE_H_ */