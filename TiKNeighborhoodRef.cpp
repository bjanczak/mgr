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
	vector<KNeighborhoodPoint> *tempDataset = &dataset.datasetKNeighborhoodPoint;
	
	vector<KNeighborhoodPoint>::iterator it;
	vector<KNeighborhoodPoint>::iterator end = tempDataset->end();
	vector<vector<KNeighborhoodPoint>::iterator>::iterator placementIt;
	
	vector<vector<KNeighborhoodPoint>::iterator> datasetIterators;
	vector<vector<KNeighborhoodPoint>::iterator>::iterator datasetIteratorsIt;
	vector<vector<KNeighborhoodPoint>::iterator>::iterator datasetIteratorsEnd;

	vector<pair<Point, Point*>> *classificationDataset = &dataset.classificationDataset;
	vector<pair<Point, Point*>>::iterator classificationIt;
	vector<pair<Point, Point*>>::iterator classificationEnd = classificationDataset->end();
	
	vector<vector<vector<KNeighborhoodPoint>::iterator>::iterator> classificationDatasetEquivalent;
	vector<vector<vector<KNeighborhoodPoint>::iterator>::iterator>::iterator classificationEquivalentIt;
	vector<vector<vector<KNeighborhoodPoint>::iterator>::iterator>::iterator classificationEquivalentEnd;
	
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
	 * Build working index.
	 */
	for(it = tempDataset->begin(); it != end; it++){
	
		datasetIterators.push_back(it);
	}

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
	 * If CLASSIFICATION, then calculate criteria fo classification dataset.
	 */
	if(properties.algorithmType == Properties::CLASSIFICATION){
	
		for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
			for(referencePointsIt = referencePoints->begin(); referencePointsIt!= referencePointsEnd; referencePointsIt++){
				
				classificationIt->first.distance.push_back(Point::minkowskiDistance((*referencePointsIt), classificationIt->first, 2));
			}
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
	if(properties.algorithmType == Properties::CLASSIFICATION){
	
		positioningStart = clock();

		if(properties.useBinaryPlacement){
	
			for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
				placementIt = Dataset::getPlacementBinary(datasetIterators, classificationIt->first);
				classificationIt->second = &(**placementIt);
				classificationDatasetEquivalent.push_back(placementIt);
			}
		}
		else{
	
			for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
				placementIt = Dataset::getPlacementLineary(datasetIterators, classificationIt->first);
				classificationIt->second = &(**placementIt);
				classificationDatasetEquivalent.push_back(placementIt);
			}
		}
	
		positioningFinish = clock();
	}

	/*
	 * Clustering.
	 */
	clusteringStart = clock();

	if(properties.algorithmType == Properties::GROUPING){
	
		for(datasetIteratorsIt = datasetIterators.begin(); datasetIteratorsIt != datasetIteratorsEnd; datasetIteratorsIt++){
	
			(*datasetIteratorsIt)->neighbors = tiKNeighborhood(datasetIterators, datasetIteratorsIt, TiKNeighborhoodRef::verifyKCandidateNeighborsBackward, TiKNeighborhoodRef::verifyKCandidateNeighborsForward);
		}
	}
	else
		if(properties.algorithmType == Properties::CLASSIFICATION){
			
			classificationEquivalentEnd = classificationDatasetEquivalent.end();

			for(classificationEquivalentIt = classificationDatasetEquivalent.begin(); classificationEquivalentIt != classificationEquivalentEnd; classificationEquivalentIt++){
				
				(**classificationEquivalentIt)->neighbors = tiKNeighborhood(datasetIterators, (*classificationEquivalentIt), TiKNeighborhoodRef::verifyKCandidateNeighborsBackward, TiKNeighborhoodRef::verifyKCandidateNeighborsForward);
			}
		}

	clusteringFinish = clock();

	timeReport.clusteringExecutionTime = ((double)(clusteringFinish - clusteringStart))/CLOCKS_PER_SEC;
	timeReport.distanceCalculationExecutionTime = ((double)(distanceCalculationFinish - distanceCalculationStart))/CLOCKS_PER_SEC;
	timeReport.sortingPointsExecutionTime =  ((double)(sortingFinish - sortingStart))/CLOCKS_PER_SEC;
	timeReport.algorithmExecutionTime = timeReport.clusteringExecutionTime + timeReport.distanceCalculationExecutionTime + timeReport.sortingPointsExecutionTime;

	if(properties.algorithmType == Properties::CLASSIFICATION){
	
		timeReport.positioningExecutionTime = ((double)(positioningFinish - positioningStart))/CLOCKS_PER_SEC;
	}

	return timeReport;
}

