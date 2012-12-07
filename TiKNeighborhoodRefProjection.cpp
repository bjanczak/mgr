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

	if(properties.isUseDatasetIndexAcess){
	
		timeReport = runDatasetIndexAccess(properties, dataset);
	}
	else{
	
		timeReport = runDatasetDirectAccess(properties, dataset);
	}

	return timeReport;
}

TimeReport TiKNeighborhoodRefProjection::runDatasetIndexAccess(const Properties& properties, Dataset& dataset){

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
		
			indexPlacementIt = Dataset::indexGetPlacementBinary(datasetIterators,classificationIt->first);
			classificationIt->second = &(**indexPlacementIt);
			classificationDatasetIndexEquivalent.push_back(pair<Point, vector<vector<KNeighborhoodPoint>::iterator>::iterator>(classificationIt->first, indexPlacementIt));
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

TimeReport TiKNeighborhoodRefProjection::runDatasetDirectAccess(const Properties& properties, Dataset& dataset){
		
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
		
			placementIt = Dataset::getPlacementBinary(*tempDataset,classificationIt->first);
			classificationIt->second = &(*placementIt);
			classificationDatasetEquivalent.push_back(pair<Point, vector<KNeighborhoodPoint>::iterator>(classificationIt->first, placementIt));
		}
	}
	else{
	
		for(classificationIt = classificationDataset->begin(); classificationIt != classificationEnd; classificationIt++){
		
			placementIt = Dataset::getPlacementLineary(*tempDataset,classificationIt->first);
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