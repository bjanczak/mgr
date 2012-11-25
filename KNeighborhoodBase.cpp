/*
 ============================================================================
 Name        : KNeighborhoodBase.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-24
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of KNeighborhoodBase.h
 ============================================================================
 */

#include "KNeighborhoodBase.h"

KNeighborhoodBase::KNeighborhoodBase():Algorithm(){

	this->algorithmName = "KNeighborhoodBase";
	this->k = 0;
}

KNeighborhoodBase::KNeighborhoodBase(const KNeighborhoodBase& object):Algorithm(object){

	this->algorithmName = "KNeighborhoodBase";
	this->k = object.k;
}