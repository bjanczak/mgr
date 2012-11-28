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

	clock_t distanceCalculationStart;
	clock_t distanceCalculationFinish;
	clock_t sortingStart;
	clock_t sortingFinish;
	clock_t clusteringStart;
	clock_t clusteringFinish;
	clock_t positioningStart;
	clock_t positioningFinish;

	TimeReport timeReport;
	Point referencePoint = properties.referencePoints.front();

	vector<vector<KNeighborhoodPoint>::iterator>::iterator placementIt;
	
	vector<KNeighborhoodPoint> *tempDataset = &dataset.datasetKNeighborhoodPoint;
	vector<KNeighborhoodPoint>::iterator it;
	vector<KNeighborhoodPoint>::iterator end = tempDataset->end();

	vector<vector<KNeighborhoodPoint>::iterator> datasetIterators;
	vector<vector<KNeighborhoodPoint>::iterator>::iterator datasetIteratorsIt;
	vector<vector<KNeighborhoodPoint>::iterator>::iterator datasetIteratorsEnd;

	vector<pair<Point, Point*>> *classificationDataset = &dataset.classificationDataset;
	vector<pair<Point, Point*>>::iterator classificationIt;
	vector<pair<Point, Point*>>::iterator classificationEnd = classificationDataset->end();

	vector<pair<KNeighborhoodPoint, vector<vector<KNeighborhoodPoint>::iterator>::iterator>> classificationDatasetEquivalent;
	vector<pair<KNeighborhoodPoint, vector<vector<KNeighborhoodPoint>::iterator>::iterator>>::iterator classificationEquivalentIt;
	vector<pair<KNeighborhoodPoint, vector<vector<KNeighborhoodPoint>::iterator>::iterator>>::iterator classificationEquivalentEnd;
	

	this->k = properties.k;

	/*
	 * Build working index.
	 */
	for(it = tempDataset->begin(); it != end; it++){
	
		datasetIterators.push_back(it);
	}

	datasetIteratorsEnd = datasetIterators.end();

	/*
	 * Distance to reference point calculation.
	 */
	distanceCalculationStart = clock();

	for(it = tempDataset->begin(); it != end; it++){
	
		it->distance.push_back(Point::minkowskiDistance(referencePoint, (*it), 2));
	}

	/*
	 * Calculate criteria for classification dataset.
	 */	
	for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
		classificationIt->first.distance.push_back(Point::minkowskiDistance(referencePoint, classificationIt->first, 2));
	}

	distanceCalculationFinish = clock();

	/*
	 * Sorting points by distance to reference point.
	 */
	sortingStart = clock();

	sort(datasetIterators.begin(), datasetIterators.end(), KNeighborhoodPoint::distanceComparatorIterator);

	sortingFinish = clock();

	/*
	 * Find dataset points nearest to classification points by distance criteria.
	 */	
	positioningStart = clock();

	if(properties.useBinaryPlacement){
	
		for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
			placementIt = Dataset::getPlacementBinary(datasetIterators,classificationIt->first);
			classificationIt->second = &(**placementIt);
			classificationDatasetEquivalent.push_back(pair<Point, vector<vector<KNeighborhoodPoint>::iterator>::iterator>(classificationIt->first, placementIt));
		}
	}
	else{
	
		for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
			placementIt = Dataset::getPlacementLineary(datasetIterators,classificationIt->first);
			classificationIt->second = &(**placementIt);
			classificationDatasetEquivalent.push_back(pair<Point, vector<vector<KNeighborhoodPoint>::iterator>::iterator>(classificationIt->first, placementIt));
		}
	}

	positioningFinish = clock();

	/*
	 * Clustering.
	 */
	clusteringStart = clock();

	for(classificationEquivalentIt = classificationDatasetEquivalent.begin(); classificationEquivalentIt != classificationEquivalentEnd; classificationEquivalentIt++){
			
		(**classificationEquivalentIt->second).neighbors = tiKNeighborhood(datasetIterators, classificationEquivalentIt->second);
	}

	clusteringFinish = clock();

	timeReport.clusteringExecutionTime = ((double)(clusteringFinish - clusteringStart))/CLOCKS_PER_SEC;
	timeReport.distanceCalculationExecutionTime = ((double)(distanceCalculationFinish - distanceCalculationStart))/CLOCKS_PER_SEC;
	timeReport.sortingPointsExecutionTime =  ((double)(sortingFinish - sortingStart))/CLOCKS_PER_SEC;
	timeReport.algorithmExecutionTime = timeReport.clusteringExecutionTime + timeReport.distanceCalculationExecutionTime + timeReport.sortingPointsExecutionTime;	
	timeReport.positioningExecutionTime = ((double)(positioningFinish - positioningStart))/CLOCKS_PER_SEC;

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