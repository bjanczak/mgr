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
	this->realDistanceCalculationsCounters = vector<unsigned long>();
	this->verificationRealDistanceCalculationsCounters = vector<unsigned long>();
	this->vpTreeSearchRealDistanceCalculations = vector<unsigned long>();
	this->makeVpTreeRealDistanceCalculations = 0;
	this->makeVpTreeSelectVpRealDistanceCalculations = 0;
}

TimeReport::~TimeReport(){
	this->realDistanceCalculationsCounters.clear();
	this->verificationRealDistanceCalculationsCounters.clear();
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
	this->realDistanceCalculationsCounters = vector<unsigned long>(object.realDistanceCalculationsCounters);
	this->verificationRealDistanceCalculationsCounters = vector<unsigned long>(object.verificationRealDistanceCalculationsCounters);
	this->vpTreeSearchRealDistanceCalculations = vector<unsigned long>(object.vpTreeSearchRealDistanceCalculations);
	this->makeVpTreeRealDistanceCalculations = object.makeVpTreeRealDistanceCalculations;
	this->makeVpTreeSelectVpRealDistanceCalculations = object.makeVpTreeSelectVpRealDistanceCalculations;
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
	this->realDistanceCalculationsCounters.clear();
	this->verificationRealDistanceCalculationsCounters.clear();
	this->vpTreeSearchRealDistanceCalculations.clear();
	this->makeVpTreeRealDistanceCalculations = 0;
	this->makeVpTreeSelectVpRealDistanceCalculations = 0;
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

	unsigned long realDistanceCalculationsCountersSum = 0;
	unsigned long verificationRealDistanceCalculationsCountersSum = 0;
	unsigned long vpTreeSearchRealDistanceCalculationsCountersSum = 0;
	vector<unsigned long>::iterator begin = this->realDistanceCalculationsCounters.begin();
	vector<unsigned long>::iterator end = this->realDistanceCalculationsCounters.end();	
	while(begin != end) {
		realDistanceCalculationsCountersSum = realDistanceCalculationsCountersSum + *begin;
		begin++;
	}
	begin = this->verificationRealDistanceCalculationsCounters.begin();
	end = this->verificationRealDistanceCalculationsCounters.end();	
	while(begin != end) {
		verificationRealDistanceCalculationsCountersSum = verificationRealDistanceCalculationsCountersSum + *begin;
		begin++;
	}
	begin = this->vpTreeSearchRealDistanceCalculations.begin();
	end = this->vpTreeSearchRealDistanceCalculations.end();	
	while(begin != end) {
		vpTreeSearchRealDistanceCalculationsCountersSum = vpTreeSearchRealDistanceCalculationsCountersSum + *begin;
		begin++;
	}
	os<<endl;
	os<<endl;
	os<<endl;
	os<<"********************************************"<<endl;
	os<<"     Real Distance Calculations Report"<<endl;
	os<<"********************************************"<<endl;
	os<<endl;
	os<<endl;
	os<<endl;
	if (this->realDistanceCalculationsCounters.size() > 0) {
		os<<"Real distance calculations                                : "<<realDistanceCalculationsCountersSum<<endl;
		os<<"Average real distance calculations                        : "<<realDistanceCalculationsCountersSum/this->realDistanceCalculationsCounters.size()<<endl;
	}
	if (this->verificationRealDistanceCalculationsCounters.size() > 0) {
		os<<"Verification real distance calculations                   : "<<verificationRealDistanceCalculationsCountersSum<<endl;
		os<<"Verification average real distance calculations           : "<<verificationRealDistanceCalculationsCountersSum/this->verificationRealDistanceCalculationsCounters.size()<<endl;	
	}
	if (this->makeVpTreeRealDistanceCalculations > 0) {
		os<<"Building Vp-Tree real distance calculations              : "<<this->makeVpTreeRealDistanceCalculations<<endl;
	}
	if (this->makeVpTreeRealDistanceCalculations > 0) {
		os<<"Building Vp-Tree VP select real distance calculations    : "<<this->makeVpTreeSelectVpRealDistanceCalculations<<endl;
	}
	if (this->vpTreeSearchRealDistanceCalculations.size() > 0) {
		os<<"Vp-Tree search real distance calculations                 : "<<vpTreeSearchRealDistanceCalculationsCountersSum<<endl;
		os<<"Vp-Tree search average real distance calculations         : "<<vpTreeSearchRealDistanceCalculationsCountersSum/this->vpTreeSearchRealDistanceCalculations.size()<<endl;	
	}
}