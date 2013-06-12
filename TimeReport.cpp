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
	this->placementComparisonCounters = vector<unsigned long>();
	this->makeVpTreeRealDistanceCalculations = 0;
	this->makeVpTreeSelectVpRealDistanceCalculations = 0;
	this->vpTreeHeight = 0;
	this->vpTreeLeafs = 0;
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
	this->placementComparisonCounters = vector<unsigned long>(object.placementComparisonCounters);
	this->makeVpTreeRealDistanceCalculations = object.makeVpTreeRealDistanceCalculations;
	this->makeVpTreeSelectVpRealDistanceCalculations = object.makeVpTreeSelectVpRealDistanceCalculations;
	this->vpTreeHeight = object.vpTreeHeight;
	this->vpTreeLeafs = object.vpTreeLeafs;
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
	this->placementComparisonCounters.clear();
	this->makeVpTreeRealDistanceCalculations = 0;
	this->makeVpTreeSelectVpRealDistanceCalculations = 0;
	this->vpTreeHeight = 0;
	this->vpTreeLeafs = 0;
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
	unsigned long realDistanceCalculationsCountersMin = ULONG_MAX;
	unsigned long realDistanceCalculationsCountersMax = 0;
	unsigned long verificationRealDistanceCalculationsCountersSum = 0;
	unsigned long verificationRealDistanceCalculationsCountersMin = ULONG_MAX;
	unsigned long verificationRealDistanceCalculationsCountersMax = 0;
	unsigned long vpTreeSearchRealDistanceCalculationsCountersSum = 0;
	unsigned long vpTreeSearchRealDistanceCalculationsCountersMin = ULONG_MAX;
	unsigned long vpTreeSearchRealDistanceCalculationsCountersMax = 0;
	unsigned long placementComparisonCountersSum = 0;
	unsigned long placementComparisonCountersMin = ULONG_MAX;
	unsigned long placementComparisonCountersMax = 0;

	vector<unsigned long>::iterator begin = this->realDistanceCalculationsCounters.begin();
	vector<unsigned long>::iterator end = this->realDistanceCalculationsCounters.end();	
	while(begin != end) {
		realDistanceCalculationsCountersSum = realDistanceCalculationsCountersSum + *begin;
		if (*begin > realDistanceCalculationsCountersMax) {
			realDistanceCalculationsCountersMax = *begin;
		}
		if (*begin < realDistanceCalculationsCountersMin) {
			realDistanceCalculationsCountersMin = *begin;
		}
		begin++;
	}
	begin = this->verificationRealDistanceCalculationsCounters.begin();
	end = this->verificationRealDistanceCalculationsCounters.end();	
	while(begin != end) {
		verificationRealDistanceCalculationsCountersSum = verificationRealDistanceCalculationsCountersSum + *begin;
		if (*begin > verificationRealDistanceCalculationsCountersMax) {
			verificationRealDistanceCalculationsCountersMax = *begin;
		}
		if (*begin < verificationRealDistanceCalculationsCountersMin) {
			verificationRealDistanceCalculationsCountersMin = *begin;
		}
		begin++;
	}
	begin = this->vpTreeSearchRealDistanceCalculations.begin();
	end = this->vpTreeSearchRealDistanceCalculations.end();	
	while(begin != end) {
		vpTreeSearchRealDistanceCalculationsCountersSum = vpTreeSearchRealDistanceCalculationsCountersSum + *begin;
		if (*begin > vpTreeSearchRealDistanceCalculationsCountersMax) {
			vpTreeSearchRealDistanceCalculationsCountersMax = *begin;
		}
		if (*begin < vpTreeSearchRealDistanceCalculationsCountersMin) {
			vpTreeSearchRealDistanceCalculationsCountersMin = *begin;
		}
		begin++;
	}
	begin = this->placementComparisonCounters.begin();
	end = this->placementComparisonCounters.end();	
	while(begin != end) {
		placementComparisonCountersSum = placementComparisonCountersSum + *begin;
		if (*begin > placementComparisonCountersMax) {
			placementComparisonCountersMax = *begin;
		}
		if (*begin < placementComparisonCountersMin) {
			placementComparisonCountersMin = *begin;
		}
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
		os<<"Min real distance calculations                            : "<<realDistanceCalculationsCountersMin<<endl;
		os<<"Max real distance calculations                            : "<<realDistanceCalculationsCountersMax<<endl;
	}
	if (this->verificationRealDistanceCalculationsCounters.size() > 0) {
		os<<"Verification real distance calculations                   : "<<verificationRealDistanceCalculationsCountersSum<<endl;
		os<<"Verification average real distance calculations           : "<<verificationRealDistanceCalculationsCountersSum/this->verificationRealDistanceCalculationsCounters.size()<<endl;
		os<<"Verification min real distance calculations               : "<<verificationRealDistanceCalculationsCountersMin<<endl;
		os<<"Verification max real distance calculations               : "<<verificationRealDistanceCalculationsCountersMax<<endl;
	}
	if (this->placementComparisonCounters.size() > 0) {
		os<<"Placement comparisons                                     : "<<placementComparisonCountersSum<<endl;
		os<<"Placement average comparisons                             : "<<placementComparisonCountersSum/this->placementComparisonCounters.size()<<endl;
		os<<"Placement min comparisons                                 : "<<placementComparisonCountersMin<<endl;
		os<<"Placement max comparisons                                 : "<<placementComparisonCountersMax<<endl;
	}
	if (this->makeVpTreeRealDistanceCalculations > 0) {
		os<<"Building Vp-Tree real distance calculations               : "<<this->makeVpTreeRealDistanceCalculations<<endl;
	}
	if (this->makeVpTreeRealDistanceCalculations > 0) {
		os<<"Building Vp-Tree VP select real distance calculations     : "<<this->makeVpTreeSelectVpRealDistanceCalculations<<endl;
	}
	if (this->vpTreeHeight > 0) {
		os<<"Vp-Tree height                                            : "<<this->vpTreeHeight<<endl;
	}
	if (this->vpTreeLeafs > 0) {
		os<<"Vp-Tree leafs                                             : "<<this->vpTreeLeafs<<endl;
	}
	if (this->vpTreeSearchRealDistanceCalculations.size() > 0) {
		os<<"Vp-Tree search real distance calculations                 : "<<vpTreeSearchRealDistanceCalculationsCountersSum<<endl;
		os<<"Vp-Tree search average real distance calculations         : "<<vpTreeSearchRealDistanceCalculationsCountersSum/this->vpTreeSearchRealDistanceCalculations.size()<<endl;
		os<<"Vp-Tree search min real distance calculations             : "<<vpTreeSearchRealDistanceCalculationsCountersMin<<endl;	
		os<<"Vp-Tree search max real distance calculations             : "<<vpTreeSearchRealDistanceCalculationsCountersMax<<endl;	
	}
}