/*
 ============================================================================
 Name        : VpsTree.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-10-01
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of VpsTree.h
 ============================================================================
 */

#include "VpsTree.h"
#include "Utils.h"

#include <set>
#include <time.h>

VpsTree::VpsTree(){

	this->algorithmName = "VpsTree";
}

VpsTree::VpsTree(const VpsTree& object){

	this->algorithmName = "VpsTree";
}

TimeReport VpsTree::run(const Properties& properties, Dataset& dataset){

	TimeReport timeReport;
	list<VpsPoint> list;
	clock_t clusteringStart;
	clock_t clusteringFinish;

	this->pSampleLimit = properties.pSampleIndex;
	this->dSampleLimit = properties.sSampleIndex;

	Dataset::fillVpsPointList(list, dataset.datasetPoint, dataset.isDense);

	clusteringStart = clock();

	dataset.vpsTree = recurseVpsTree(list);

	clusteringFinish = clock();

	timeReport.clusteringExecutionTime = ((double)(clusteringFinish - clusteringStart))/CLOCKS_PER_SEC;

	return timeReport;
}

void VpsTree::search(Point* query, double& eps, VpsPoint* point, list<Point*>& neighbors, unsigned long& neighborsSize, unsigned long k){

	//TODO
}

VpsPoint* VpsTree::recurseVpsTree(list<VpsPoint>& candidates){

	VpsPoint* result = NULL;

	//TODO

	return result;
}
	
void VpsTree::buildBoundariesList(VpsPoint& point){

	//TODO
}