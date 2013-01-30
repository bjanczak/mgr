/*
 ============================================================================
 Name        : TiDbscanRefProjection.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-23
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of TiDbscanRefProjection.h
 ============================================================================
 */

#include "TiDbscanRefProjection.h"

#include <algorithm>
#include <ctime>

TiDbscanRefProjection::TiDbscanRefProjection():TiDbscanRef(){

	this->algorithmName = "TiDbscanRefProjection";
}

TiDbscanRefProjection::TiDbscanRefProjection(const TiDbscanRefProjection& object):TiDbscanRef(object){

	this->algorithmName = "TiDbscanRefProjection";
}

TimeReport TiDbscanRefProjection::run(const Properties& properties, Dataset& dataset){

	TimeReport timeReport;
	
	const vector<Point> *referencePoints = &properties.referencePoints;
	const vector<unsigned long> *projectionDimensions = &properties.projectionDimensions;
	const vector<pair<char, unsigned long>> *projectionSourceSequence = &properties.projectionSourceSequence;
	
	vector<pair<char, unsigned long>>::const_iterator projectionSourceSequenceIt;
	vector<pair<char, unsigned long>>::const_iterator projectionSourceSequenceEnd = projectionSourceSequence->end();
	
	vector<DbscanPoint> *tempDataset = &dataset.datasetDbscanPoint;
	vector<DbscanPoint>::iterator it;
	vector<DbscanPoint>::iterator end = tempDataset->end();
	
	list<vector<DbscanPoint>::iterator> datasetIterators;
	list<vector<DbscanPoint>::iterator>::iterator datasetIteratorsIt;
	
	unsigned long clusterId = 1;
	clock_t distanceCalculationStart;
	clock_t distanceCalculationFinish;
	clock_t sortingStart;
	clock_t sortingFinish;
	clock_t clusteringStart;
	clock_t clusteringFinish;

	this->eps = properties.eps;
	this->minPts = properties.minPts;

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

					(*it).distance.push_back((*it).getDimensionValue(properties.projectionDimensions.at((*projectionSourceSequenceIt).second -1 )));
				}
		}

		(*it).neighborsNr = 1;
	}

	distanceCalculationFinish = clock();

	if(properties.isUseDatasetIndexAcess){
		
		/*
		* Build working index.
		*/
		for(it = tempDataset->begin(); it != end; it++){
	
			datasetIterators.push_back(it);
		}
		
		/*
		* Sorting points by distance to reference point.
		*/
		sortingStart = clock();

		datasetIterators.sort(DbscanPoint::distanceComparatorIterator);

		sortingFinish = clock();
	}
	else{
	
		/*
		* Sorting points by distance to reference point.
		*/
		sortingStart = clock();

		sort(tempDataset->begin(), tempDataset->end(), DbscanPoint::distanceComparator);

		sortingFinish = clock();

		/*
		* Build working index.
		*/
		for(it = tempDataset->begin(); it != end; it++){
	
			datasetIterators.push_back(it);
		}
	}

	/*
	 * Clustering.
	 */
	clusteringStart = clock();

	while(datasetIterators.size() > 0){

		datasetIteratorsIt = datasetIterators.begin();

		if(indexExpandCluster(datasetIterators, datasetIteratorsIt, clusterId, TiDbscanRef::indexTiNeighborhood)){
		
			clusterId++;
		}
	}

	clusteringFinish = clock();

	timeReport.clusteringExecutionTime = ((double)(clusteringFinish - clusteringStart))/CLOCKS_PER_SEC;
	timeReport.distanceCalculationExecutionTime = ((double)(distanceCalculationFinish - distanceCalculationStart))/CLOCKS_PER_SEC;
	timeReport.sortingPointsExecutionTime =  ((double)(sortingFinish - sortingStart))/CLOCKS_PER_SEC;
	timeReport.algorithmExecutionTime = timeReport.clusteringExecutionTime + timeReport.distanceCalculationExecutionTime + timeReport.sortingPointsExecutionTime;

	return timeReport;
}