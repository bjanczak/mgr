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
#include <vector>
#include <ctime>

KNeighborhood::KNeighborhood():KNeighborhoodBase(){

	this->algorithmName = "KNeighborhood";
}

KNeighborhood::KNeighborhood(const KNeighborhood& object):KNeighborhoodBase(object){

	this->algorithmName = "KNeighborhood";
}

TimeReport KNeighborhood::run(const Properties& properties, Dataset& dataset){

	TimeReport timeReport;

	if(properties.isUseDatasetIndexAcess){
	
		timeReport = runDatasetIndexAccess(properties, dataset);
	}
	else{
	
		timeReport = runDatasetDirectAccess(properties, dataset);
	}

	return timeReport;
}

TimeReport KNeighborhood::runDatasetIndexAccess(const Properties& properties, Dataset& dataset){

	clock_t distanceCalculationStart;
	clock_t distanceCalculationFinish;
	clock_t sortingStart;
	clock_t sortingFinish;
	clock_t clusteringStart;
	clock_t clusteringFinish;
	clock_t positioningStart;
	clock_t positioningFinish;
	clock_t indexBuildingStart;
	clock_t indexBuildingFinish;

	TimeReport timeReport;

	Point referencePoint = Dataset::getZeroPoint(dataset);

	vector<vector<KNeighborhoodPoint>::iterator>::iterator indexPlacementIt;
	
	vector<KNeighborhoodPoint> *tempDataset = &dataset.datasetKNeighborhoodPoint;
	vector<KNeighborhoodPoint>::iterator it;
	vector<KNeighborhoodPoint>::iterator end = tempDataset->end();

	vector<vector<KNeighborhoodPoint>::iterator> datasetIterators;
	vector<vector<KNeighborhoodPoint>::iterator>::iterator datasetIteratorsIt;
	vector<vector<KNeighborhoodPoint>::iterator>::iterator datasetIteratorsEnd;

	vector<pair<Point, Point*>> *classificationDataset = &dataset.classificationDataset;
	vector<pair<Point, Point*>>::iterator classificationIt;
	vector<pair<Point, Point*>>::iterator classificationEnd = classificationDataset->end();

	vector<pair<KNeighborhoodPoint, vector<vector<KNeighborhoodPoint>::iterator>::iterator>> classificationDatasetIndexEquivalent;
	vector<pair<KNeighborhoodPoint, vector<vector<KNeighborhoodPoint>::iterator>::iterator>>::iterator classificationIndexEquivalentIt;
	vector<pair<KNeighborhoodPoint, vector<vector<KNeighborhoodPoint>::iterator>::iterator>>::iterator classificationIndexEquivalentEnd;
	
	vector<unsigned long> placementComparisonCounters;

	this->k = properties.k;

	/*
	 * Build working index.
	 */
	indexBuildingStart = clock();

	for(it = tempDataset->begin(); it != end; it++){
	
		datasetIterators.push_back(it);
	}

	indexBuildingFinish = clock();

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
		unsigned long placementComparisonCounter;
		for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
			placementComparisonCounter = 0;
			indexPlacementIt = Dataset::indexGetPlacementBinary(datasetIterators,classificationIt->first, placementComparisonCounter);
			classificationIt->second = &(**indexPlacementIt);
			classificationDatasetIndexEquivalent.push_back(pair<Point, vector<vector<KNeighborhoodPoint>::iterator>::iterator>(classificationIt->first, indexPlacementIt));
			placementComparisonCounters.push_back(placementComparisonCounter);
		}
	}
	else{
	
		for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
			indexPlacementIt = Dataset::indexGetPlacementLineary(datasetIterators,classificationIt->first);
			classificationIt->second = &(**indexPlacementIt);
			classificationDatasetIndexEquivalent.push_back(pair<Point, vector<vector<KNeighborhoodPoint>::iterator>::iterator>(classificationIt->first, indexPlacementIt));
		}
	}

	positioningFinish = clock();

	/*
	 * Clustering.
	 */
	clusteringStart = clock();

	classificationIndexEquivalentEnd = classificationDatasetIndexEquivalent.end();

	for(classificationIndexEquivalentIt = classificationDatasetIndexEquivalent.begin(); classificationIndexEquivalentIt != classificationIndexEquivalentEnd; classificationIndexEquivalentIt++){
			
		(**classificationIndexEquivalentIt->second).neighbors = indexTiKNeighborhood(datasetIterators, classificationIndexEquivalentIt->second);
	}

	clusteringFinish = clock();

	timeReport.clusteringExecutionTime = ((double)(clusteringFinish - clusteringStart))/CLOCKS_PER_SEC;
	timeReport.distanceCalculationExecutionTime = ((double)(distanceCalculationFinish - distanceCalculationStart))/CLOCKS_PER_SEC;
	timeReport.sortingPointsExecutionTime =  ((double)(sortingFinish - sortingStart))/CLOCKS_PER_SEC;
	timeReport.algorithmExecutionTime = timeReport.clusteringExecutionTime;	
	timeReport.positioningExecutionTime = ((double)(positioningFinish - positioningStart))/CLOCKS_PER_SEC;
	timeReport.indexBuildingExecutionTime = ((double)(indexBuildingFinish - indexBuildingStart))/CLOCKS_PER_SEC;
	timeReport.placementComparisonCounters = vector<unsigned long>(placementComparisonCounters);

	return timeReport;
}

