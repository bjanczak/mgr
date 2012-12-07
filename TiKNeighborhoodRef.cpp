/*
 ============================================================================
 Name        : TiKNeighborhoodRef.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-25
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of TiKNeighborhoodRef.h
 ============================================================================
 */

#include "TiKNeighborhoodRef.h"

#include <algorithm>
#include <cmath>
#include <ctime>

TiKNeighborhoodRef::TiKNeighborhoodRef():TiKNeighborhoodBase(){

	this->algorithmName = "TiKNeighborhoodRef";
}

TiKNeighborhoodRef::TiKNeighborhoodRef(const TiKNeighborhoodRef& object):TiKNeighborhoodBase(object){

	this->algorithmName = "TiKNeighborhoodRef";
}

TimeReport TiKNeighborhoodRef::run(const Properties& properties, Dataset& dataset){

	TimeReport timeReport;

	if(properties.isUseDatasetIndexAcess){
	
		timeReport = runDatasetIndexAccess(properties, dataset);
	}
	else{
	
		timeReport = runDatasetDirectAccess(properties, dataset);
	}

	return timeReport;
}

TimeReport TiKNeighborhoodRef::runDatasetIndexAccess(const Properties& properties, Dataset& dataset){

	TimeReport timeReport;
	vector<KNeighborhoodPoint> *tempDataset = &dataset.datasetKNeighborhoodPoint;
	
	vector<KNeighborhoodPoint>::iterator it;
	vector<KNeighborhoodPoint>::iterator end = tempDataset->end();

	vector<vector<KNeighborhoodPoint>::iterator>::iterator indexPlacementIt;
	
	vector<vector<KNeighborhoodPoint>::iterator> datasetIterators;
	vector<vector<KNeighborhoodPoint>::iterator>::iterator datasetIteratorsIt;
	vector<vector<KNeighborhoodPoint>::iterator>::iterator datasetIteratorsEnd;

	vector<pair<Point, Point*>> *classificationDataset = &dataset.classificationDataset;
	vector<pair<Point, Point*>>::iterator classificationIt;
	vector<pair<Point, Point*>>::iterator classificationEnd = classificationDataset->end();
	
	vector<pair<KNeighborhoodPoint, vector<vector<KNeighborhoodPoint>::iterator>::iterator>> classificationDatasetIndexEquivalent;
	vector<pair<KNeighborhoodPoint, vector<vector<KNeighborhoodPoint>::iterator>::iterator>>::iterator classificationIndexEquivalentIt;
	vector<pair<KNeighborhoodPoint, vector<vector<KNeighborhoodPoint>::iterator>::iterator>>::iterator classificationIndexEquivalentEnd;
	
	const vector<Point> *referencePoints = &properties.referencePoints;
	vector<Point>::const_iterator referencePointsIt;
	vector<Point>::const_iterator referencePointsEnd = referencePoints->end();
	
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

	this->k = properties.k;

	/*
	 * Build working index.
	 */
	indexBuildingStart = clock();

	for(it = tempDataset->begin(); it != end; it++){
	
		datasetIterators.push_back(it);
	}

	indexBuildingFinish = clock();

	/*
	 * Distance to reference point calculation.
	 */
	distanceCalculationStart = clock();

	for(it = tempDataset->begin(); it != end; it++){
	
		for(referencePointsIt = referencePoints->begin(); referencePointsIt!= referencePointsEnd; referencePointsIt++){
		
			(*it).distance.push_back(Point::minkowskiDistance(*referencePointsIt, (*it), 2));		
		}
	}

	/*
	 * Calculate criteria fo classification dataset.
	 */	
	for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
		for(referencePointsIt = referencePoints->begin(); referencePointsIt!= referencePointsEnd; referencePointsIt++){
				
			classificationIt->first.distance.push_back(Point::minkowskiDistance((*referencePointsIt), classificationIt->first, 2));
		}
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
		
			indexPlacementIt = Dataset::indexGetPlacementBinary(datasetIterators, classificationIt->first);
			classificationIt->second = &(**indexPlacementIt);
			classificationDatasetIndexEquivalent.push_back(pair<Point, vector<vector<KNeighborhoodPoint>::iterator>::iterator>(classificationIt->first, indexPlacementIt));
		}
	}
	else{
	
		for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
			indexPlacementIt = Dataset::indexGetPlacementLineary(datasetIterators, classificationIt->first);
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
				
		(**classificationIndexEquivalentIt->second).neighbors = indexTiKNeighborhood(datasetIterators, classificationIndexEquivalentIt->second, classificationIndexEquivalentIt->first, TiKNeighborhoodRef::indexVerifyKCandidateNeighborsBackward, TiKNeighborhoodRef::indexVerifyKCandidateNeighborsForward);
	}

	clusteringFinish = clock();

	timeReport.clusteringExecutionTime = ((double)(clusteringFinish - clusteringStart))/CLOCKS_PER_SEC;
	timeReport.distanceCalculationExecutionTime = ((double)(distanceCalculationFinish - distanceCalculationStart))/CLOCKS_PER_SEC;
	timeReport.sortingPointsExecutionTime =  ((double)(sortingFinish - sortingStart))/CLOCKS_PER_SEC;
	timeReport.indexBuildingExecutionTime = ((double)(indexBuildingFinish - indexBuildingStart))/CLOCKS_PER_SEC;
	timeReport.algorithmExecutionTime = timeReport.clusteringExecutionTime + timeReport.distanceCalculationExecutionTime + timeReport.sortingPointsExecutionTime + timeReport.indexBuildingExecutionTime;
	timeReport.positioningExecutionTime = ((double)(positioningFinish - positioningStart))/CLOCKS_PER_SEC;

	return timeReport;
}

