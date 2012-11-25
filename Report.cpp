/*
 ============================================================================
 Name        : Report.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-10-14
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of Report.h
 ============================================================================
 */

#include "Report.h"

const string Report::columnSeparator = "; ";

Report::~Report(){

	this->referencePoints.clear();
	this->projectionDimensions.clear();
	this->projectionSourceSequence.clear();
}

Report::Report(){
	
	this->reportFileName				= "";
	this->propertiesFileName				= "";
	this->algorithmType						= "";
	this->algorithmGroup					= "";
	this->algorithmGroupId					= 0;
	this->algorithmName						= "";
	this->algorithmNameId					= 0;
	this->eps								= DBL_MIN;
	this->minEps							= DBL_MIN;
	this->avgEps							= DBL_MIN;
	this->maxEps							= DBL_MIN;
	this->minPts							= 0;
	this->k									= 0;
	this->useCosineSimilarity				= false;
	this->isDatasetFileFormatDense			= false;
	this->datasetFilePath					= "";
	this->datasetDimension					= 0;
	this->datasetDimensionValueTreshold		= 0;
	this->datasetElementsNumber				= 0;
	this->isDatasetInternalFormatDense		= false;
	this->referencePointsString				= "";
	this->referencePoints.clear();
	this->isReferencePointFormatDense		= false;
	this->projectionDimensions.clear();
	this->projectionSourceSequence.clear();
	this->classificationSubsetFactor		= 0;
	this->useBinaryPlacement				= false;
	this->pSampleIndex						= 0;
	this->sSampleIndex						= 0;
	this->constantVantagePointString		= "";
	this->projectionDimensionsString		= "";
	this->projectionSortingCriteriaString	= "";
	this->algorithmExecutionTime			= DBL_MIN;
	this->clusteringExecutionTime			= DBL_MIN;
	this->distanceCalculationExecutionTime	= DBL_MIN;
	this->normalizingDatasetExecutionTime	= DBL_MIN;
	this->positioningExecutionTime			= DBL_MIN;
	this->sortingPointsExecutionTime		= DBL_MIN;
	this->indexBuildingExecutionTime		= DBL_MIN;
	this->datafileReadingTime				= DBL_MIN;
	this->calculatingReferencePointsTime	= DBL_MIN;
}

Report::Report(const Report& object){
	
	this->reportFileName					= object.reportFileName;
	this->propertiesFileName				= object.propertiesFileName;
	this->algorithmType						= object.algorithmType;
	this->algorithmGroup					= object.algorithmGroup;
	this->algorithmGroupId					= object.algorithmGroupId;
	this->algorithmName						= object.algorithmName;
	this->algorithmNameId					= object.algorithmNameId;
	this->eps								= object.eps;
	this->minEps							= object.minEps;
	this->avgEps							= object.avgEps;
	this->maxEps							= object.maxEps;
	this->minPts							= object.minPts;
	this->k									= object.k;
	this->useCosineSimilarity				= object.useCosineSimilarity;
	this->isDatasetFileFormatDense			= object.isDatasetFileFormatDense;
	this->datasetFilePath					= object.datasetFilePath;
	this->datasetDimension					= object.datasetDimension;
	this->datasetDimensionValueTreshold		= object.datasetDimensionValueTreshold;
	this->datasetElementsNumber				= object.datasetElementsNumber;
	this->isDatasetInternalFormatDense		= object.isDatasetInternalFormatDense;
	this->referencePointsString				= object.referencePointsString;
	this->referencePoints.clear();
	this->isReferencePointFormatDense		= object.isReferencePointFormatDense;
	this->projectionDimensions.clear();
	this->projectionSourceSequence.clear();
	this->classificationSubsetFactor		= object.classificationSubsetFactor;
	this->useBinaryPlacement				= object.useBinaryPlacement;
	this->pSampleIndex						= object.pSampleIndex;
	this->sSampleIndex						= object.sSampleIndex;
	this->constantVantagePointString		= object.constantVantagePointString;
	this->projectionDimensionsString		= object.projectionDimensionsString;
	this->projectionSortingCriteriaString	= object.projectionSortingCriteriaString;
	this->algorithmExecutionTime			= object.algorithmExecutionTime;
	this->clusteringExecutionTime			= object.clusteringExecutionTime;
	this->distanceCalculationExecutionTime	= object.distanceCalculationExecutionTime;
	this->normalizingDatasetExecutionTime	= object.normalizingDatasetExecutionTime;
	this->positioningExecutionTime			= object.positioningExecutionTime;
	this->sortingPointsExecutionTime		= object.sortingPointsExecutionTime;
	this->indexBuildingExecutionTime		= object.indexBuildingExecutionTime;
	this->datafileReadingTime				= object.datafileReadingTime;
	this->calculatingReferencePointsTime	= object.calculatingReferencePointsTime;
}

