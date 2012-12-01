/*
 ============================================================================
 Name        : DbscanPointsElimination.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-11-30
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of DbscanPointsElimination.h
 ============================================================================
 */

#include "DbscanPointsElimination.h"

#include <algorithm>
#include <ctime>

DbscanPointsElimination::DbscanPointsElimination():TiDbscanBase(){

	this->algorithmName = "DbscanPointsElimination";
}

DbscanPointsElimination::DbscanPointsElimination(const DbscanPointsElimination& object):TiDbscanBase(object){

	this->algorithmName = "DbscanPointsElimination";
}

TimeReport DbscanPointsElimination::run(const Properties& properties, Dataset& dataset){

	TimeReport timeReport;
	
	vector<DbscanPoint> *tempDataset = &dataset.datasetDbscanPoint;
	vector<DbscanPoint>::iterator it;
	vector<DbscanPoint>::iterator end = tempDataset->end();
	
	list<vector<DbscanPoint>::iterator> datasetIterators;
	list<vector<DbscanPoint>::iterator>::iterator datasetIteratorsIt;
	
	unsigned long clusterId = 1;
	clock_t clusteringStart;
	clock_t clusteringFinish;

	this->eps = properties.eps;
	this->minPts = properties.minPts;

	/*
	 * Build working index.
	 */
	for(it = tempDataset->begin(); it != end; it++){
	
		datasetIterators.push_back(it);
	}	

	/*
	 * Clustering.
	 */
	clusteringStart = clock();

	while(datasetIterators.size() > 0){

		datasetIteratorsIt = datasetIterators.begin();

		if(indexExpandCluster(datasetIterators, datasetIteratorsIt, clusterId, DbscanPointsElimination::indexNeighborhood)){
		
			clusterId++;
		}
	}

	clusteringFinish = clock();

	timeReport.clusteringExecutionTime = ((double)(clusteringFinish - clusteringStart))/CLOCKS_PER_SEC;
	timeReport.algorithmExecutionTime = timeReport.clusteringExecutionTime + timeReport.distanceCalculationExecutionTime + timeReport.sortingPointsExecutionTime;

	return timeReport;
}

list<list<vector<DbscanPoint>::iterator>::iterator> DbscanPointsElimination::indexNeighborhood(list<vector<DbscanPoint>::iterator>& setOfPoints, list<vector<DbscanPoint>::iterator>::iterator pointIt, const double eps){

	list<list<vector<DbscanPoint>::iterator>::iterator> forwardNeighborhood = DbscanPointsElimination::indexForwardNeighborhood(setOfPoints, pointIt, eps);
	list<list<vector<DbscanPoint>::iterator>::iterator> backwardNeighborhood = DbscanPointsElimination::indexBackwardNeighborhood(setOfPoints, pointIt, eps);
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

list<list<vector<DbscanPoint>::iterator>::iterator> DbscanPointsElimination::indexBackwardNeighborhood(list<vector<DbscanPoint>::iterator>& setOfPoints, list<vector<DbscanPoint>::iterator>::iterator pointIt, const double eps){
	
	list<list<vector<DbscanPoint>::iterator>::iterator> result;

	if(pointIt == setOfPoints.begin()){

		return result;
	}
	else{

		list<vector<DbscanPoint>::iterator>::iterator it = pointIt;
		list<vector<DbscanPoint>::iterator>::iterator begin = setOfPoints.begin();
		it--;

		while(true){

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

list<list<vector<DbscanPoint>::iterator>::iterator> DbscanPointsElimination::indexForwardNeighborhood(list<vector<DbscanPoint>::iterator>& setOfPoints, list<vector<DbscanPoint>::iterator>::iterator pointIt, const double eps){
	
	list<list<vector<DbscanPoint>::iterator>::iterator> result;
	
	if(pointIt == setOfPoints.end()){

		return result;
	}
	else{

		list<vector<DbscanPoint>::iterator>::iterator it = pointIt;
		it++;

		if(it == setOfPoints.end()){

			return result;
		}
		else{
			
			list<vector<DbscanPoint>::iterator>::iterator end = setOfPoints.end();
			
			for(it; it != end; it++){

				if(Point::minkowskiDistance(*(*it), *(*pointIt), 2) <= eps){
		
					result.push_back(it);
				}
			}
	
			return result;
		}
	}
}