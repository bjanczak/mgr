/*
 ============================================================================
 Name        : TiKNeighborhoodBase.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-24
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of TiKNeighborhoodBase.h
 ============================================================================
 */

#include "TiKNeighborhoodBase.h"

TiKNeighborhoodBase::TiKNeighborhoodBase():KNeighborhoodBase(){

	this->algorithmName = "TiKNeighborhoodBase";
}

TiKNeighborhoodBase::TiKNeighborhoodBase(const TiKNeighborhoodBase& object):KNeighborhoodBase(object){

	this->algorithmName = "TiKNeighborhoodBase";
}

multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator> TiKNeighborhoodBase::indexTiKNeighborhood(
	const vector<vector<KNeighborhoodPoint>::iterator>& dataset
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator pointIt
	, KNeighborhoodPoint& point
	, indexVerifyKCandidateNeighborsBackwardFunction backwardVerification
	, indexVerifyKCandidateNeighborsForwardFunction forwardVerification){
	
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator> kNeighborhood;
	vector<vector<KNeighborhoodPoint>::iterator>::iterator pointBackwardIt = pointIt;
	vector<vector<KNeighborhoodPoint>::iterator>::iterator pointForwardIt = pointIt;
	bool backwardSearch = true;
	bool forwardSearch = indexFollowingPoint(dataset, pointForwardIt);
	unsigned long foundNeighborsCounter = 0;

	indexFindFirstKCandidateNeighborsForwardAndBackward(dataset, point, pointBackwardIt, pointForwardIt, backwardSearch, forwardSearch, kNeighborhood, foundNeighborsCounter);
	indexFindFirstKCandidateNeighborsBackward(dataset, point, pointBackwardIt, backwardSearch, kNeighborhood, foundNeighborsCounter);
	indexFindFirstKCandidateNeighborsForward(dataset, point, pointForwardIt, forwardSearch, kNeighborhood, foundNeighborsCounter);

	point.eps = getMaxDistance(kNeighborhood);

	backwardVerification(dataset, point, pointBackwardIt, backwardSearch, kNeighborhood, this->k);
	forwardVerification(dataset, point, pointForwardIt, forwardSearch, kNeighborhood, this->k);

	// In order to print value.
	(*pointIt)->eps = point.eps;

	return kNeighborhood;
}
multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator> TiKNeighborhoodBase::tiKNeighborhood(
	const vector<KNeighborhoodPoint>& dataset
	, vector<KNeighborhoodPoint>::iterator pointIt
	, KNeighborhoodPoint& point
	, verifyKCandidateNeighborsBackwardFunction backwardVerification
	, verifyKCandidateNeighborsForwardFunction forwardVerification){
	
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator> kNeighborhood;
	vector<KNeighborhoodPoint>::iterator pointBackwardIt = pointIt;
	vector<KNeighborhoodPoint>::iterator pointForwardIt = pointIt;
	bool backwardSearch = true;
	bool forwardSearch = followingPoint(dataset, pointForwardIt);
	unsigned long foundNeighborsCounter = 0;

	findFirstKCandidateNeighborsForwardAndBackward(dataset, point, pointBackwardIt, pointForwardIt, backwardSearch, forwardSearch, kNeighborhood, foundNeighborsCounter);
	findFirstKCandidateNeighborsBackward(dataset, point, pointBackwardIt, backwardSearch, kNeighborhood, foundNeighborsCounter);
	findFirstKCandidateNeighborsForward(dataset, point, pointForwardIt, forwardSearch, kNeighborhood, foundNeighborsCounter);

	point.eps = getMaxDistance(kNeighborhood);

	backwardVerification(dataset, point, pointBackwardIt, backwardSearch, kNeighborhood, this->k);
	forwardVerification(dataset, point, pointForwardIt, forwardSearch, kNeighborhood, this->k);

	// In order to print value.
	pointIt->eps = point.eps;

	return kNeighborhood;
}

void TiKNeighborhoodBase::indexFindFirstKCandidateNeighborsForwardAndBackward(
	const vector<vector<KNeighborhoodPoint>::iterator>& dataset
	, const KNeighborhoodPoint& point
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointBackwardIt
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointForwardIt
	, bool& backwardSearch
	, bool& forwardSearch
	, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
	, unsigned long& foundNeighboursCounter){

	double distance;

	while(backwardSearch && forwardSearch && (foundNeighboursCounter < k)){
	
		if((point.distance[0] - (*pointBackwardIt)->distance[0]) < ((*pointForwardIt)->distance[0] - point.distance[0])){
		
			distance = Point::minkowskiDistance((**pointBackwardIt), point, 2);
			foundNeighboursCounter++;
			kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointBackwardIt));
			backwardSearch = indexPrecedingPoint(dataset, pointBackwardIt);
		}
		else{
			
			distance = Point::minkowskiDistance((**pointForwardIt), point, 2);
			foundNeighboursCounter++;
			kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointForwardIt));
			forwardSearch = indexFollowingPoint(dataset, pointForwardIt);
		}
	}
}
void TiKNeighborhoodBase::findFirstKCandidateNeighborsForwardAndBackward(
	const vector<KNeighborhoodPoint>& dataset
	, const KNeighborhoodPoint& point
	, vector<KNeighborhoodPoint>::iterator& pointBackwardIt
	, vector<KNeighborhoodPoint>::iterator& pointForwardIt
	, bool& backwardSearch
	, bool& forwardSearch
	, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
	, unsigned long& foundNeighboursCounter){

	double distance;

	while(backwardSearch && forwardSearch && (foundNeighboursCounter < k)){
	
		if((point.distance[0] - pointBackwardIt->distance[0]) < (pointForwardIt->distance[0] - point.distance[0])){
		
			distance = Point::minkowskiDistance((*pointBackwardIt), point, 2);
			foundNeighboursCounter++;
			kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, pointBackwardIt));
			backwardSearch = precedingPoint(dataset, pointBackwardIt);
		}
		else{
			
			distance = Point::minkowskiDistance((*pointForwardIt), point, 2);
			foundNeighboursCounter++;
			kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, pointForwardIt));
			forwardSearch = followingPoint(dataset, pointForwardIt);
		}
	}
}

