/*
 ============================================================================
 Name        : Point.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-04
 Version     : 2.0
 Copyright   : Your copyright notice
 Description : Implementation of Point.h
 ============================================================================
 */
#include "Points.h"
#include "Utils.h"

#include <ctime>
#include <math.h>
#include <set>

SparsePoint::SparsePoint(){

	this->dimension = 0;
	this->value = 0;
}

SparsePoint::SparsePoint(const SparsePoint& object){

	this->dimension = object.dimension;
	this->value = object.value;
}

SparsePoint::SparsePoint(unsigned long dimension, double value){

	this->value = value;
	this->dimension = dimension;
}

bool SparsePoint::comparator(SparsePoint& a, SparsePoint& b){

	return (a.dimension < b.dimension);
}

Point::~Point(){

	this->distance.clear();
	this->denseFormatPoint.clear();
	this->sparseFormatPoint.clear();
}

Point::Point(){
	
	this->id = 0;
	this->distance;
	this->isDense = true;
	this->denseFormatPoint;
	this->sparseFormatPoint;
}

_inline Point::Point(const Point& object){

	this->id = object.id;
	this->distance = vector<double>(object.distance);
	this->isDense = object.isDense;
	
	if(this->isDense){

		this->denseFormatPoint = vector<double>(object.denseFormatPoint);
	}
	else{
		this->sparseFormatPoint = vector<SparsePoint>(object.sparseFormatPoint);
	}
}

Point::Point(const unsigned long id, const vector<double>& denseFormatPoint){

	this->id = id;
	this->isDense = true;
	this->denseFormatPoint = vector<double>(denseFormatPoint);
}

Point::Point(const unsigned long id, const vector<SparsePoint>& sparseFormatPoint){
	
	this->id = id;
	this->isDense = false;
	this->sparseFormatPoint = vector<SparsePoint>(sparseFormatPoint);	
		
}

double Point::getDimensionValue(const unsigned long dimension){

	double dimensionValue;

	if(this->isDense){
	
		dimensionValue =  this->denseFormatPoint[dimension-1];
	}
	else{

		vector<SparsePoint>::iterator it;
		vector<SparsePoint>::iterator end = this->sparseFormatPoint.end();
		double tempDimensionValue = 0;

		for(it = this->sparseFormatPoint.begin(); it != end; it++){

			if((*it).dimension == dimension){
			
				dimensionValue = (*it).value;
				break;
			}
		}

		dimensionValue = tempDimensionValue;
	}

	return dimensionValue;
}

float Point::invSqrt2(float x){

	union{
	
		int intPart;
		float floatPart;
	} convertor;

	convertor.floatPart = x;
	convertor.intPart = 0x5f3759df - (convertor.intPart >> 1);
	return 0.5f*convertor.floatPart*(3.0f - x*convertor.floatPart*convertor.floatPart);
}

