/*
 ============================================================================
 Name        : TimeReport.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-20
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of TimeReport.h
 ============================================================================
 */

#include "TimeReport.h"

TimeReport::TimeReport(){

	this->algorithmExecutionTime = DBL_MIN;
	this->clusteringExecutionTime = DBL_MIN;
	this->distanceCalculationExecutionTime = DBL_MIN;
	this->normalizingDatasetExecutionTime = DBL_MIN;
	this->positioningExecutionTime = DBL_MIN;
	this->sortingPointsExecutionTime = DBL_MIN;
	this->indexBuildingExecutionTime = DBL_MIN;
	this->datafileReadingTime = DBL_MIN;
	this->calculatingReferencePointsTime = DBL_MIN;
}

TimeReport::TimeReport( const TimeReport& object){

	this->algorithmExecutionTime = object.algorithmExecutionTime;
	this->clusteringExecutionTime = object.clusteringExecutionTime;
	this->distanceCalculationExecutionTime = object.distanceCalculationExecutionTime;
	this->normalizingDatasetExecutionTime = object.normalizingDatasetExecutionTime;
	this->positioningExecutionTime = object.positioningExecutionTime;
	this->sortingPointsExecutionTime = object.sortingPointsExecutionTime;
	this->indexBuildingExecutionTime = object.indexBuildingExecutionTime;
	this->datafileReadingTime = object.datafileReadingTime;
	this->calculatingReferencePointsTime = object.calculatingReferencePointsTime;
}

void TimeReport::clear(){

	this->algorithmExecutionTime = DBL_MIN;
	this->clusteringExecutionTime = DBL_MIN;
	this->distanceCalculationExecutionTime = DBL_MIN;
	this->normalizingDatasetExecutionTime = DBL_MIN;
	this->positioningExecutionTime = DBL_MIN;
	this->sortingPointsExecutionTime = DBL_MIN;
	this->indexBuildingExecutionTime = DBL_MIN;
	this->datafileReadingTime = DBL_MIN;
	this->calculatingReferencePointsTime = DBL_MIN;
}

void TimeReport::print(ofstream &os){

	os<<"********************************************"<<endl;
	os<<"                Time Report"<<endl;
	os<<"********************************************"<<endl;
	os<<endl;
	os<<endl;
	os<<endl;
	os<<"********Non Algorithm execution times*******"<<endl;
	os<<endl;
	os<<endl;
	if(this->datafileReadingTime != DBL_MIN){
		os<<"Reading dataset from file      : "<<this->datafileReadingTime<<" [s]"<<endl;
		os<<endl;
	}
	if(this->calculatingReferencePointsTime != DBL_MIN){
		os<<"Calculating reference points   : "<<this->calculatingReferencePointsTime<<" [s]"<<endl;
		os<<endl;
	}
	if(this->normalizingDatasetExecutionTime != DBL_MIN){
		
		os<<"Normalization                  : "<<this->normalizingDatasetExecutionTime<<" [s]"<<endl;
		os<<endl;
	}
	os<<endl;
	os<<"********Algorithm execution times*******"<<endl;
	os<<endl;
	os<<endl;
	if(this->algorithmExecutionTime != DBL_MIN){
	
		os<<"Algorithm execution            : "<<this->algorithmExecutionTime<<" [s]"<<endl;
		os<<endl;
	}
	if(this->clusteringExecutionTime != DBL_MIN){
	
		os<<"	Clustering                  : "<<this->clusteringExecutionTime<<" [s]"<<endl;
		os<<endl;
	}
	if(this->distanceCalculationExecutionTime != DBL_MIN){
	
		os<<"	Distance calculation        : "<<this->distanceCalculationExecutionTime<<" [s]"<<endl;
		os<<endl;
	}	
	if(this->sortingPointsExecutionTime != DBL_MIN){
		os<<"	Sorting                     : "<<this->sortingPointsExecutionTime<<" [s]"<<endl;
		os<<endl;
	}
	if(this->positioningExecutionTime != DBL_MIN){
		os<<"	Positioning                 : "<<this->positioningExecutionTime<<" [s]"<<endl;
		os<<endl;
	}
	if(this->indexBuildingExecutionTime != DBL_MIN){
		os<<"	Building index              : "<<this->indexBuildingExecutionTime<<" [s]"<<endl;
		os<<endl;
	}
}