void TiKNeighborhoodBase::indexFindFirstKCandidateNeighborsBackward(
	const vector<vector<KNeighborhoodPoint>::iterator>& dataset
	, const KNeighborhoodPoint& point
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointBackwardIt
	, bool &backwardSearch
	, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
	, unsigned long& foundNeighboursCounter){

	double distance;

	while(backwardSearch && (foundNeighboursCounter < k)){
		
		distance = Point::minkowskiDistance((**pointBackwardIt), point, 2);
		foundNeighboursCounter++;
		kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointBackwardIt));
		backwardSearch = indexPrecedingPoint(dataset, pointBackwardIt);		
	}
}
void TiKNeighborhoodBase::findFirstKCandidateNeighborsBackward(
	const vector<KNeighborhoodPoint>& dataset
	, const KNeighborhoodPoint& point
	, vector<KNeighborhoodPoint>::iterator& pointBackwardIt
	, bool &backwardSearch
	, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
	, unsigned long& foundNeighboursCounter){

	double distance;

	while(backwardSearch && (foundNeighboursCounter < k)){
		
		distance = Point::minkowskiDistance((*pointBackwardIt), point, 2);
		foundNeighboursCounter++;
		kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, pointBackwardIt));
		backwardSearch = precedingPoint(dataset, pointBackwardIt);		
	}
}

void TiKNeighborhoodBase::indexFindFirstKCandidateNeighborsForward(
	const vector<vector<KNeighborhoodPoint>::iterator>& dataset
	, const KNeighborhoodPoint& point
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointForwardIt
	, bool &forwardSearch
	, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
	, unsigned long& foundNeighboursCounter){
	
	double distance;

	while(forwardSearch && (foundNeighboursCounter < k)){
			
		distance = Point::minkowskiDistance((**pointForwardIt), point, 2);
		foundNeighboursCounter++;
		kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, *pointForwardIt));
		forwardSearch = indexFollowingPoint(dataset, pointForwardIt);
	}
}
void TiKNeighborhoodBase::findFirstKCandidateNeighborsForward(
	const vector<KNeighborhoodPoint>& dataset
	, const KNeighborhoodPoint& point
	, vector<KNeighborhoodPoint>::iterator& pointForwardIt
	, bool &forwardSearch
	, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood
	, unsigned long& foundNeighboursCounter){
	
	double distance;

	while(forwardSearch && (foundNeighboursCounter < k)){
			
		distance = Point::minkowskiDistance((*pointForwardIt), point, 2);
		foundNeighboursCounter++;
		kNeighborhood.insert(pair<double, vector<KNeighborhoodPoint>::iterator>(distance, pointForwardIt));
		forwardSearch = followingPoint(dataset, pointForwardIt);
	}
}

bool TiKNeighborhoodBase::indexPrecedingPoint(
	const vector<vector<KNeighborhoodPoint>::iterator>& dataset
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointIt){

	if(pointIt == dataset.begin()){
			
		return false;
	}
	else{
			
		pointIt--;
		return true;
	}
}
bool TiKNeighborhoodBase::precedingPoint(
	const vector<KNeighborhoodPoint>& dataset
	, vector<KNeighborhoodPoint>::iterator& pointIt){

	if(pointIt == dataset.begin()){
			
		return false;
	}
	else{
			
		pointIt--;
		return true;
	}
}

bool TiKNeighborhoodBase::indexFollowingPoint(
	const vector<vector<KNeighborhoodPoint>::iterator>& dataset
	, vector<vector<KNeighborhoodPoint>::iterator>::iterator& pointIt){
	
	vector<vector<KNeighborhoodPoint>::iterator>::iterator tempPointIt = pointIt;

	if(pointIt == dataset.end()){
		
		return false;
	}
	else{

		tempPointIt++;

		if(tempPointIt == dataset.end()){
		
			return false;
		}
		else{

			pointIt++;
			return true;
		}
	}
}
bool TiKNeighborhoodBase::followingPoint(
	const vector<KNeighborhoodPoint>& dataset
	, vector<KNeighborhoodPoint>::iterator& pointIt){
	
	vector<KNeighborhoodPoint>::iterator tempPointIt = pointIt;

	if(pointIt == dataset.end()){
		
		return false;
	}
	else{

		tempPointIt++;

		if(tempPointIt == dataset.end()){
		
			return false;
		}
		else{

			pointIt++;
			return true;
		}
	}
}

double TiKNeighborhoodBase::getMaxDistance(const multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood){
	
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>::const_iterator end = kNeighborhood.end();
	end--;
	return (*end).first;
}

unsigned long TiKNeighborhoodBase::getKeysNr(const multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>& kNeighborhood, const double key){

	unsigned long result = 0;
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>::const_iterator it;
	pair<multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>::const_iterator, multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>::const_iterator> ret = kNeighborhood.equal_range(key);

	for(it = ret.first; it != ret.second; it++){
		
		result++;
	}

	return result;
}