TimeReport TiKNeighborhoodRef::runDatasetDirectAccess(const Properties& properties, Dataset& dataset){
	
	TimeReport timeReport;
	vector<KNeighborhoodPoint> *tempDataset = &dataset.datasetKNeighborhoodPoint;
	
	vector<KNeighborhoodPoint>::iterator it;
	vector<KNeighborhoodPoint>::iterator end = tempDataset->end();

	vector<KNeighborhoodPoint>::iterator placementIt;

	vector<pair<Point, Point*>> *classificationDataset = &dataset.classificationDataset;
	vector<pair<Point, Point*>>::iterator classificationIt;
	vector<pair<Point, Point*>>::iterator classificationEnd = classificationDataset->end();
	
	vector<pair<KNeighborhoodPoint, vector<KNeighborhoodPoint>::iterator>> classificationDatasetEquivalent;
	vector<pair<KNeighborhoodPoint, vector<KNeighborhoodPoint>::iterator>>::iterator classificationEquivalentIt;
	vector<pair<KNeighborhoodPoint, vector<KNeighborhoodPoint>::iterator>>::iterator classificationEquivalentEnd;
	
	const vector<Point> *referencePoints = &properties.referencePoints;
	vector<Point>::const_iterator referencePointsIt;
	vector<Point>::const_iterator referencePointsEnd = referencePoints->end();
	
	clock_t distanceCalculationStart;
	clock_t distanceCalculationFinish;
	clock_t sortingStart;
	clock_t sortingFinish;
	clock_t clusteringStart;
	clock_t clusteringFinish;
	clock_t positioningStart;
	clock_t positioningFinish;

	this->k = properties.k;

	/*
	 * Distance to reference point calculation.
	 */
	distanceCalculationStart = clock();

	for(it = tempDataset->begin(); it != end; it++){
	
		for(referencePointsIt = referencePoints->begin(); referencePointsIt!= referencePointsEnd; referencePointsIt++){
		
			(*it).distance.push_back(Point::minkowskiDistance(*referencePointsIt, (*it), 2));		
		}
	}

	/*
	 * Calculate criteria fo classification dataset.
	 */	
	for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
		for(referencePointsIt = referencePoints->begin(); referencePointsIt!= referencePointsEnd; referencePointsIt++){
				
			classificationIt->first.distance.push_back(Point::minkowskiDistance((*referencePointsIt), classificationIt->first, 2));
		}
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
	
		for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
			placementIt = Dataset::getPlacementBinary(*tempDataset, classificationIt->first);
			classificationIt->second = &(*placementIt);
			classificationDatasetEquivalent.push_back(pair<Point, vector<KNeighborhoodPoint>::iterator>(classificationIt->first, placementIt));
		}
	}
	else{
	
		for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
			placementIt = Dataset::getPlacementLineary(*tempDataset, classificationIt->first);
			classificationIt->second = &(*placementIt);
			classificationDatasetEquivalent.push_back(pair<Point, vector<KNeighborhoodPoint>::iterator>(classificationIt->first, placementIt));
		}
	}
	
	positioningFinish = clock();

	/*
	 * Clustering.
	 */
	clusteringStart = clock();
			
	classificationEquivalentEnd = classificationDatasetEquivalent.end();

	for(classificationEquivalentIt = classificationDatasetEquivalent.begin(); classificationEquivalentIt != classificationEquivalentEnd; classificationEquivalentIt++){
				
		(*classificationEquivalentIt->second).neighbors = tiKNeighborhood(*tempDataset, classificationEquivalentIt->second, classificationEquivalentIt->first, TiKNeighborhoodRef::verifyKCandidateNeighborsBackward, TiKNeighborhoodRef::verifyKCandidateNeighborsForward);
	}

	clusteringFinish = clock();

	timeReport.clusteringExecutionTime = ((double)(clusteringFinish - clusteringStart))/CLOCKS_PER_SEC;
	timeReport.distanceCalculationExecutionTime = ((double)(distanceCalculationFinish - distanceCalculationStart))/CLOCKS_PER_SEC;
	timeReport.sortingPointsExecutionTime =  ((double)(sortingFinish - sortingStart))/CLOCKS_PER_SEC;
	timeReport.algorithmExecutionTime = timeReport.clusteringExecutionTime + timeReport.distanceCalculationExecutionTime + timeReport.sortingPointsExecutionTime;
	timeReport.positioningExecutionTime = ((double)(positioningFinish - positioningStart))/CLOCKS_PER_SEC;

	return timeReport;
}

