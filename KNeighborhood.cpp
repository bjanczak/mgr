/*
 ============================================================================
 Name        : KNeighborhood.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-29
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of KNeighborhood.h
 ============================================================================
 */

#include "KNeighborhood.h"

#include <algorithm>
#include <ctime>

KNeighborhood::KNeighborhood():KNeighborhoodBase(){

	this->algorithmName = "KNeighborhood";
}

KNeighborhood::KNeighborhood(const KNeighborhood& object):KNeighborhoodBase(object){

	this->algorithmName = "KNeighborhood";
}

TimeReport KNeighborhood::run(const Properties& properties, Dataset& dataset){

	clock_t clusteringStart;
	clock_t clusteringFinish;

	TimeReport timeReport;
	
	//Point referencePoint = properties.referencePoints.front();
	
	vector<KNeighborhoodPoint> *tempDataset = &dataset.datasetKNeighborhoodPoint;
	vector<KNeighborhoodPoint>::iterator it;
	vector<KNeighborhoodPoint>::iterator end = tempDataset->end();

	vector<vector<KNeighborhoodPoint>::iterator> datasetIterators;
	vector<vector<KNeighborhoodPoint>::iterator>::iterator datasetIteratorsIt;
	vector<vector<KNeighborhoodPoint>::iterator>::iterator datasetIteratorsEnd;
	

	this->k = properties.k;

	/*
	 * Build working index.
	 */
	for(it = tempDataset->begin(); it != end; it++){
	
		datasetIterators.push_back(it);
	}

	datasetIteratorsEnd = datasetIterators.end();

	/*
	 * Clustering.
	 */
	clusteringStart = clock();

	for(datasetIteratorsIt = datasetIterators.begin(); datasetIteratorsIt != datasetIteratorsEnd; datasetIteratorsIt++){
	
		(*datasetIteratorsIt)->neighbors = tiKNeighborhood(datasetIterators, datasetIteratorsIt);
	}

	clusteringFinish = clock();

	timeReport.algorithmExecutionTime = ((double)(clusteringFinish - clusteringStart))/CLOCKS_PER_SEC;
	timeReport.clusteringExecutionTime = timeReport.algorithmExecutionTime;

	return timeReport;
}

multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator> KNeighborhood::tiKNeighborhood(
	const vector<vector<KNeighborhoodPoint>::iterator>& dataset
	, const vector<vector<KNeighborhoodPoint>::iterator>::iterator pointIt){
	
	unsigned long foundNeighborsCounter = 1;
	
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator> kNeighborhood;
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>::iterator mapIt;
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>::iterator mapEnd;
	
	vector<vector<KNeighborhoodPoint>::iterator>::const_iterator it;
	vector<vector<KNeighborhoodPoint>::iterator>::const_iterator end = dataset.end();
	
	/*
	 * Brute force implementation.
	 */
	for(it = dataset.begin(); it != end; it++){
	
		if(it != pointIt){		
			kNeighborhood.insert(pair<double,vector<KNeighborhoodPoint>::iterator>(Point::minkowskiDistance((**pointIt), (**it), 2), *it));
		}
	}	
	
	mapIt = kNeighborhood.begin();
	mapEnd = kNeighborhood.end();
	
	while(foundNeighborsCounter<k){
	
		mapIt++;
		foundNeighborsCounter++;
	}

	(*pointIt)->eps = mapIt->first;
	mapIt++;
	
	kNeighborhood.erase(mapIt, mapEnd);

	return kNeighborhood;
}