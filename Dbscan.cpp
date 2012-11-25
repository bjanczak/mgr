/*
 ============================================================================
 Name        : Dbscan.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-20
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of Dbscan.h
 ============================================================================
 */

#include "Dbscan.h"

#include <time.h>

Dbscan::Dbscan():DbscanBase(){

	this->algorithmName = "Dbscan";
}

Dbscan::Dbscan(const Dbscan& object):DbscanBase(object){

	this->algorithmName = "Dbscan";
}

TimeReport Dbscan::run(const Properties& properties, Dataset& dataset){

	TimeReport timeReport;
	vector<DbscanPoint> *setOfPoints = &dataset.datasetDbscanPoint;
	vector<DbscanPoint>::iterator it;
	vector<DbscanPoint>::iterator end = setOfPoints->end();
	unsigned long clusterId = 1;
	clock_t clusteringStart, clusteringFinish;
	this->eps = properties.eps;
	this->minPts = properties.minPts;

	clusteringStart = clock();

	for(it = setOfPoints->begin(); it != end; it++){
	
		if((*it).clusterId == DbscanPoint::UNCLASSIFIED){		
			
			if(expandCluster(*setOfPoints, *it, clusterId)){
					
				clusterId++;
			}
		}
	}

	clusteringFinish = clock();

	timeReport.algorithmExecutionTime = ((double)(clusteringFinish - clusteringStart))/CLOCKS_PER_SEC;
	timeReport.clusteringExecutionTime = timeReport.algorithmExecutionTime;

	return timeReport;
}

bool Dbscan::expandCluster(vector<DbscanPoint>& setOfPoints, DbscanPoint& point, const unsigned int clusterId){

	list<DbscanPoint*>::iterator currentPointIt;
	DbscanPoint* resultPoint;
	list<DbscanPoint*> result;
	list<DbscanPoint*> seeds = regionQuery(setOfPoints, point);
	list<DbscanPoint*>::iterator it;
	list<DbscanPoint*>::iterator end = seeds.end();	

	/*
	 * No core point.
	 */
	if(seeds.size() < minPts){
	
		point.clusterId = DbscanPoint::NOISE;
		
		return false;
	}
	/*
	 * All points in seeds are directly reachable from point.
	 */
	else{
	

		for(it = seeds.begin(); it != end; it++){
		
			(*it)->clusterId = clusterId;
		}

		seeds.remove(&point);

		while(!seeds.empty()){
		
			currentPointIt = seeds.begin();
			result = regionQuery(setOfPoints, *(*currentPointIt));

			if(result.size() >= minPts){

				end = result.end();

				for( it = result.begin(); it != end; it++){

					resultPoint = (*it);

					if(resultPoint->clusterId == DbscanPoint::NOISE || resultPoint->clusterId == DbscanPoint::UNCLASSIFIED){
					
						if(resultPoint->clusterId == DbscanPoint::UNCLASSIFIED){

							seeds.push_back(resultPoint);
						}

						resultPoint->clusterId = clusterId;
					}
				}
			}
			
			seeds.erase(currentPointIt);
		}

		return true;
	}
}

list<DbscanPoint*> Dbscan::regionQuery(vector<DbscanPoint>& setOfPoints, const DbscanPoint& point){

	list<DbscanPoint*> result;
	vector<DbscanPoint>::iterator it;
	vector<DbscanPoint>::iterator end = setOfPoints.end();

	for(it = setOfPoints.begin(); it != end; it++){
	
		if(Point::minkowskiDistance(point, (*it), 2) <= eps){
		
			result.push_back(&(*it));
		}
	}

	return result;
}