bool TiKNeighborhoodRef::isCandidateNeighborByAdditionalReferencePoints(
	const vector<vector<KNeighborhoodPoint>::iterator>::iterator pointIt
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator queryPointIt
	, double eps){

	bool isCandidateNeighbor = true;
	unsigned long i = 1;

	while (isCandidateNeighbor && (i < (*pointIt)->distance.size())){
		
		if(abs((*queryPointIt)->distance[i] - (*pointIt)->distance[i]) > eps ){
		
			isCandidateNeighbor = false;
		}
		else{
		
			i++;
		}
	}

	return isCandidateNeighbor;
}

void TiKNeighborhoodRef::verifyKCandidateNeighborsBackward (
	const vector<vector<KNeighborhoodPoint>::iterator>& dataset
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator pointIt
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointBackwardIt
	, bool& backwardSearch
	, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
	, unsigned long k){

	double distance;
	unsigned long i;

	while(backwardSearch && (((*pointIt)->distance[0] - (*pointBackwardIt)->distance[0]) <= (*pointIt)->eps)){

		if(isCandidateNeighborByAdditionalReferencePoints(pointIt, pointBackwardIt, (*pointIt)->eps)){
		
			distance = Point::minkowskiDistance((**pointBackwardIt), (**pointIt), 2);

			if(distance < (*pointIt)->eps){
		
				i = getKeysNr(kNeighborhood, (*pointIt)->eps);

				if((kNeighborhood.size() - i) >= (k - 1)){
			
					kNeighborhood.erase((*pointIt)->eps);
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointBackwardIt));
					(*pointIt)->eps = getMaxDistance(kNeighborhood);
				}
				else{
				
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointBackwardIt));
				}
			}
			else
				if(distance == (*pointIt)->eps){
		
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointBackwardIt));
				}
		}

		backwardSearch = precedingPoint(dataset, pointBackwardIt);
	}
}

void TiKNeighborhoodRef::verifyKCandidateNeighborsForward (
	const vector<vector<KNeighborhoodPoint>::iterator>& dataset
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator pointIt
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointForwardIt
	, bool& forwardSearch
	, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
	, unsigned long k){

	double distance;
	unsigned long i;

	while(forwardSearch && (((*pointForwardIt)->distance[0] - (*pointIt)->distance[0]) <= (*pointIt)->eps)){

		if(isCandidateNeighborByAdditionalReferencePoints(pointIt, pointForwardIt, (*pointIt)->eps)){

			distance = Point::minkowskiDistance((**pointForwardIt), (**pointIt), 2);

			if(distance < (*pointIt)->eps){
		
				i = getKeysNr(kNeighborhood, (*pointIt)->eps);

				if((kNeighborhood.size() - i) >= (k - 1)){
			
					kNeighborhood.erase((*pointIt)->eps);
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointForwardIt));
					(*pointIt)->eps = getMaxDistance(kNeighborhood);
				}
				else{
				
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointForwardIt));
				}
			}
			else
				if(distance == (*pointIt)->eps){
		
					kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointForwardIt));
				}
		}

		forwardSearch = followingPoint(dataset, pointForwardIt);
	}
}