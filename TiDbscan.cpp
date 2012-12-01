/*
 ============================================================================
 Name        : TiDbscan.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-21
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of TiDbscan.h
 ============================================================================
 */

#include "TiDbscan.h"

#include <algorithm>
#include <ctime>

TiDbscan::TiDbscan():TiDbscanBase(){

	this->algorithmName = "TiDbscan";
}

TiDbscan::TiDbscan(const TiDbscan& object):TiDbscanBase(object){

	this->algorithmName = "TiDbscan";
}

TimeReport TiDbscan::run(const Properties& properties, Dataset& dataset){

	TimeReport timeReport;
	Point referencePoint = properties.referencePoints.front();
	
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
	 * Distance to reference point calculation.
	 */
	distanceCalculationStart = clock();

	for(it = tempDataset->begin(); it != end; it++){
	
		(*it).distance.push_back(Point::minkowskiDistance(referencePoint, (*it), 2));
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

		if(indexExpandCluster(datasetIterators, datasetIteratorsIt, clusterId, TiDbscan::indexTiNeighborhood)){
		
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

list<list<vector<DbscanPoint>::iterator>::iterator> TiDbscan::indexTiNeighborhood(list<vector<DbscanPoint>::iterator>& setOfPoints, list<vector<DbscanPoint>::iterator>::iterator pointIt, const double eps){

	list<list<vector<DbscanPoint>::iterator>::iterator> forwardNeighborhood = TiDbscan::indexTiForwardNeighborhood(setOfPoints, pointIt, eps);
	list<list<vector<DbscanPoint>::iterator>::iterator> backwardNeighborhood = TiDbscan::indexTiBackwardNeighborhood(setOfPoints, pointIt, eps);
	list<list<vector<DbscanPoint>::iterator>::iterator> result;
	list<list<vector<DbscanPoint>::iterator>::iterator>::iterator it;
	list<list<vector<DbscanPoint>::iterator>::iterator>::iterator end;
	list<vector<DbscanPoint>::iterator>::iterator datasetEnd = setOfPoints.end();
	list<vector<DbscanPoint>::iterator>::iterator datasetIt;	

	end = forwardNeighborhood.end();

	for(it = forwardNeighborhood.begin(); it != end; it++){

		result.push_back(*it);
	}

	end = backwardNeighborhood.end();

	for(it = backwardNeighborhood.begin(); it != end; it++){

		result.push_back(*it);
	}

	return result;
}

list<list<vector<DbscanPoint>::iterator>::iterator> TiDbscan::indexTiBackwardNeighborhood(list<vector<DbscanPoint>::iterator>& setOfPoints, list<vector<DbscanPoint>::iterator>::iterator pointIt, const double eps){
	
	list<list<vector<DbscanPoint>::iterator>::iterator> result;

	if(pointIt == setOfPoints.begin()){

		return result;
	}
	else{

		double backwardTreshold = (*pointIt)->distance[0] - eps;
		list<vector<DbscanPoint>::iterator>::iterator it = pointIt;
		list<vector<DbscanPoint>::iterator>::iterator begin = setOfPoints.begin();
		it--;

		while(true){

			if((*it)->distance[0] < backwardTreshold){

				break;
			}

			if(Point::minkowskiDistance(*(*it), *(*pointIt), 2) <= eps){
		
				result.push_back(it);
			}

			if(it == begin){

				break;
			}

			it--;
		}

		return result;
	}
}

list<list<vector<DbscanPoint>::iterator>::iterator> TiDbscan::indexTiForwardNeighborhood(list<vector<DbscanPoint>::iterator>& setOfPoints, list<vector<DbscanPoint>::iterator>::iterator pointIt, const double eps){
	
	list<list<vector<DbscanPoint>::iterator>::iterator> result;
	
	if(pointIt == setOfPoints.end()){

		return result;
	}
	else{

		double forwardTreshold = (*pointIt)->distance[0] + eps;
		list<vector<DbscanPoint>::iterator>::iterator it = pointIt;
		it++;

		if(it == setOfPoints.end()){

			return result;
		}
		else{
			
			list<vector<DbscanPoint>::iterator>::iterator end = setOfPoints.end();
			
			for(it; it != end; it++){
	
				if((*it)->distance[0] > forwardTreshold){

					break;
				}

				if(Point::minkowskiDistance(*(*it), *(*pointIt), 2) <= eps){
		
					result.push_back(it);
				}
			}
	
			return result;
		}
	}
}