Report::Report(const Properties& properties, const TimeReport& timeReport, const string reportFileName){
	
	this->reportFileName					= reportFileName;
	this->propertiesFileName				= properties.propertiesFileName;
	this->algorithmType						= properties.algorithmType;
	this->algorithmGroup					= properties.algorithmGroup;
	this->algorithmGroupId					= properties.algorithmGroupId;
	this->algorithmName						= properties.algorithmName;
	this->algorithmNameId					= properties.algorithmNameId;
	this->eps								= properties.eps;
	this->minEps							= properties.minEps;
	this->avgEps							= properties.avgEps;
	this->maxEps							= properties.maxEps;
	this->minPts							= properties.minPts;
	this->k									= properties.k;
	this->useCosineSimilarity				= properties.useCosineSimilarity;
	this->isDatasetFileFormatDense			= properties.isDatasetFileFormatDense;
	this->datasetFilePath					= properties.datasetFilePath;
	this->datasetDimension					= properties.datasetDimension;
	this->datasetDimensionValueTreshold		= properties.datasetDimensionValueTreshold;
	this->datasetElementsNumber				= properties.datasetElementsNumber;
	this->isDatasetInternalFormatDense		= properties.isDatasetInternalFormatDense;
	this->referencePointsString				= properties.referencePointsString;
	this->referencePoints.clear();
	this->isReferencePointFormatDense		= properties.isReferencePointFormatDense;
	this->projectionDimensions.clear();
	this->projectionSourceSequence.clear();
	this->classificationSubsetFactor		= properties.classificationSubsetFactor;
	this->useBinaryPlacement				= properties.useBinaryPlacement;
	this->pSampleIndex						= properties.pSampleIndex;
	this->sSampleIndex						= properties.sSampleIndex;
	this->constantVantagePointString		= properties.constantVantagePointString;
	this->projectionDimensionsString		= properties.projectionDimensionsString;
	this->projectionSortingCriteriaString	= properties.projectionSortingCriteriaString;
	this->algorithmExecutionTime			= timeReport.algorithmExecutionTime;
	this->clusteringExecutionTime			= timeReport.clusteringExecutionTime;
	this->distanceCalculationExecutionTime	= timeReport.distanceCalculationExecutionTime;
	this->normalizingDatasetExecutionTime	= timeReport.normalizingDatasetExecutionTime;
	this->positioningExecutionTime			= timeReport.positioningExecutionTime;
	this->sortingPointsExecutionTime		= timeReport.sortingPointsExecutionTime;
	this->indexBuildingExecutionTime		= timeReport.indexBuildingExecutionTime;
	this->datafileReadingTime				= timeReport.datafileReadingTime;
	this->calculatingReferencePointsTime	= timeReport.calculatingReferencePointsTime;
}

void Report::printHeader(ofstream& os){
	
	os<<"Algorithm Execution [s]"<<columnSeparator;
	os<<"Clustering [s]"<<columnSeparator;
	os<<"Index Building [s]"<<columnSeparator;
	os<<"Distance Calculation [s]"<<columnSeparator;
	os<<"Points Sorting [s]"<<columnSeparator;
	os<<"Positioning [s]"<<columnSeparator;	
	os<<"Normalization [s]"<<columnSeparator;
	os<<"Datafile Reading [s]"<<columnSeparator;
	os<<"Reference Points Calculation [s]"<<columnSeparator;
	os<<"Positioning [s]"<<columnSeparator;	
	os<<"Algorithm Type"<<columnSeparator;	
	os<<"Algorithm Name"<<columnSeparator;
	os<<"Use Cosine Similarity"<<columnSeparator;
	os<<"Dataset File Path"<<columnSeparator;
	os<<"Dataset Size"<<columnSeparator;
	os<<"Dataset Maximal Dimension"<<columnSeparator;
	os<<"Dataset Dimension Value Treshold"<<columnSeparator;	
	os<<"Dataset Internal Representation"<<columnSeparator;	
	os<<"Reference Points"<<columnSeparator;
	os<<"Projection Dimensions"<<columnSeparator;
	os<<"Projection Source Sequence"<<columnSeparator;
	os<<"Classification Subset Factor"<<columnSeparator;
	os<<"Placement Method"<<columnSeparator;
	os<<"K"<<columnSeparator;
	os<<"Eps"<<columnSeparator;
	os<<"minEps"<<columnSeparator;
	os<<"avgEps"<<columnSeparator;
	os<<"maxEps"<<columnSeparator;
	os<<"MinPts"<<columnSeparator;
	os<<"P Sample Index"<<columnSeparator;	
	os<<"S Sample Index"<<columnSeparator;	
	os<<"Properties File Path"<<columnSeparator;	
	os<<"Report File Path";	
	os<<endl;
}

