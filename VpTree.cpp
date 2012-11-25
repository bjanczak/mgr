/*
 ============================================================================
 Name        : VpTree.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-10-01
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of VpTree.h
 ============================================================================
 */

#include "VpTree.h"
#include "Utils.h"

#include <set>
#include <time.h>

VpTree::VpTree(){

	this->algorithmName = "VpTree";
}

VpTree::VpTree(const VpTree& object){

	this->algorithmName = "VpTree";
}

TimeReport VpTree::run(const Properties& properties, Dataset& dataset){

	TimeReport timeReport;
	list<VpsPoint> tempDataset;
	list<VpsPoint>::iterator it;
	list<VpsPoint>::iterator end;
	list<list<VpsPoint>::iterator> pointList;
	list<Point*> kNeighbors;
	vector<pair<Point*, list<Point*>>>* classificationResult = &dataset.classificationResult;
	vector<pair<Point, Point*>>* classificationDataset = &dataset.classificationDataset;
	unsigned long classificationDatasetSize = classificationDataset->size();
	vector<pair<Point, Point*>>::iterator classificationIt = classificationDataset->begin();
	vector<pair<Point, Point*>>::iterator classificationEnd = classificationDataset->end();
	vector<multimap<double, Point*, DistanceComparator>> tempKNeighbors(classificationDataset->size());
	multimap<double, Point*, DistanceComparator>::iterator multimapIt;
	multimap<double, Point*, DistanceComparator>::iterator multimapEnd;
	unsigned long k = properties.k;
	unsigned long counter;
	double eps;
	Point p;
	clock_t clusteringStart;
	clock_t clusteringFinish;
	clock_t indexBuildingStart;
	clock_t indexBuildingFinish;

	this->pSampleLimit = properties.pSampleIndex;
	this->dSampleLimit = properties.sSampleIndex;

	Dataset::fillVpsPointList(tempDataset, dataset.datasetPoint, dataset.isDense);

	/*
	 * Fill tempKNeighbors if k neighborhood method search.
	 */
	if(properties.searchMethod == Properties::K_NEIGHBORHOOD){
	
		for(unsigned long i=0; i < classificationDatasetSize; i++){
	
			for(unsigned long j=0; j < k; j++){
			
				tempKNeighbors[i].insert(pair<double, Point*>(DBL_MAX, &p));
			}
		}
	}

	/*
	 * Build list of iterators on tempDataset.
	 */
	it = tempDataset.begin();
	end = tempDataset.end();

	for(; it != end; it++){
	
		pointList.push_back(it);
	}

	indexBuildingStart = clock();

	dataset.vpsTree = makeVpTree(pointList, tempDataset);

	indexBuildingFinish = clock();

	clusteringStart = clock();

	if(properties.searchMethod == Properties::RANGE){
		
		counter = 0;
		
		for(; classificationIt != classificationEnd; classificationIt++){
		
			eps = properties.eps;

			rangeSearch(&classificationIt->first, eps, dataset.vpsTree, tempKNeighbors[counter]);
			counter++;
		}
	}
	else
		if(properties.searchMethod == Properties::K_NEIGHBORHOOD){
			
			counter = 0;

			for(; classificationIt != classificationEnd; classificationIt++){
		
				kNeighborhoodSearch(&classificationIt->first, dataset.vpsTree, tempKNeighbors[counter]);
				counter++;
			}
		}

	clusteringFinish = clock();

	/*
	 * Rewrite Map to list and place in results.
	 */
	counter = 0;
	classificationIt = classificationDataset->begin();
	
	while(classificationIt != classificationEnd){
		
		multimapIt = tempKNeighbors[counter].begin();
		multimapEnd = tempKNeighbors[counter].end();
		kNeighbors.clear();

		while(multimapIt != multimapEnd){
			
			kNeighbors.push_back(multimapIt->second);
			multimapIt++;
		}
		
		classificationResult->push_back(pair<Point*, list<Point*>>(&(classificationIt->first), kNeighbors));
		counter++;
		classificationIt++;
	}

	timeReport.indexBuildingExecutionTime = ((double)(indexBuildingFinish - indexBuildingStart))/CLOCKS_PER_SEC;
	timeReport.clusteringExecutionTime = ((double)(clusteringFinish - clusteringStart))/CLOCKS_PER_SEC;
	timeReport.algorithmExecutionTime = timeReport.indexBuildingExecutionTime + timeReport.clusteringExecutionTime;

	return timeReport;
}

