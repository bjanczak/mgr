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
	this->minEps							= DBL_MAX;
	this->avgEps							= 0;
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
	this->isUseDatasetIndexAcess            = false;
	this->isUseBoundaries                   = false;
	this->realDistanceCalculationsCounters = vector<unsigned long>();
	this->verificationRealDistanceCalculationsCounters = vector<unsigned long>();
	this->vpTreeSearchRealDistanceCalculations = vector<unsigned long>();
	this->placementComparisonCounters = vector<unsigned long>();
	this->makeVpTreeRealDistanceCalculations = 0;
	this->makeVpTreeSelectVpRealDistanceCalculations = 0;
	this->vpTreeHeight = 0;
	this->vpTreeLeafs = 0;
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
	this->isUseDatasetIndexAcess            = object.isUseDatasetIndexAcess;
	this->isUseBoundaries                   = object.isUseBoundaries;
	this->realDistanceCalculationsCounters = vector<unsigned long>(object.realDistanceCalculationsCounters);
	this->verificationRealDistanceCalculationsCounters = vector<unsigned long>(object.verificationRealDistanceCalculationsCounters);
	this->vpTreeSearchRealDistanceCalculations = vector<unsigned long>(object.vpTreeSearchRealDistanceCalculations);
	this->makeVpTreeRealDistanceCalculations = object.makeVpTreeRealDistanceCalculations;
	this->makeVpTreeSelectVpRealDistanceCalculations = object.makeVpTreeSelectVpRealDistanceCalculations;
	this->placementComparisonCounters = vector<unsigned long>(placementComparisonCounters);
	this->vpTreeHeight = object.vpTreeHeight;
	this->vpTreeLeafs = object.vpTreeLeafs;
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
	this->isUseDatasetIndexAcess            = properties.isUseDatasetIndexAcess;
	this->isUseBoundaries                   = properties.isUseBoundaries;
	this->algorithmExecutionTime			= timeReport.algorithmExecutionTime;
	this->clusteringExecutionTime			= timeReport.clusteringExecutionTime;
	this->distanceCalculationExecutionTime	= timeReport.distanceCalculationExecutionTime;
	this->normalizingDatasetExecutionTime	= timeReport.normalizingDatasetExecutionTime;
	this->positioningExecutionTime			= timeReport.positioningExecutionTime;
	this->sortingPointsExecutionTime		= timeReport.sortingPointsExecutionTime;
	this->indexBuildingExecutionTime		= timeReport.indexBuildingExecutionTime;
	this->datafileReadingTime				= timeReport.datafileReadingTime;
	this->calculatingReferencePointsTime	= timeReport.calculatingReferencePointsTime;	
	this->realDistanceCalculationsCounters = vector<unsigned long>(timeReport.realDistanceCalculationsCounters);
	this->verificationRealDistanceCalculationsCounters = vector<unsigned long>(timeReport.verificationRealDistanceCalculationsCounters);
	this->vpTreeSearchRealDistanceCalculations = vector<unsigned long>(timeReport.vpTreeSearchRealDistanceCalculations);
	this->placementComparisonCounters = vector<unsigned long>(timeReport.placementComparisonCounters);
	this->makeVpTreeRealDistanceCalculations = timeReport.makeVpTreeRealDistanceCalculations;
	this->makeVpTreeSelectVpRealDistanceCalculations = timeReport.makeVpTreeSelectVpRealDistanceCalculations;
	this->vpTreeHeight = timeReport.vpTreeHeight;
	this->vpTreeLeafs = timeReport.vpTreeLeafs;
}