void Report::print(ofstream& os){
	
	if(this->algorithmExecutionTime != DBL_MIN){
	
		os<<this->algorithmExecutionTime<<columnSeparator;
	}
	else{
		
		os<<"N/A"<<columnSeparator;
	}
	if(this->clusteringExecutionTime != DBL_MIN){
	
		os<<this->clusteringExecutionTime<<columnSeparator;
	}
	else{
		
		os<<"N/A"<<columnSeparator;
	}
	if(this->indexBuildingExecutionTime != DBL_MIN){
	
		os<<this->indexBuildingExecutionTime<<columnSeparator;
	}
	else{
		
		os<<"N/A"<<columnSeparator;
	}
	if(this->distanceCalculationExecutionTime != DBL_MIN){
	
		os<<this->distanceCalculationExecutionTime<<columnSeparator;
	}
	else{
		
		os<<"N/A"<<columnSeparator;
	}
	if(this->sortingPointsExecutionTime != DBL_MIN){
	
		os<<this->sortingPointsExecutionTime<<columnSeparator;
	}
	else{
		
		os<<"N/A"<<columnSeparator;
	}
	if(this->positioningExecutionTime != DBL_MIN){
	
		os<<this->positioningExecutionTime<<columnSeparator;
	}
	else{
		
		os<<"N/A"<<columnSeparator;
	}
	if(this->normalizingDatasetExecutionTime != DBL_MIN){
	
		os<<this->normalizingDatasetExecutionTime<<columnSeparator;
	}
	else{
		
		os<<"N/A"<<columnSeparator;
	}
	if(this->datafileReadingTime != DBL_MIN){
	
		os<<this->datafileReadingTime<<columnSeparator;
	}
	else{
		
		os<<"N/A"<<columnSeparator;
	}
	if(this->calculatingReferencePointsTime != DBL_MIN){
	
		os<<this->calculatingReferencePointsTime<<columnSeparator;
	}
	else{
		
		os<<"N/A"<<columnSeparator;
	}
	if(this->positioningExecutionTime != DBL_MIN){
	
		os<<this->positioningExecutionTime<<columnSeparator;
	}
	else{
		
		os<<"N/A"<<columnSeparator;
	}	
	os<<this->algorithmType<<columnSeparator;	
	os<<this->algorithmName<<columnSeparator;
	if(this->useCosineSimilarity){
		
		os<<"true"<<columnSeparator;
	}
	else{
		
		os<<"false"<<columnSeparator;
	}
	os<<this->datasetFilePath<<columnSeparator;
	os<<this->datasetElementsNumber<<columnSeparator;
	os<<this->datasetDimension<<columnSeparator;
	os<<this->datasetDimensionValueTreshold<<columnSeparator;	
	if(this->isDatasetInternalFormatDense){
		
		os<<"dense"<<columnSeparator;
	}
	else{
		
		os<<"sparse"<<columnSeparator;
	}
	
	if(this->algorithmName == Properties::DBSCAN 
		||this->algorithmName == Properties::VP_TREE
		||this->algorithmName == Properties::K_NEIGHBORHOOD){
	
		os<<"N/A"<<columnSeparator;
	}
	else{
	
		os<<this->referencePointsString<<columnSeparator;
	}
	if(this->algorithmName == Properties::TI_DBSCAN_REF_PROJECTION 
		||this->algorithmName == Properties::TI_K_NEIGHBORHOOD_REF_PROJECTION
		){
	
			os<<this->projectionDimensionsString<<columnSeparator;
			os<<this->projectionSortingCriteriaString<<columnSeparator;
	}
	else{
	
		os<<"N/A"<<columnSeparator;
		os<<"N/A"<<columnSeparator;
	}
	if(this->classificationSubsetFactor != 0){
		
		os<<this->classificationSubsetFactor<<columnSeparator;
	}
	else{
		
		os<<"N/A"<<columnSeparator;
	}
	if((this->algorithmType == Properties::CLASSIFICATION) && (this->algorithmName != Properties::VPS_TREE)){

		if(this->useBinaryPlacement){
		
			os<<"binary"<<columnSeparator;
		}
		else{
		
			os<<"linear"<<columnSeparator;
		}
	}
	else{
	
		os<<"N/A"<<columnSeparator;
	}
	if(this->k != 0){
		
		os<<this->k<<columnSeparator;
	}
	else{
		
		os<<"N/A"<<columnSeparator;
	}
	if(this->eps != DBL_MIN){
	
		os<<this->eps<<columnSeparator;	
	}
	else{
	
		os<<"N/A"<<columnSeparator;
	}
	if(this->minEps != DBL_MIN){
	
		os<<this->minEps<<columnSeparator;	
	}
	else{
	
		os<<"N/A"<<columnSeparator;
	}
	if(this->avgEps != DBL_MIN){
	
		os<<this->avgEps<<columnSeparator;	
	}
	else{
	
		os<<"N/A"<<columnSeparator;
	}
	if(this->maxEps != DBL_MIN){
	
		os<<this->maxEps<<columnSeparator;	
	}
	else{
	
		os<<"N/A"<<columnSeparator;
	}
	if(this->minPts != 0){

		os<<this->minPts<<columnSeparator;
	}
	else{
		
		os<<"N/A"<<columnSeparator;
	}
	if(this->algorithmName != Properties::VPS_TREE){
	
		os<<this->pSampleIndex<<columnSeparator;
		os<<this->sSampleIndex<<columnSeparator;
	}
	else{
		
		os<<"N/A"<<columnSeparator;
		os<<"N/A"<<columnSeparator;
	}		
	os<<this->propertiesFileName<<columnSeparator;	
	os<<this->reportFileName;
	os<<endl;
}