TimeReport KNeighborhood::runDatasetDirectAccess(const Properties& properties, Dataset& dataset){

	clock_t distanceCalculationStart;
	clock_t distanceCalculationFinish;
	clock_t sortingStart;
	clock_t sortingFinish;
	clock_t clusteringStart;
	clock_t clusteringFinish;
	clock_t positioningStart;
	clock_t positioningFinish;

	TimeReport timeReport;

	Point referencePoint = Dataset::getZeroPoint(dataset);

	vector<KNeighborhoodPoint>::iterator placementIt;
	
	vector<KNeighborhoodPoint> *tempDataset = &dataset.datasetKNeighborhoodPoint;
	vector<KNeighborhoodPoint>::iterator it;
	vector<KNeighborhoodPoint>::iterator end = tempDataset->end();

	vector<pair<Point, Point*>> *classificationDataset = &dataset.classificationDataset;
	vector<pair<Point, Point*>>::iterator classificationIt;
	vector<pair<Point, Point*>>::iterator classificationEnd = classificationDataset->end();

	vector<pair<KNeighborhoodPoint, vector<KNeighborhoodPoint>::iterator>> classificationDatasetEquivalent;
	vector<pair<KNeighborhoodPoint, vector<KNeighborhoodPoint>::iterator>>::iterator classificationEquivalentIt;
	vector<pair<KNeighborhoodPoint, vector<KNeighborhoodPoint>::iterator>>::iterator classificationEquivalentEnd;

	vector<unsigned long> placementComparisonCounters;
	
	this->k = properties.k;

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

	sort(tempDataset->begin(), tempDataset->end(), KNeighborhoodPoint::distanceComparator);

	sortingFinish = clock();

	/*
	 * Find dataset points nearest to classification points by distance criteria.
	 */	
	positioningStart = clock();

	if(properties.useBinaryPlacement){
		unsigned long placementComparisonCounter;
		for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
			placementComparisonCounter = 0;
			placementIt = Dataset::getPlacementBinary(*tempDataset,classificationIt->first, placementComparisonCounter);
			classificationIt->second = &(*placementIt);
			classificationDatasetEquivalent.push_back(pair<Point, vector<KNeighborhoodPoint>::iterator>(classificationIt->first, placementIt));
			placementComparisonCounters.push_back(placementComparisonCounter);
		}
	}
	else{
	
		for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
			placementIt = Dataset::getPlacementLineary(*tempDataset,classificationIt->first);
			classificationIt->second = &(*placementIt);
			classificationDatasetEquivalent.push_back(pair<Point,  vector<KNeighborhoodPoint>::iterator>(classificationIt->first, placementIt));
		}
	}

	positioningFinish = clock();

	/*
	 * Clustering.
	 */
	clusteringStart = clock();

	classificationEquivalentEnd = classificationDatasetEquivalent.end();

	for(classificationEquivalentIt = classificationDatasetEquivalent.begin(); classificationEquivalentIt != classificationEquivalentEnd; classificationEquivalentIt++){
			
		(*classificationEquivalentIt->second).neighbors = tiKNeighborhood(*tempDataset, classificationEquivalentIt->second);
	}

	clusteringFinish = clock();

	timeReport.clusteringExecutionTime = ((double)(clusteringFinish - clusteringStart))/CLOCKS_PER_SEC;
	timeReport.distanceCalculationExecutionTime = ((double)(distanceCalculationFinish - distanceCalculationStart))/CLOCKS_PER_SEC;
	timeReport.sortingPointsExecutionTime =  ((double)(sortingFinish - sortingStart))/CLOCKS_PER_SEC;
	timeReport.algorithmExecutionTime = timeReport.clusteringExecutionTime;	
	timeReport.positioningExecutionTime = ((double)(positioningFinish - positioningStart))/CLOCKS_PER_SEC;
	timeReport.placementComparisonCounters = vector<unsigned long>(placementComparisonCounters);

	return timeReport;
}

multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator> KNeighborhood::indexTiKNeighborhood(
	vector<vector<KNeighborhoodPoint>::iterator>& dataset
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator pointIt){
	
	unsigned long foundNeighborsCounter = 1;
	
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator> kNeighborhood;
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>::iterator mapIt;
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>::iterator mapEnd;
	
	vector<vector<KNeighborhoodPoint>::iterator>::iterator it;
	vector<vector<KNeighborhoodPoint>::iterator>::iterator end = dataset.end();
	
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

multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator> KNeighborhood::tiKNeighborhood(
	vector<KNeighborhoodPoint>& dataset
	, vector<KNeighborhoodPoint>::iterator pointIt){
	
	unsigned long foundNeighborsCounter = 1;
	
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator> kNeighborhood;
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>::iterator mapIt;
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>::iterator mapEnd;
	
	vector<KNeighborhoodPoint>::iterator it;
	vector<KNeighborhoodPoint>::iterator end = dataset.end();
	
	for(unsigned int i = 0; i < 2*this->k; i++){
		
		kNeighborhood.insert(pair<double,vector<KNeighborhoodPoint>::iterator>(DBL_MAX, it));
	}

	/*
	 * Brute force implementation.
	 */
	for(it = dataset.begin(); it != end; it++){
	
		if(it != pointIt){		
			
			kNeighborhood.insert(pair<double,vector<KNeighborhoodPoint>::iterator>(Point::minkowskiDistance((*pointIt), (*it), 2), it));
		}
	}	
	
	mapIt = kNeighborhood.begin();
	mapEnd = kNeighborhood.end();
	
	while(foundNeighborsCounter<k){
	
		mapIt++;
		foundNeighborsCounter++;
	}

	pointIt->eps = mapIt->first;
	mapIt++;
	
	kNeighborhood.erase(mapIt, mapEnd);

	return kNeighborhood;
}