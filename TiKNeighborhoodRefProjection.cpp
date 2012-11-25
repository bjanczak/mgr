/*
 ============================================================================
 Name        : TiKNeighborhoodRefProjection.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-25
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of TiKNeighborhoodRefProjection.h
 ============================================================================
 */

#include "TiKNeighborhoodRefProjection.h"

#include <algorithm>
#include <ctime>

TiKNeighborhoodRefProjection::TiKNeighborhoodRefProjection():TiKNeighborhoodRef(){

	this->algorithmName = "TiKNeighborhoodRefProjection";
}

TiKNeighborhoodRefProjection::TiKNeighborhoodRefProjection(const TiKNeighborhoodRefProjection& object):TiKNeighborhoodRef(object){

	this->algorithmName = "TiKNeighborhoodRefProjection";
}

TimeReport TiKNeighborhoodRefProjection::run(const Properties& properties, Dataset& dataset){

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
	
	const vector<pair<char, unsigned long>> *projectionSourceSequence = &properties.projectionSourceSequence;
	vector<pair<char, unsigned long>>::const_iterator projectionSourceSequenceIt;
	vector<pair<char, unsigned long>>::const_iterator projectionSourceSequenceEnd = projectionSourceSequence->end();
	
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
	 * Distance to reference points or dimensions value calculation.
	 * Distance is either distance to one of reference points or
	 * vaule of dimension given as projection dimension.
	 */
	distanceCalculationStart = clock();

	for(it = tempDataset->begin(); it != end; it++){
	
		for(projectionSourceSequenceIt = projectionSourceSequence->begin(); projectionSourceSequenceIt != projectionSourceSequenceEnd; projectionSourceSequenceIt++){
		
			if((*projectionSourceSequenceIt).first == 'r'){
			
				(*it).distance.push_back(Point::minkowskiDistance(referencePoints->at((*projectionSourceSequenceIt).second - 1), (*it), 2));
			}
			else
				if((*projectionSourceSequenceIt).first == 'd'){

					(*it).distance.push_back((*it).getDimensionValue((*projectionSourceSequenceIt).second));
				}
		}
	}

	/*
	 * If CLASSIFICATION, then calculate criteria fo classification dataset.
	 */
	if(properties.algorithmType == Properties::CLASSIFICATION){
	
		for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
			for(projectionSourceSequenceIt = projectionSourceSequence->begin(); projectionSourceSequenceIt != projectionSourceSequenceEnd; projectionSourceSequenceIt++){
		
				if((*projectionSourceSequenceIt).first == 'r'){

					classificationIt->first.distance.push_back(Point::minkowskiDistance(referencePoints->at((*projectionSourceSequenceIt).second - 1), classificationIt->first, 2));
				}
				else
					if((*projectionSourceSequenceIt).first == 'd'){

					classificationIt->first.distance.push_back(classificationIt->first.getDimensionValue((*projectionSourceSequenceIt).second));
					}
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
		
				placementIt = Dataset::getPlacementBinary(datasetIterators,classificationIt->first);
				classificationIt->second = &(**placementIt);
				classificationDatasetEquivalent.push_back(placementIt);
			}
		}
		else{
	
			for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
				placementIt = Dataset::getPlacementLineary(datasetIterators,classificationIt->first);
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