void VpTree::kNeighborhoodSearch(Point* query, VpsPoint* point, multimap<double, Point*, DistanceComparator>& neighbors){

	if(point == NULL){
	
		return;
	}
	multimap<double, Point*, DistanceComparator>::iterator it = neighbors.end();
	it--;
	double tau = it->first;
	double distance = Point::minkowskiDistance(*query, *point, 2);
	double leftBoundary = distance - tau;
	double rightBoundary = distance + tau;
	double leftBuffer;
	double rightBuffer;

	if(distance <= tau){
	
		neighbors.erase(it);
		neighbors.insert(pair<double, Point*>(distance, point));
	}

	if(leftBoundary <= point->leftBoundHigh){
	
		if(rightBoundary >= point->rightBoundLow){
		
			leftBuffer = point->leftBoundHigh - leftBoundary;
			rightBuffer = rightBoundary - point->rightBoundLow;

			if(leftBuffer < rightBuffer){
				
				kNeighborhoodSearch(query, point->left, neighbors);
				it = neighbors.end();
				it--;
				tau = it->first;
				kNeighborhoodSearch(query, point->right, neighbors);
			}
			else{
				
				kNeighborhoodSearch(query, point->right, neighbors);
				it = neighbors.end();
				it--;
				tau = it->first;
				kNeighborhoodSearch(query, point->left, neighbors);
			}
		}
		else{
		
			kNeighborhoodSearch(query, point->left, neighbors);
		}
	}
	else{
	
		if(rightBoundary >= point->rightBoundLow){
			
			kNeighborhoodSearch(query, point->right, neighbors);
		}
	}
}

void VpTree::rangeSearch( Point* query, double& tau, VpsPoint* point, multimap<double, Point*, DistanceComparator>& neighbors){
	
	if(point == NULL){
	
		return;
	}

	double distance = Point::minkowskiDistance(*query, *point, 2);
	double leftBoundary = distance - tau;
	double rightBoundary = distance + tau;
	double leftBuffer;
	double rightBuffer;

	if(distance <= tau){
	
		neighbors.insert(pair<double, Point*>(distance, point));
	}

	if(leftBoundary <= point->leftBoundHigh){
	
		if(rightBoundary >= point->rightBoundLow){
		
			leftBuffer = point->leftBoundHigh - leftBoundary;
			rightBuffer = rightBoundary - point->rightBoundLow;

			if(leftBuffer < rightBuffer){
				
				rangeSearch(query, tau, point->left, neighbors);
				rangeSearch(query, tau, point->right, neighbors);
			}
			else{
				
				rangeSearch(query, tau, point->right, neighbors);
				rangeSearch(query, tau, point->left, neighbors);
			}
		}
		else{
		
			rangeSearch(query, tau, point->left, neighbors);
		}
	}
	else{
	
		if(rightBoundary >= point->rightBoundLow){
			
			rangeSearch(query, tau, point->right, neighbors);
		}
	}
}

VpsPoint* VpTree::makeVpTree(list<list<VpsPoint>::iterator>& candidates, list<VpsPoint>& dataset){

	VpsPoint* result = NULL;
	
	switch(candidates.size()){
	
		case 0:
		
			result = NULL;
			break;
	
		case 1:
		
			result = new VpsPoint(*(*(candidates.begin())));
			result->median = 0;
			result->leftBoundHigh = 0;
			result->rightBoundLow = 0;
			result->left = NULL;
			result->right = NULL;
			break;
	
		default:

			list<list<VpsPoint>::iterator> leftCandidates;
			list<list<VpsPoint>::iterator> rightCandidates;
			list<list<VpsPoint>::iterator>::iterator it;
			list<list<VpsPoint>::iterator>::iterator itToErase;
			list<list<VpsPoint>::iterator>::iterator end;
			list<VpsPoint>::iterator vantagePointIt;
			vector<double> distances;
			unsigned long counter = 0;
			double median;
			vector<double> medianWithNeighbors;

			vantagePointIt = selectVp(candidates);
			end = candidates.end();

			/*
			 * Calculate distances to vantage point.
			 */
			for(it = candidates.begin(); it != end; it++){
	
				if(*it != vantagePointIt){
		
					distances.push_back(Point::minkowskiDistance(*(*it), *vantagePointIt, 2));
				}
				else{
			
					itToErase = it;
				}
			}

			/*
			 * Delete choosen vantage point from candidates.
			 */
			candidates.erase(itToErase);

			/*
			 * Calculate median and it's neighbors.
			 */
			medianWithNeighbors = Utils::medianWithNeighbors(distances);

			median = medianWithNeighbors[0];

			end = candidates.end();

			/*
			 * Divide ponits in two groups.
			 */
			for(it = candidates.begin(); it != end;){
	
				if(distances[counter] < median){
		
					leftCandidates.push_back(*it);
				}
				else{

					rightCandidates.push_back(*it);
				}

				counter++;
				it++;

				candidates.pop_front();
			}

			result = new VpsPoint(*vantagePointIt);
	
			dataset.erase(vantagePointIt);

			result->median = medianWithNeighbors[0];
			result->leftBoundHigh = medianWithNeighbors[1];
			result->rightBoundLow = medianWithNeighbors[2];
			result->left = makeVpTree(leftCandidates, dataset);
			result->right = makeVpTree(rightCandidates, dataset);
	}

	return result;
}