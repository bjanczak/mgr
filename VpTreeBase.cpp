/*
 ============================================================================
 Name        : VpTreeBase.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-10-02
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of VpTreeBase.h
 ============================================================================
 */

#include "VpTreeBase.h"
#include "Utils.h"

#include <algorithm>
#include <set>
#include <time.h>

VpTreeBase::VpTreeBase(){

	this->algorithmName = "VpTreeBase";
}

VpTreeBase::VpTreeBase(const VpTreeBase& object){

	this->algorithmName = "VpTreeBase";
}

TimeReport VpTreeBase::run(const Properties& properties, Dataset& dataset){

	TimeReport timeReport;
	this->pSampleLimit = properties.pSampleIndex;
	this->dSampleLimit = properties.sSampleIndex;

	return timeReport;
}

void VpTreeBase::search(Point* query, double& eps, VpsPoint* point, list<Point*>& neighbors, unsigned long& neighborsSize, unsigned long k){

}

vector<list<list<VpsPoint>::iterator>::iterator> VpTreeBase::randomSample(list<list<VpsPoint>::iterator>& candidates, unsigned long sampleLimit){
	
	srand(time(NULL));
	
	unsigned long counter = 0;
	unsigned long size = candidates.size();
	unsigned long factor = sampleLimit;
	set<unsigned long> index;
	set<unsigned long>::iterator indexIt;
	set<unsigned long>::iterator indexEnd;
	vector<list<list<VpsPoint>::iterator>::iterator> result;
	list<list<VpsPoint>::iterator>::iterator it = candidates.begin();
	list<list<VpsPoint>::iterator>::iterator end = candidates.end();

	factor = factor < 10 ? 10 : factor;

	for(unsigned long i = 0; i < factor; i++){
	
		index.insert(rand() % size);
	}

	indexIt = index.begin();
	indexEnd = index.end();

	for(; it != end; it++){
		
		if(counter == *indexIt){
		
			result.push_back(it);

			indexIt++;

			if(indexIt == indexEnd){
			
				break;
			}
		}

		counter++;
	}
	
	return result;
}

list<VpsPoint>::iterator VpTreeBase::selectVp(list<list<VpsPoint>::iterator>& candidates){
	 
	vector<list<list<VpsPoint>::iterator>::iterator> randomSampleP = randomSample(candidates, this->pSampleLimit);
	vector<list<list<VpsPoint>::iterator>::iterator> randomSampleD;
	list<VpsPoint>::iterator bestVantagePointIt = *candidates.begin();
	vector<list<list<VpsPoint>::iterator>::iterator>::iterator itP = randomSampleP.begin();
	vector<list<list<VpsPoint>::iterator>::iterator>::iterator endP = randomSampleP.end();
	vector<list<list<VpsPoint>::iterator>::iterator>::iterator itD;
	vector<list<list<VpsPoint>::iterator>::iterator>::iterator endD;
	vector<double> distances;
	unsigned long randomSampleDSize;
	double median;
	double spread;
	double bestSpread = 0;

	if(candidates.size() == 1){
	
		bestVantagePointIt = *candidates.begin();
	}
	else{

		for(; itP != endP; itP++){
	
			randomSampleD = randomSample(candidates, this->dSampleLimit);
			randomSampleDSize = randomSampleD.size();
			itD = randomSampleD.begin();
			endD = randomSampleD.end();
			distances.clear();
			distances.reserve(randomSampleDSize);

			for(; itD != endD; itD++){

				distances.push_back(Point::minkowskiDistance(*(*(*itP)), *(*(*itD)), 2));
			}

			sort(distances.begin(), distances.end());

			median = Utils::median(distances);
			spread = Utils::variance(distances, median);

			if(spread > bestSpread){
		
				bestSpread = spread;
				bestVantagePointIt = *(*itP);
			}
		}
	}
		
	return bestVantagePointIt;
}