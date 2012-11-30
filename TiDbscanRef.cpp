/*
 ============================================================================
 Name        : TiDbscanRef.cpp
 Author      : Bart�omiej Ja�czak
 Date        : 2012-09-21
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of TiDbscanRef.h
 ============================================================================
 */

#include "TiDbscanRef.h"

#include <algorithm>
#include <cmath>
#include <ctime>

TiDbscanRef::TiDbscanRef():TiDbscanBase(){

	this->algorithmName = "TiDbscanRef";
}

TiDbscanRef::TiDbscanRef(const TiDbscanRef& object):TiDbscanBase(object){

	this->algorithmName = "TiDbscanRef";
}

TimeReport TiDbscanRef::run(const Properties& properties, Dataset& dataset){

	TimeReport timeReport;

	if(properties.isUseDatasetIndexAcess){
	
		timeReport = runDatasetIndexAccess(properties, dataset);
	}
	else{
	
		timeReport = runDatasetDirectAccess(properties, dataset);
	}

	return timeReport;
}

TimeReport TiDbscanRef::runDatasetIndexAccess(const Properties& properties, Dataset& dataset){

	TimeReport timeReport;
	
	const vector<Point> *referencePoints = &properties.referencePoints;
	
	vector<Point>::const_iterator referencePointsIt;
	vector<Point>::const_iterator referencePointsEnd = referencePoints->end();
	
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
	 * Build working index.
	 */
	for(it = tempDataset->begin(); it != end; it++){
	
		datasetIterators.push_back(it);
	}

	/*
	 * Distance to reference points calculation.
	 */
	distanceCalculationStart = clock();

	for(it = tempDataset->begin(); it != end; it++){
	
		for(referencePointsIt = referencePoints->begin(); referencePointsIt!= referencePointsEnd; referencePointsIt++){
		
			(*it).distance.push_back(Point::minkowskiDistance(*referencePointsIt, (*it), 2));		
		}

		(*it).neighborsNr = 1;
	}

	distanceCalculationFinish = clock();

	/*
	 * Sorting points by distance to reference point.
	 */
	sortingStart = clock();

	datasetIterators.sort(DbscanPoint::distanceComparatorIterator);

	sortingFinish = clock();

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
TimeReport TiDbscanRef::runDatasetDirectAccess(const Properties& properties, Dataset& dataset){

	TimeReport timeReport;
	
	const vector<Point> *referencePoints = &properties.referencePoints;
	
	vector<Point>::const_iterator referencePointsIt;
	vector<Point>::const_iterator referencePointsEnd = referencePoints->end();
	
	list<DbscanPoint> tempDataset =  TiDbscanBase::vectorToList(dataset.datasetDbscanPoint);
	list<DbscanPoint>::iterator it;
	list<DbscanPoint>::iterator end = tempDataset.end();
	
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
	 * Distance to reference points calculation.
	 */
	distanceCalculationStart = clock();

	for(it = tempDataset.begin(); it != end; it++){
	
		for(referencePointsIt = referencePoints->begin(); referencePointsIt!= referencePointsEnd; referencePointsIt++){
		
			it->distance.push_back(Point::minkowskiDistance(*referencePointsIt, (*it), 2));		
		}

		it->neighborsNr = 1;
	}

	distanceCalculationFinish = clock();

	/*
	 * Sorting points by distance to reference point.
	 */
	sortingStart = clock();

	tempDataset.sort(DbscanPoint::distanceComparator);

	sortingFinish = clock();

	/*
	 * Clustering.
	 */
	clusteringStart = clock();

	while(datasetIterators.size() > 0){

		it = tempDataset.begin();

		if(expandCluster(tempDataset, datasetIteratorsIt, clusterId, TiDbscanRef::indexTiNeighborhood)){
		
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

list<list<vector<DbscanPoint>::iterator>::iterator> TiDbscanRef::indexTiNeighborhood(list<vector<DbscanPoint>::iterator>& setOfPoints, list<vector<DbscanPoint>::iterator>::iterator pointIt, const double eps){

	list<list<vector<DbscanPoint>::iterator>::iterator> forwardNeighborhood = TiDbscanRef::indexTiForwardNeighborhood(setOfPoints, pointIt, eps);
	list<list<vector<DbscanPoint>::iterator>::iterator> backwardNeighborhood = TiDbscanRef::indexTiBackwardNeighborhood(setOfPoints, pointIt, eps);
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
list<list<DbscanPoint>::iterator> TiDbscanRef::tiNeighborhood(list<DbscanPoint>& setOfPoints, list<DbscanPoint>::iterator pointIt, const double eps){

	list<list<DbscanPoint>::iterator> forwardNeighborhood = TiDbscanRef::tiForwardNeighborhood(setOfPoints, pointIt, eps);
	list<list<DbscanPoint>::iterator> backwardNeighborhood = TiDbscanRef::tiBackwardNeighborhood(setOfPoints, pointIt, eps);
	list<list<DbscanPoint>::iterator> result;
	list<list<DbscanPoint>::iterator>::iterator it;
	list<list<DbscanPoint>::iterator>::iterator end;
	list<DbscanPoint>::iterator datasetEnd = setOfPoints.end();
	list<DbscanPoint>::iterator datasetIt;

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

list<list<vector<DbscanPoint>::iterator>::iterator> TiDbscanRef::indexTiBackwardNeighborhood(list<vector<DbscanPoint>::iterator>& setOfPoints, list<vector<DbscanPoint>::iterator>::iterator pointIt, const double eps){
	
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

			bool candidateNeighbour = indexIsCandidateNeighborByAdditionalReferencePoints(pointIt, it, eps);

			if(candidateNeighbour && (Point::minkowskiDistance(*(*it), *(*pointIt), 2) <= eps)){
		
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

list<list<DbscanPoint>::iterator> TiDbscanRef::tiForwardNeighborhood(list<DbscanPoint>& setOfPoints, list<DbscanPoint>::iterator pointIt, const double eps){
	
	list<list<DbscanPoint>::iterator> result;
	
	if(pointIt == setOfPoints.end()){

		return result;
	}
	else{

		double forwardTreshold = pointIt->distance[0] + eps;
		list<DbscanPoint>::iterator it = pointIt;
		it++;

		if(it == setOfPoints.end()){

			return result;
		}
		else{

			
			list<DbscanPoint>::iterator end = setOfPoints.end();
			
			for(it; it != end; it++){
	
				if(it->distance[0] > forwardTreshold){

					break;
				}

				bool candidateNeighbour = isCandidateNeighborByAdditionalReferencePoints(pointIt, it, eps);

				if(candidateNeighbour && (Point::minkowskiDistance(*it, *pointIt, 2) <= eps)){
		
					result.push_back(it);
				}
			}
	
			return result;
		}
	}
}

bool TiDbscanRef::indexIsCandidateNeighborByAdditionalReferencePoints(
	list<vector<DbscanPoint>::iterator>::iterator pointIt
	, list<vector<DbscanPoint>::iterator>::iterator queryPointIt
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
bool TiDbscanRef::isCandidateNeighborByAdditionalReferencePoints(
	list<DbscanPoint>::iterator pointIt
	, list<DbscanPoint>::iterator queryPointIt
	, double eps){

	bool isCandidateNeighbor = true;
	unsigned long i = 1;

	while (isCandidateNeighbor && (i < pointIt->distance.size())){
		
		if(abs(queryPointIt->distance[i] - pointIt->distance[i]) > eps ){
		
			isCandidateNeighbor = false;
		}
		else{
		
			i++;
		}
	}

	return isCandidateNeighbor;
}