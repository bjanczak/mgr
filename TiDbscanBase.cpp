/*
 ============================================================================
 Name        : TiDbscanBase.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-21
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of TiDbscanBase.h
 ============================================================================
 */

#include "TiDbscanBase.h"

TiDbscanBase::TiDbscanBase():DbscanBase(){

	this->algorithmName = "TiDbscanBase";
}

TiDbscanBase::TiDbscanBase(const TiDbscanBase& object):DbscanBase(object){

	this->algorithmName = "TiDbscanBase";
}

bool TiDbscanBase::indexExpandCluster(list<vector<DbscanPoint>::iterator>& dataset, list<vector<DbscanPoint>::iterator>::iterator& pointIt, unsigned int clusterId, indexNeighborhoodFunction *neighborhood){

	list<list<vector<DbscanPoint>::iterator>::iterator>::iterator currentPointIt;
	list<list<vector<DbscanPoint>::iterator>::iterator> seeds = neighborhood(dataset, pointIt, eps);
	list<list<vector<DbscanPoint>::iterator>::iterator> currentSeeds;
	list<list<vector<DbscanPoint>::iterator>::iterator>::iterator currentSeedsIt;
	list<list<vector<DbscanPoint>::iterator>::iterator>::iterator currentSeedsEnd;
	list<list<vector<DbscanPoint>::iterator>::iterator>::iterator seedsIt;
	list<list<vector<DbscanPoint>::iterator>::iterator>::iterator seedsEnd = seeds.end();
	list<DbscanPoint*>::iterator borderIt;
	list<DbscanPoint*>::iterator borderEnd;

	/*
	 * Including point itself.
	 */
	(*pointIt)->neighborsNr = (*pointIt)->neighborsNr + seeds.size();

	/*
	 * Point is either noise or a border point.
	 */
	if((*pointIt)->neighborsNr < minPts){

		(*pointIt)->clusterId = DbscanPoint::NOISE;

		for(seedsIt = seeds.begin(); seedsIt != seedsEnd; seedsIt++){
		
			(*(*seedsIt))->borderPoints.push_back(&(**pointIt));
			(*(*seedsIt))->neighborsNr = (*(*seedsIt))->neighborsNr + 1;
		}

		(*pointIt)->borderPoints.clear();
		(*pointIt)->isAnalyzed = true;
		dataset.erase(pointIt);	

		return false;
	}
	else{
		
		(*pointIt)->clusterId = clusterId;

		for(seedsIt = seeds.begin(); seedsIt != seedsEnd; seedsIt++){
		
			(*(*seedsIt))->clusterId = clusterId;
			(*(*seedsIt))->neighborsNr = (*(*seedsIt))->neighborsNr + 1;
		}

		borderEnd = (*pointIt)->borderPoints.end();

		for(borderIt = (*pointIt)->borderPoints.begin(); borderIt != borderEnd; borderIt++){

			if((*borderIt)->isAnalyzed){
			
				(*borderIt)->clusterId = clusterId;
			}
		}

		(*pointIt)->borderPoints.clear();
		(*pointIt)->isAnalyzed = true;
		dataset.erase(pointIt);

		while(seeds.size() > 0){

			currentPointIt = seeds.begin();
			currentSeeds = neighborhood(dataset, *currentPointIt, eps);
			(*(*currentPointIt))->neighborsNr = (*(*currentPointIt))->neighborsNr + currentSeeds.size();

			if((*(*currentPointIt))->neighborsNr < minPts){				

				currentSeedsEnd = currentSeeds.end();

				for(currentSeedsIt = currentSeeds.begin(); currentSeedsIt != currentSeedsEnd; currentSeedsIt++){

					(*(*currentSeedsIt))->neighborsNr = (*(*currentSeedsIt))->neighborsNr + 1;
				}
			}
			else{

				while(currentSeeds.size()>0){
					
					currentSeedsIt = currentSeeds.begin();			

					(*(*currentSeedsIt))->neighborsNr = (*(*currentSeedsIt))->neighborsNr + 1;

					if((*(*currentSeedsIt))->clusterId == DbscanPoint::UNCLASSIFIED){

						(*(*currentSeedsIt))->clusterId = clusterId;
						seeds.push_back(*currentSeedsIt);
					}
					
					currentSeeds.erase(currentSeedsIt);
				}

				borderEnd = (*(*currentPointIt))->borderPoints.end();
				
				for(borderIt = (*(*currentPointIt))->borderPoints.begin(); borderIt != borderEnd; borderIt++){

					if((*borderIt)->isAnalyzed){
			
						(*borderIt)->clusterId = clusterId;
					}
				}
			}

			(*(*currentPointIt))->borderPoints.clear();
			(*(*currentPointIt))->isAnalyzed = true;
			
			dataset.erase(*currentPointIt);
			seeds.erase(currentPointIt);
		}

		return true;
	}
}