bool TiKNeighborhoodRef::indexIsCandidateNeighborByAdditionalReferencePoints(
	const KNeighborhoodPoint& point
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator queryPointIt
	, double eps){

	bool isCandidateNeighbor = true;
	unsigned long i = 1;

	while (isCandidateNeighbor && (i < point.distance.size())){
		
		if(abs((*queryPointIt)->distance[i] - point.distance[i]) > eps ){
		
			isCandidateNeighbor = false;
		}
		else{
		
			i++;
		}
	}

	return isCandidateNeighbor;
}
bool TiKNeighborhoodRef::isCandidateNeighborByAdditionalReferencePoints(
	const KNeighborhoodPoint& point
	, vector<KNeighborhoodPoint>::iterator queryPointIt
	, double eps){

	bool isCandidateNeighbor = true;
	unsigned long i = 1;

	while (isCandidateNeighbor && (i < point.distance.size())){
		
		if(abs(queryPointIt->distance[i] - point.distance[i]) > eps ){
		
			isCandidateNeighbor = false;
		}
		else{
		
			i++;
		}
	}

	return isCandidateNeighbor;
}

void TiKNeighborhoodRef::indexVerifyKCandidateNeighborsBackward (
	const vector<vector<KNeighborhoodPoint>::iterator>& dataset
	, KNeighborhoodPoint& point
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointBackwardIt
	, bool& backwardSearch
	, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
	, unsigned long k){

	double distance;
	unsigned long i;

	while(backwardSearch && ((point.distance[0] - (*pointBackwardIt)->distance[0]) <= point.eps)){

		if(indexIsCandidateNeighborByAdditionalReferencePoints(point, pointBackwardIt, point.eps)){
		
			distance = Point::minkowskiDistance((**pointBackwardIt), point, 2);

			if(distance < point.eps){
		
				i = getKeysNr(kNeighborhood, point.eps);

				if((kNeighborhood.size() - i) >= (k - 1)){
			
					kNeighborhood.erase(point.eps);
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointBackwardIt));
					point.eps = getMaxDistance(kNeighborhood);
				}
				else{
				
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointBackwardIt));
				}
			}
			else
				if(distance == point.eps){
		
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointBackwardIt));
				}
		}

		backwardSearch = indexPrecedingPoint(dataset, pointBackwardIt);
	}
}
void TiKNeighborhoodRef::verifyKCandidateNeighborsBackward (
	const vector<KNeighborhoodPoint>& dataset
	, KNeighborhoodPoint& point
	, vector<KNeighborhoodPoint>::iterator& pointBackwardIt
	, bool& backwardSearch
	, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
	, unsigned long k){

	double distance;
	unsigned long i;

	while(backwardSearch && ((point.distance[0] - pointBackwardIt->distance[0]) <= point.eps)){

		if(isCandidateNeighborByAdditionalReferencePoints(point, pointBackwardIt, point.eps)){
		
			distance = Point::minkowskiDistance((*pointBackwardIt), point, 2);

			if(distance < point.eps){
		
				i = getKeysNr(kNeighborhood, point.eps);

				if((kNeighborhood.size() - i) >= (k - 1)){
			
					kNeighborhood.erase(point.eps);
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, pointBackwardIt));
					point.eps = getMaxDistance(kNeighborhood);
				}
				else{
				
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, pointBackwardIt));
				}
			}
			else
				if(distance == point.eps){
		
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, pointBackwardIt));
				}
		}

		backwardSearch = precedingPoint(dataset, pointBackwardIt);
	}
}