double Point::minkowskiDistance(const Point& a, const Point& b, unsigned int orderP){

	/*
	 * Error!
	 */
	if(a.isDense!=b.isDense){
			
		return DBL_MIN;
	}

	double sum = 0;

	/*
	 * Euclidean distance.
	 */
	if(orderP==2){

		if(a.isDense){

			unsigned long dimension = a.denseFormatPoint.size();
			const vector<double> *denseA = &(a.denseFormatPoint);
			const vector<double> *denseB = &(b.denseFormatPoint);


			for(unsigned long i = 0; i<dimension; i++){
			
				sum = sum + (denseA->at(i)-denseB->at(i))*(denseA->at(i)-denseB->at(i));
			}
		}
		else{
		
			const vector<SparsePoint> *sparseA = &(a.sparseFormatPoint);
			const vector<SparsePoint> *sparseB = &(b.sparseFormatPoint);
			unsigned long sparseASize = sparseA->size();
			unsigned long sparseBSize = sparseB->size();
			unsigned long sparseAIndex;
			unsigned long sparseBIndex;
			SparsePoint sparsePointA;
			SparsePoint sparsePointB;

			if(sparseASize == 0){
			
				sparseAIndex = UINT_MAX;
				sparsePointA = SparsePoint(UINT_MAX, 0);
			}
			else{
			
				sparseAIndex = 0;
				sparsePointA = sparseA->at(sparseAIndex);
			}

			if(sparseBSize == 0){
			
				sparseBIndex = UINT_MAX;
				sparsePointB = SparsePoint(UINT_MAX, 0);
			}
			else{
			
				sparseBIndex = 0;
				sparsePointB = sparseB->at(sparseBIndex);
			}

			while(sparseAIndex != UINT_MAX || sparseBIndex != UINT_MAX){
			
				if(sparsePointA.dimension > sparsePointB.dimension){
					
					sum = sum + sparsePointB.value *sparsePointB.value;
					sparseBIndex++;
				
					if(sparseBIndex==sparseBSize){
				
						sparseBIndex = UINT_MAX;
						sparsePointB.dimension = UINT_MAX;
					}
					else{
				
						sparsePointB = sparseB->at(sparseBIndex);
					}
				}
				else
					if(sparsePointA.dimension < sparsePointB.dimension){

						sum = sum + sparsePointA.value *sparsePointA.value;
						sparseAIndex++;
				
						if(sparseAIndex==sparseASize){
				
							sparseAIndex = UINT_MAX;
							sparsePointA.dimension = UINT_MAX;
						}
						else{
				
							sparsePointA = sparseA->at(sparseAIndex);
						}
					}
					else
						if(sparsePointA.dimension == sparsePointB.dimension){
						
							sum = sum + (sparsePointA.value-sparsePointB.value)*(sparsePointA.value-sparsePointB.value);
							sparseAIndex++;
							sparseBIndex++;

							if(sparseAIndex==sparseASize){
				
								sparseAIndex = UINT_MAX;
								sparsePointA.dimension = UINT_MAX;
							}
							else{
				
								sparsePointA = sparseA->at(sparseAIndex);
							}

							if(sparseBIndex==sparseBSize){
				
								sparseBIndex = UINT_MAX;
								sparsePointB.dimension = UINT_MAX;
							}
							else{
				
								sparsePointB = sparseB->at(sparseBIndex);
							}
						}
			}
		}
	}

	//return 1/(double)Point::invSqrt2(sum);
	return sqrt(sum);
}

bool Point::distanceComparator(const Point& a, const Point& b){
	
	unsigned long size;
	double distanceA;
	double distanceB;

	if(a.distance.size() == b.distance.size()){

		size = a.distance.size();

		for(unsigned long i = 0; i< size; i++){
		
			distanceA = a.distance[i];
			distanceB = b.distance[i];

			if(distanceA == distanceB){
			
				if(i == size-1){
				
					return distanceA < distanceB;
				}
			}
			else{
			
				return distanceA < distanceB;
			}
		}
	}
	else{
	
		return false;
	}
}

bool Point::distanceComparatorPointer( const Point* a, const Point* b){
	
	unsigned long size;
	double distanceA;
	double distanceB;

	if(a->distance.size() == b->distance.size()){

		size = a->distance.size();

		for(unsigned long i = 0; i< size; i++){
		
			distanceA = a->distance[i];
			distanceB = b->distance[i];

			if(distanceA == distanceB){
			
				if(i == size-1){
				
					return distanceA < distanceB;
				}
			}
			else{
			
				return distanceA < distanceB;
			}
		}
	}
	else{
	
		return false;
	}
}

const unsigned long DbscanPoint::NOISE = 0;

const unsigned long DbscanPoint::UNCLASSIFIED = ULONG_MAX;

DbscanPoint::~DbscanPoint(){

	this->borderPoints.clear();
}

DbscanPoint::DbscanPoint():Point(){
	
	this->isAnalyzed = false;
	this->neighborsNr = 0;
	this->clusterId = DbscanPoint::UNCLASSIFIED;
	this->borderPoints;
}

DbscanPoint::DbscanPoint(const DbscanPoint& object):Point(object){
	
	this->isAnalyzed = object.isAnalyzed;
	this->neighborsNr = object.neighborsNr;
	this->clusterId = object.clusterId;
	this->borderPoints = list<DbscanPoint*>(object.borderPoints);
}

DbscanPoint::DbscanPoint(const Point& object):Point(object){
	
	this->isAnalyzed = false;
	this->neighborsNr = 0;
	this->clusterId = DbscanPoint::UNCLASSIFIED;
	this->borderPoints;
}

bool DbscanPoint::clusterIdComparator(DbscanPoint& a, DbscanPoint& b){
	
	return (a.clusterId < b.clusterId);
}

