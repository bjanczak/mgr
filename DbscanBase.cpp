/*
 ============================================================================
 Name        : DbscanBase.cpp
 Author      : Bart�omiej Ja�czak
 Date        : 2012-09-23
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of TiDbscanBase.h
 ============================================================================
 */

#include "DbscanBase.h"

DbscanBase::DbscanBase():Algorithm(){

	this->algorithmName = "DbscanBase";
	this->eps = 0;
	this->minPts = 0;
}

DbscanBase::DbscanBase(const DbscanBase& object):Algorithm(object){

	this->algorithmName = "DbscanBase";
	this->eps = object.eps;
	this->minPts = object.minPts;
}