void TiKNeighborhoodRef::indexVerifyKCandidateNeighborsForward (
	const vector<vector<KNeighborhoodPoint>::iterator>& dataset
	, KNeighborhoodPoint& point
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointForwardIt
	, bool& forwardSearch
	, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
	, unsigned long k){

	double distance;
	unsigned long i;

	while(forwardSearch && (((*pointForwardIt)->distance[0] - point.distance[0]) <= point.eps)){

		if(indexIsCandidateNeighborByAdditionalReferencePoints(point, pointForwardIt, point.eps)){

			distance = Point::minkowskiDistance((**pointForwardIt), point, 2);

			if(distance < point.eps){
		
				i = getKeysNr(kNeighborhood, point.eps);

				if((kNeighborhood.size() - i) >= (k - 1)){
			
					kNeighborhood.erase(point.eps);
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointForwardIt));
					point.eps = getMaxDistance(kNeighborhood);
				}
				else{
				
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointForwardIt));
				}
			}
			else
				if(distance == point.eps){
		
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointForwardIt));
				}
		}

		forwardSearch = indexFollowingPoint(dataset, pointForwardIt);
	}
}
void TiKNeighborhoodRef::verifyKCandidateNeighborsForward (
	const vector<KNeighborhoodPoint>& dataset
	, KNeighborhoodPoint& point
	, vector<KNeighborhoodPoint>::iterator& pointForwardIt
	, bool& forwardSearch
	, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
	, unsigned long k){

	double distance;
	unsigned long i;

	while(forwardSearch && ((pointForwardIt->distance[0] - point.distance[0]) <= point.eps)){

		if(isCandidateNeighborByAdditionalReferencePoints(point, pointForwardIt, point.eps)){

			distance = Point::minkowskiDistance((*pointForwardIt), point, 2);

			if(distance < point.eps){
		
				i = getKeysNr(kNeighborhood, point.eps);

				if((kNeighborhood.size() - i) >= (k - 1)){
			
					kNeighborhood.erase(point.eps);
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, pointForwardIt));
					point.eps = getMaxDistance(kNeighborhood);
				}
				else{
				
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, pointForwardIt));
				}
			}
			else
				if(distance == point.eps){
		
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, pointForwardIt));
				}
		}

		forwardSearch = followingPoint(dataset, pointForwardIt);
	}
}