bool DbscanPoint::distanceComparatorIterator(const vector<DbscanPoint>::iterator a, const vector<DbscanPoint>::iterator b){
	
	unsigned long size;
	double distanceA;
	double distanceB;

	if(a->distance.size() == b->distance.size()){

		size = a->distance.size();

		for(unsigned long i = 0; i< size; i++){
		
			distanceA = a->distance[i];
			distanceB = b->distance[i];

			if(distanceA == distanceB){
			
				if(i == size-1){
				
					return distanceA < distanceB;
				}
			}
			else{
			
				return distanceA < distanceB;
			}
		}
	}
	else{
	
		return false;
	}
}

KNeighborhoodPoint::~KNeighborhoodPoint(){

	this->neighbors.clear();
}

KNeighborhoodPoint::KNeighborhoodPoint():Point(){
	
	this->eps = 0;
	this->neighbors;
	this->realDistanceCalculations = 0;
}

KNeighborhoodPoint::KNeighborhoodPoint(const KNeighborhoodPoint& object):Point(object){
	
	this->eps = object.eps;
	this->neighbors = object.neighbors;
	this->realDistanceCalculations = object.realDistanceCalculations;
}

KNeighborhoodPoint::KNeighborhoodPoint(const Point& object):Point(object){
	
	this->eps = 0;
	this->neighbors;
	this->realDistanceCalculations = 0;
}
bool KNeighborhoodPoint::distanceComparatorIterator(const vector<KNeighborhoodPoint>::iterator a, const vector<KNeighborhoodPoint>::iterator b){

	unsigned long size;
	double distanceA;
	double distanceB;

	if(a->distance.size() == b->distance.size()){

		size = a->distance.size();

		for(unsigned long i = 0; i< size; i++){
		
			distanceA = a->distance[i];
			distanceB = b->distance[i];

			if(distanceA == distanceB){
			
				if(i == size-1){
				
					return distanceA < distanceB;
				}
			}
			else{
			
				return distanceA < distanceB;
			}
		}
	}
	else{
	
		return false;
	}
}

History::~History(){

}

History::History(){

	this->ancestor = NULL;
	this->distance = 0;
}

History::History(const History& object){

	this->ancestor = object.ancestor;
	this->distance = object.distance;
}

Boundaries::~Boundaries(){
	
}

Boundaries::Boundaries(){
	
	this->ancestor = NULL;
	this->lower = 0;
	this->higher = 0;
}

Boundaries::Boundaries(const Boundaries& object){
	
	this->ancestor = object.ancestor;
	this->lower = object.lower;
	this->higher = object.higher;
}

VpsPoint::~VpsPoint(){

	this->history.clear();
	this->boundaries.clear();
}

VpsPoint::VpsPoint():Point(){

	this->median = 0;
	this->leftBoundHigh = 0;
	this->rightBoundLow = 0;
	this->history;
	this->boundaries;
	this->left = NULL;
	this->right = NULL;
}

VpsPoint::VpsPoint(const VpsPoint& object):Point(object){

	this->median = object.median;
	this->leftBoundHigh = object.leftBoundHigh;
	this->rightBoundLow = object.rightBoundLow;
	this->history = vector<History>(object.history);
	this->boundaries = vector<Boundaries>(object.boundaries);
	this->left = object.left;
	this->right = object.right;
}

VpsPoint::VpsPoint(const unsigned long id, const vector<double>& denseFormatPoint):Point(id, denseFormatPoint){
	
	this->median = 0;
	this->leftBoundHigh = 0;
	this->rightBoundLow = 0;
	this->history;
	this->boundaries;
	this->left = NULL;
	this->right = NULL;
}

VpsPoint::VpsPoint(
		const unsigned long id
		, const vector<SparsePoint>& sparseFormatPoint):Point(id, sparseFormatPoint){
	
	this->median = 0;
	this->leftBoundHigh = 0;
	this->rightBoundLow = 0;
	this->history;
	this->boundaries;
	this->left = NULL;
	this->right = NULL;
}

void VpsPoint::destroy(){

	this->distance.clear();
	this->denseFormatPoint.clear();
	this->sparseFormatPoint.clear();
	this->history.clear();
	this->boundaries.clear();
	
	if(this->left != NULL){
		
		this->left->destroy();
	}

	if(this->right != NULL){
	
		this->right->destroy();
	}

	this->~VpsPoint();
}

vector<History>::iterator VpsPoint::getHistoryTail(){

	vector<History>::iterator tail = this->history.end();

	if(this->history.size() > 0){

		tail--;
	}

	return tail;
}