void Report::printHeader(ofstream& os){
	
	os<<"Algorithm Execution [s]"<<columnSeparator;
	os<<"Clustering [s]"<<columnSeparator;
	os<<"Index Building [s]"<<columnSeparator;
	os<<"Distance Calculation [s]"<<columnSeparator;
	os<<"Points Sorting [s]"<<columnSeparator;
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
	os<<"Use Dataset Index Access"<<columnSeparator;
	os<<"K"<<columnSeparator;
	os<<"Eps"<<columnSeparator;
	os<<"minEps"<<columnSeparator;
	os<<"avgEps"<<columnSeparator;
	os<<"maxEps"<<columnSeparator;
	os<<"MinPts"<<columnSeparator;
	os<<"P Sample Index"<<columnSeparator;	
	os<<"S Sample Index"<<columnSeparator;
	os<<"Use Boundaries"<<columnSeparator;	
	os<<"Real Distance Calculations"<<columnSeparator;
	os<<"Average Real Distance Calculations"<<columnSeparator;
	os<<"Minimal Real Distance Calculations"<<columnSeparator;	
	os<<"Maximal Real Distance Calculations"<<columnSeparator;	
	os<<"Verification Real Distance Calculations"<<columnSeparator;
	os<<"Average Verification Real Distance Calculations"<<columnSeparator;	
	os<<"Minimal Verification Real Distance Calculations"<<columnSeparator;	
	os<<"Maximal Verification Real Distance Calculations"<<columnSeparator;	
	os<<"Placement comparisons"<<columnSeparator;	
	os<<"Placement average comparisons"<<columnSeparator;	
	os<<"Placement min comparisons"<<columnSeparator;	
	os<<"Placement max comparisons"<<columnSeparator;	
	os<<"Building Vp-Tree real distance calculations"<<columnSeparator;
	os<<"Building Vp-Tree VP select real distance calculations"<<columnSeparator;
	os<<"Vp-Tree height"<<columnSeparator;
	os<<"Vp-Tree leafs"<<columnSeparator;
	os<<"Vp-Tree search real distance calculations"<<columnSeparator;
	os<<"Vp-Tree search average real distance calculations"<<columnSeparator;
	os<<"Vp-Tree search minimal real distance calculations"<<columnSeparator;
	os<<"Vp-Tree search maximal real distance calculations"<<columnSeparator;
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
		||this->algorithmName == Properties::DBSCAN_POINTS_ELIMINATION
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
	if(this->isUseDatasetIndexAcess){
		
		os<<"true"<<columnSeparator;
	}
	else{
		
		os<<"false"<<columnSeparator;
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
	if(this->minEps != DBL_MAX){
	
		os<<this->minEps<<columnSeparator;	
	}
	else{
	
		os<<"N/A"<<columnSeparator;
	}
	if(this->avgEps != 0){
	
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
		if(this->isUseBoundaries){

			os<<"true"<<columnSeparator;
		}
		else{

			os<<"false"<<columnSeparator;
		}
	}
	else{
		
		os<<"N/A"<<columnSeparator;
		os<<"N/A"<<columnSeparator;
		os<<"N/A"<<columnSeparator;
	}

	unsigned long realDistanceCalculationsCountersSum = 0;
	unsigned long realDistanceCalculationsCountersMin = ULONG_MAX;
	unsigned long realDistanceCalculationsCountersMax = 0;
	unsigned long verificationRealDistanceCalculationsCountersSum = 0;
	unsigned long verificationRealDistanceCalculationsCountersMin = ULONG_MAX;
	unsigned long verificationRealDistanceCalculationsCountersMax = 0;
	unsigned long vpTreeSearchRealDistanceCalculationsCountersSum = 0;
	unsigned long vpTreeSearchRealDistanceCalculationsCountersMin = ULONG_MAX;
	unsigned long vpTreeSearchRealDistanceCalculationsCountersMax = 0;
	unsigned long placementComparisonCountersSum = 0;
	unsigned long placementComparisonCountersMin = ULONG_MAX;
	unsigned long placementComparisonCountersMax = 0;

	vector<unsigned long>::iterator begin = this->realDistanceCalculationsCounters.begin();
	vector<unsigned long>::iterator end = this->realDistanceCalculationsCounters.end();	
	while(begin != end) {
		realDistanceCalculationsCountersSum = realDistanceCalculationsCountersSum + *begin;
		if (*begin > realDistanceCalculationsCountersMax) {
			realDistanceCalculationsCountersMax = *begin;
		}
		if (*begin < realDistanceCalculationsCountersMin) {
			realDistanceCalculationsCountersMin = *begin;
		}
		begin++;
	}
	begin = this->verificationRealDistanceCalculationsCounters.begin();
	end = this->verificationRealDistanceCalculationsCounters.end();	
	while(begin != end) {
		verificationRealDistanceCalculationsCountersSum = verificationRealDistanceCalculationsCountersSum + *begin;
		if (*begin > verificationRealDistanceCalculationsCountersMax) {
			verificationRealDistanceCalculationsCountersMax = *begin;
		}
		if (*begin < verificationRealDistanceCalculationsCountersMin) {
			verificationRealDistanceCalculationsCountersMin = *begin;
		}
		begin++;
	}
	begin = this->vpTreeSearchRealDistanceCalculations.begin();
	end = this->vpTreeSearchRealDistanceCalculations.end();	
	while(begin != end) {
		vpTreeSearchRealDistanceCalculationsCountersSum = vpTreeSearchRealDistanceCalculationsCountersSum + *begin;
		if (*begin > vpTreeSearchRealDistanceCalculationsCountersMax) {
			vpTreeSearchRealDistanceCalculationsCountersMax = *begin;
		}
		if (*begin < vpTreeSearchRealDistanceCalculationsCountersMax) {
			vpTreeSearchRealDistanceCalculationsCountersMax = *begin;
		}
		begin++;
	}
	begin = this->placementComparisonCounters.begin();
	end = this->placementComparisonCounters.end();	
	while(begin != end) {
		placementComparisonCountersSum = placementComparisonCountersSum + *begin;
		if (*begin > placementComparisonCountersMax) {
			placementComparisonCountersMax = *begin;
		}
		if (*begin < placementComparisonCountersMin) {
			placementComparisonCountersMin = *begin;
		}
		begin++;
	}

	if (this->realDistanceCalculationsCounters.size() > 0) {
		os<<realDistanceCalculationsCountersSum<<columnSeparator;
		os<<realDistanceCalculationsCountersSum/this->realDistanceCalculationsCounters.size()<<columnSeparator;
		os<<realDistanceCalculationsCountersMin<<columnSeparator;
		os<<realDistanceCalculationsCountersMax<<columnSeparator;		
	} else {
		os<<"N/A"<<columnSeparator;
		os<<"N/A"<<columnSeparator;
		os<<"N/A"<<columnSeparator;	
		os<<"N/A"<<columnSeparator;	
	}
	if (this->verificationRealDistanceCalculationsCounters.size() > 0) {
		os<<verificationRealDistanceCalculationsCountersSum<<columnSeparator;
		os<<verificationRealDistanceCalculationsCountersSum/this->verificationRealDistanceCalculationsCounters.size()<<columnSeparator;	
		os<<verificationRealDistanceCalculationsCountersMin<<columnSeparator;
		os<<verificationRealDistanceCalculationsCountersMax<<columnSeparator;
	} else {
		os<<"N/A"<<columnSeparator;
		os<<"N/A"<<columnSeparator;
		os<<"N/A"<<columnSeparator;	
		os<<"N/A"<<columnSeparator;	
	}
	if (this->placementComparisonCounters.size() > 0) {
		os<<placementComparisonCountersSum<<columnSeparator;
		os<<placementComparisonCountersSum/this->placementComparisonCounters.size()<<columnSeparator;	
		os<<placementComparisonCountersMin<<columnSeparator;
		os<<placementComparisonCountersMax<<columnSeparator;
	} else {
		os<<"N/A"<<columnSeparator;
		os<<"N/A"<<columnSeparator;
		os<<"N/A"<<columnSeparator;	
		os<<"N/A"<<columnSeparator;	
	}
	if (this->makeVpTreeRealDistanceCalculations > 0) {
		os<<this->makeVpTreeRealDistanceCalculations<<columnSeparator;
	}else {
		os<<"N/A"<<columnSeparator;
	}
	if (this->makeVpTreeRealDistanceCalculations > 0) {
		os<<this->makeVpTreeSelectVpRealDistanceCalculations<<columnSeparator;
	} else {
		os<<"N/A"<<columnSeparator;
	}
	if (this->vpTreeHeight > 0) {
		os<<this->vpTreeHeight<<columnSeparator;
	} else {
		os<<"N/A"<<columnSeparator;
	}
	if (this->vpTreeLeafs > 0) {
		os<<this->vpTreeLeafs<<columnSeparator;
	} else {
		os<<"N/A"<<columnSeparator;
	}
	if (this->vpTreeSearchRealDistanceCalculations.size() > 0) {
		os<<vpTreeSearchRealDistanceCalculationsCountersSum<<columnSeparator;
		os<<vpTreeSearchRealDistanceCalculationsCountersSum/this->vpTreeSearchRealDistanceCalculations.size()<<columnSeparator;
		os<<vpTreeSearchRealDistanceCalculationsCountersMin<<columnSeparator;
		os<<vpTreeSearchRealDistanceCalculationsCountersMax<<columnSeparator;
	} else {
		os<<"N/A"<<columnSeparator;
		os<<"N/A"<<columnSeparator;
		os<<"N/A"<<columnSeparator;	
		os<<"N/A"<<columnSeparator;	
	}
	os<<this->propertiesFileName<<columnSeparator;	
	os<<this->reportFileName;
	os<<endl;
}