/*
 ============================================================================
 Name        : Dataset.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-05
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Implementation of Dataset.h.
 ============================================================================
 */
#include "Dataset.h"

#include <algorithm>
#include <float.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>

Dataset::~Dataset(){
	
	this->classificationDataset.clear();
	this->datasetPoint.clear();
	this->datasetDbscanPoint.clear();
	this->datasetKNeighborhoodPoint.clear();
	this->classificationResult.clear();
	if(this->vpsTree != NULL){
	
		this->vpsTree->destroy();
	}
}

Dataset::Dataset(){

	this->isDense = false;
	this->algorithmGroup = "";
	this->algorithmType = "";
	this->algorithmName = "";
	this->dimension = 0;
	this->size = 0;
	this->datasetPoint;
	this->datasetDbscanPoint;
	this->datasetKNeighborhoodPoint;
	this->classificationDataset;
	this->classificationResult;
	this->filePath = "";
	this->vpsTree = NULL;
	this->minEps = DBL_MAX;
	this->maxEps = DBL_MIN;
	this->avgEps = 0;
}

void Dataset::clear(){
	
	isDense = false;
	dimension = 0;
	size = 0;
	algorithmGroup = "";
	algorithmType = "";
	algorithmName = "";
	filePath = "";

	this->minEps = DBL_MAX;
	this->maxEps = DBL_MIN;
	this->avgEps = 0;

	this->classificationDataset.clear();
	this->datasetPoint.clear();
	this->datasetDbscanPoint.clear();
	this->datasetKNeighborhoodPoint.clear();
	this->classificationResult.clear();
	
	if(this->vpsTree != NULL){
	
		this->vpsTree->destroy();
		this->vpsTree = NULL;
	}
}

void Dataset::setProperties(const Properties& properties){

	this->isDense = properties.isDatasetInternalFormatDense;
	this->algorithmGroup = properties.algorithmGroup;
	this->algorithmType = properties.algorithmType;
	this->algorithmName = properties.algorithmName;
	this->dimension = properties.datasetDimension;
	this->size = properties.datasetElementsNumber;
	this->filePath = properties.datasetFilePath;
	this->vpsTree = NULL;
	
	this->datasetPoint.reserve(properties.datasetElementsNumber);
}

void Dataset::readData(const Properties& properties){
	
	string bufor;
	string parameterName;
	string parameterValue;
	unsigned long elementCounter;
	unsigned long maxElements = size;
	vector<double> tempDensePoint;
	vector<SparsePoint> tempSparsePoint;
	ifstream inputFileStream (filePath.c_str());

	if (!inputFileStream){
		cerr<<"Error: properties file: "<<filePath<<" could not be opened"<<endl;
	}

	elementCounter = 1;

	/*
	 * Iterate all file text lines.
	 */
	if(inputFileStream.is_open()){
	
		while(inputFileStream.good()){
		
			getline(inputFileStream, bufor);
			
			if(bufor.size()>0){

				if(isDense){
					
					tempDensePoint = getDensePoint(bufor, properties);
					this->datasetPoint.push_back(Point(elementCounter, tempDensePoint));
				}
				else{

					tempSparsePoint = getSparsePoint(bufor, properties);
					this->datasetPoint.push_back(Point(elementCounter, tempSparsePoint));
				}
		
				/*
				 * Read only specified number of records.
				 */
				if(maxElements == elementCounter){
				
					break;
				}
				elementCounter++;

				unsigned int max_size = this->datasetPoint.max_size();
				if(elementCounter == 2000){
				
					int i = 0;
				}
			}
		}

		inputFileStream.close();
	}

	this->datasetPoint.shrink_to_fit();
}

Point Dataset::getZeroPoint(const Dataset& dataset){
	
	unsigned long id = 0;
	
	if(dataset.isDense){
		
		return Point::Point(id,vector<double>(Properties::getInstance().datasetDimension,0));
	}
	else{
		
		return Point::Point(id,vector<SparsePoint>());
	}
}

Point Dataset::getNPoint(const Dataset& dataset, const unsigned int n){
	
	unsigned long id = 0;
	unsigned long dimension = Properties::getInstance().datasetDimension;
	
	if(dataset.isDense){
		
		return Point::Point(id,vector<double>(dimension,n));
	}
	else{

		vector<SparsePoint> sparsePoint;
		
		for(unsigned long i = 0; i < dimension; i++){
		
			sparsePoint.push_back(SparsePoint(i+1, (double)n));
		}


		return Point::Point(id, sparsePoint);
	}
}

double Dataset::maxDimensionValue(const unsigned long dimension){
	
	double tempValue;
	double maxValue = -DBL_MAX;
	unsigned long datasetSize;
	const string &algorithmGroup = this->algorithmGroup;
	const vector<DbscanPoint> &datasetDbscanPoint = this->datasetDbscanPoint;
	const vector<KNeighborhoodPoint> &datasetKNeighborhoodPoint = this->datasetKNeighborhoodPoint;
	const vector<Point> &datasetPoint = this->datasetPoint;
	
	if(this->isDense){
	
		datasetSize = datasetPoint.size();

		for(unsigned long i=0; i<datasetSize; i++){
			
			tempValue = datasetPoint[i].denseFormatPoint[dimension];

			if(tempValue>maxValue){
				
				maxValue = tempValue;
			}
		}
	}
	else{
		
		unsigned long sparseFormatSize;		
		const vector<SparsePoint> *sparseFormatPoint;
		bool dimensionFound;

		datasetSize = datasetPoint.size();

		for(unsigned long i=0; i<datasetSize; i++){

			sparseFormatPoint =  &(datasetPoint[i].sparseFormatPoint);
			sparseFormatSize = sparseFormatPoint->size();
			dimensionFound = false;

			for(unsigned long j=0; j<sparseFormatSize; j++){
				
				if(sparseFormatPoint->at(j).dimension==dimension){
				
					dimensionFound = true;
					tempValue = sparseFormatPoint->at(j).value;

					if(tempValue>maxValue){
						
						maxValue = tempValue;
					}

					break;
				}
			}

			/*
		 	 * No dimension was found so dimension value is 0.
			 */
			if(!dimensionFound){
				
				maxValue = 0;
			}
		}
	}

	return maxValue;
}

double Dataset::minDimensionValue(const unsigned long dimension){

	double tempValue;
	double minValue = DBL_MAX;
	unsigned long datasetSize;
	const string &algorithmGroup = this->algorithmGroup;
	const vector<DbscanPoint> &datasetDbscanPoint = this->datasetDbscanPoint;
	const vector<KNeighborhoodPoint> &datasetKNeighborhoodPoint = this->datasetKNeighborhoodPoint;
	const vector<Point> &datasetPoint = this->datasetPoint;

	if(this->isDense){
	
		datasetSize = datasetPoint.size();

		for(unsigned long i=0; i<datasetSize; i++){
			
			tempValue = datasetPoint[i].denseFormatPoint[dimension];

			if(tempValue<minValue){
		
				minValue = tempValue;
			}
		}
	}
	else{
		
		unsigned long sparseFormatSize;		
		const vector<SparsePoint> *sparseFormatPoint;
		bool dimensionFound;

		datasetSize = datasetPoint.size();

		for(unsigned long i=0; i<datasetSize; i++){

			sparseFormatPoint =  &(datasetPoint[i].sparseFormatPoint);
			sparseFormatSize = sparseFormatPoint->size();
			dimensionFound = false;

			for(unsigned long j=0; j<sparseFormatSize; j++){
			
				if(sparseFormatPoint->at(j).dimension==dimension){
					
					dimensionFound = true;
					tempValue = sparseFormatPoint->at(j).value;

					if(tempValue<minValue){
						
						minValue = tempValue;
					}

					break;
				}
			}

			/*
			 * No dimension was found so dimension value is 0.
			 */
			if(!dimensionFound){
			
				minValue = 0;
			}
		}
	}

	return minValue;
}

vector<double> Dataset::getDensePoint(string line, const Properties& properties){

	unsigned long begin = 0;
	unsigned long end;
	string tempLine = deleteMultipleSpaceCharacters(line);
	double tempValue;
	double tresholdValue = properties.datasetDimensionValueTreshold;

	if(properties.isDatasetFileFormatDense){

		unsigned long counter = 0;
		vector<double> denseResult;
		denseResult.reserve(properties.datasetDimension);

		while(counter < properties.datasetDimension){
	
			end = tempLine.find(' ', begin);
			
			if(end==string::npos){
				
				tempValue = strtod(tempLine.substr(begin, tempLine.size() - begin).c_str(), NULL);
				
				if(tempValue>=tresholdValue){
					
					denseResult.push_back(tempValue);
				}
				else{
				
					denseResult.push_back(0);
				}

				break;
			}
			else{
			
				tempValue = strtod(tempLine.substr(begin, end - begin).c_str(), NULL);
				
				if(tempValue>=tresholdValue){
					
					denseResult.push_back(tempValue);
				}
				else{
				
					denseResult.push_back(0);
				}

				begin = end + 1;
			}

			counter++;
		}

		return denseResult;
	}
	else{
		
		vector<double> sparseResult(properties.datasetDimension, 0);		
		SparsePoint tempSparsePoint;
		vector<SparsePoint> tempResult;
		unsigned long tempResultSize;
		unsigned long dimension;
		bool isDimension = true;

		while(1){
	
			end = tempLine.find(' ', begin);

			if(end==string::npos){
			
				if(dimension <= properties.datasetDimension){
					
					tempValue = strtod(tempLine.substr(begin, tempLine.size() - begin).c_str(), NULL);

					if(tempValue>=tresholdValue){
					
						tempResult.push_back(SparsePoint(dimension, tempValue));
					}
				}

				break;
			}
			else{
			
				if(isDimension){
					dimension = strtol(tempLine.substr(begin, end - begin).c_str(), NULL, 10);
				}
				else{
					
					if(dimension <= properties.datasetDimension){

						tempValue = strtod(tempLine.substr(begin, end - begin).c_str(), NULL);

						if(tempValue>=tresholdValue){
					
							tempResult.push_back(SparsePoint(dimension, tempValue));
						}
					}
				}
				
				begin = end + 1;
			}
			
			isDimension = !isDimension;
		}

		tempResultSize = tempResult.size();
		
		/*
		 * Rewrite sparse point to dense point.
		 */
		for(unsigned long i=0; i< tempResultSize; i++){
		
			tempSparsePoint = tempResult[i];
			
			sparseResult[tempSparsePoint.dimension-1]=tempSparsePoint.value;
		}

		return sparseResult;
	}
}

vector<SparsePoint> Dataset::getSparsePoint(string line, const Properties& properties){

	unsigned long begin = 0;
	unsigned long end;
	vector<SparsePoint> result;
	string tempLine = deleteMultipleSpaceCharacters(line);
	double tempValue;
	double tresholdValue = properties.datasetDimensionValueTreshold;

	if(properties.isDatasetFileFormatDense){
		
		unsigned long tempResultSize = properties.datasetDimension;
		unsigned long counter = 0;
		vector<double> tempResult;
		tempResult.reserve(tempResultSize);

		while(counter < properties.datasetDimension){
	
			end = tempLine.find(' ', begin);
			
			if(end==string::npos){
				
				tempValue = strtod(tempLine.substr(begin, tempLine.size() - begin).c_str(), NULL);
				
				if(tempValue>=tresholdValue){
					
					tempResult.push_back(tempValue);
				}
				else{
				
					tempResult.push_back(0);
				}

				break;
			}
			else{
			
				tempValue = strtod(tempLine.substr(begin, end - begin).c_str(), NULL);
				
				if(tempValue>=tresholdValue){
					
					tempResult.push_back(tempValue);
				}
				else{
				
					tempResult.push_back(0);
				}

				begin = end + 1;
			}

			counter++;
		}

		for(unsigned long i=0; i<tempResultSize; i++){
		
			if(tempResult[i]!=0){
			
				result.push_back(SparsePoint(i+1, tempResult[i]));
			}
		}
	}
	else{
		
		unsigned long dimension;
		bool isDimension = true;

		while(1){
	
			end = tempLine.find(' ', begin);

			if(end==string::npos){
			
				if(dimension <= properties.datasetDimension){
				
					tempValue = strtod(tempLine.substr(begin,tempLine.size() - begin).c_str(), NULL);

					if(tempValue>=tresholdValue){
					
						result.push_back(SparsePoint(dimension,tempValue));
					}
				}

				break;
			}
			else{
			
				if(isDimension){
					dimension = strtol(tempLine.substr(begin,end - begin).c_str(), NULL, 10);
				}
				else{
					
					if(dimension <= properties.datasetDimension){
						
						tempValue = strtod(tempLine.substr(begin,end - begin).c_str(), NULL);

						if(tempValue>=tresholdValue){
					
							result.push_back(SparsePoint(dimension, tempValue));
						}
					}
				}
				
				begin = end + 1;
			}
			
			isDimension = !isDimension;
		}
	}

	/*
	 * Sort by dimensions.
	 */
	sort(result.begin(), result.end(), SparsePoint::comparator);

	return result;
}

Point Dataset::getRandomPoint(){
	
	unsigned long size;
	unsigned long id = 0;
	vector<double> tempPoint;
	vector<SparsePoint> tempSparsePoint;
	Point maxPoint = getMaxPoint();

	if(this->isDense){

		size = maxPoint.denseFormatPoint.size();

		for(unsigned long i = 0; i < size; i++){

			tempPoint.push_back((double)(rand()%(unsigned long)(maxPoint.denseFormatPoint[i] + 1)));
		}
		
		return Point(id, tempPoint);		
	}
	else{

		size = maxPoint.sparseFormatPoint.size();

		for(unsigned long i = 0; i < size; i++){

			tempSparsePoint.push_back(SparsePoint(maxPoint.sparseFormatPoint[i].dimension, (double)(rand()%(unsigned long)(maxPoint.sparseFormatPoint[i].value + 1))));
		}

		return Point(id, tempSparsePoint);
	}

}

Point Dataset::getMaxPoint(){

	unsigned long id = 0;
	double tempValue;
	vector<double> tempPoint;
	vector<SparsePoint> tempSparsePoint;
	const unsigned long &size = this->dimension;

	if(this->isDense){

		for(unsigned long i = 0; i<size; i++){
		
			tempPoint.push_back(maxDimensionValue(i));
		}

		return Point(id, tempPoint);		
	}
	else{
	
		for(unsigned long i = 1; i<=size; i++){
		
			tempValue = maxDimensionValue(i);

			if(tempValue>0){
			
				tempSparsePoint.push_back(SparsePoint(i, tempValue));
			}			
		}

		return Point(id, tempSparsePoint);
	}
}

Point Dataset::getMinPoint(){

	unsigned long id = 0;
	double tempValue;
	vector<double> tempPoint;
	vector<SparsePoint> tempSparsePoint;
	const unsigned long &size = this->dimension;

	if(this->isDense){

		for(unsigned long i = 0; i<size; i++){
		
			tempPoint.push_back(minDimensionValue(i));
		}

		return Point(id, tempPoint);		
	}
	else{
	
		for(unsigned long i = 1; i<=size; i++){
		
			tempValue = minDimensionValue(i);

			if(tempValue>0){
			
				tempSparsePoint.push_back(SparsePoint(i, tempValue));
			}			
		}

		return Point(id, tempSparsePoint);
	}
}

unsigned long Dataset::generateNFromDefinition(const string definition){
	
	string tempString;
	unsigned long n;
	unsigned long size =  definition.size() - 1;
														
	for(unsigned long i = 1; i < size; i++){
							
		tempString.push_back(definition[i]);
	}

	n = atoi(tempString.c_str());

	return n;
}

Point Dataset::getReferenceCustomPoint(string definition){
	
	vector<double> tempDensePoint;
	vector<SparsePoint> tempSparsePoint;
	vector<pair<unsigned long, string>> definitionInstructions;
	unsigned long size;
	unsigned long id;
	unsigned long tempFirst;
	double tempValue;
	string tempSecond;

	id = 0;

	definitionInstructions = customPointDefinitionToInstructionVector(definition, isDense);

	size = definitionInstructions.size();

	if(this->isDense){

		for(unsigned long i = 0; i < size; i++){
		
			tempFirst = definitionInstructions[i].first - 1;
			tempSecond = definitionInstructions[i].second;

			if(tempSecond=="min"){
			
				tempDensePoint[tempFirst] = minDimensionValue(tempFirst);
			}
			else 
				if(tempSecond=="max"){
				
					tempDensePoint[tempFirst] = maxDimensionValue(tempFirst);
				}
				else{
					
					if(tempSecond.find('.')==string::npos){
					
						tempSecond.push_back('.');
						tempSecond.push_back('0');
					}

					tempDensePoint[tempFirst] = strtod(tempSecond.c_str(), NULL);
				}
		}

		return Point(id, tempDensePoint);		
	}
	else{
	
		for(unsigned long i = 0; i<size; i++){
		
			tempFirst = definitionInstructions[i].first;
			tempSecond = definitionInstructions[i].second;

			if(tempSecond=="min"){
			
				tempValue = minDimensionValue(tempFirst);
				
				if(tempValue > 0){

					tempSparsePoint.push_back(SparsePoint(tempFirst, tempValue));
				}
			}
			else 
				if(tempSecond=="max"){
				
					tempValue = maxDimensionValue(tempFirst);

					if(tempValue > 0){
					
						tempSparsePoint.push_back(SparsePoint(tempFirst, tempValue));
					}
				}
				else{
					
					if(tempSecond.find('.')==string::npos){
					
						tempSecond.push_back('.');
						tempSecond.push_back('0');
					}
					
					tempValue = strtod(tempSecond.c_str(), NULL);

					if(tempValue > 0){
					
						tempSparsePoint.push_back(SparsePoint(tempFirst, tempValue));
					}				
				}	
			}

			return Point(id, tempSparsePoint);
		}
}

Point Dataset::getPatternReferenceCustomPoint(string definition, bool isDense, unsigned long dimension){
	
	unsigned long size = definition.size() - 3;
	unsigned long intervalSize;
	unsigned long id = 0;
	string tempDefinition;
	string tempValue;
	vector<double> interval;

	for(unsigned long i = 2; i < size; i++){
		
		tempDefinition.push_back(definition[i]);
	}

	size = tempDefinition.size();

	/*
	 * Read interval.
	 */
	for(unsigned long i = 0; i < size; i++){
		
		if(tempDefinition[i] == ','){
				
			interval.push_back(atof(tempValue.c_str()));
			tempValue.clear();
		}
		else
			if(i == size-1){
				
				tempValue.push_back(tempDefinition[i]);
				interval.push_back(atof(tempValue.c_str()));
				tempValue.clear();
			}
			else{
				
				tempValue.push_back(tempDefinition[i]);
			}	
	}

	intervalSize = interval.size();
	size = dimension;

	/*
	 * Fill ponit with interval.
	 */
	if(isDense){
			
		vector<double> tempDensePoint;

		for(unsigned long i = 0; i < size; i++){
				
			tempDensePoint.push_back(interval[i%intervalSize]);
		}

		return Point(id, tempDensePoint);	
	}
	else{
			
		vector<SparsePoint> tempSparsePoint;
		
		for(unsigned long i = 0; i < size; i++){
				
			tempSparsePoint.push_back(SparsePoint(i+1, interval[i%intervalSize]));
		}

		return Point(id, tempSparsePoint);	
	}
}

Point Dataset::getCustomPoint(string definition){

	if(isCustomReferencePoint(definition)){
	
		return getCustomPoint(definition);		
	}
	/*
	 * Reference point defined as pattern.
	 */
	else
		if(isPatternReferencePoint(definition)){
		
			return Dataset::getPatternReferenceCustomPoint(definition, this->isDense, this->dimension);
		}
		else{
		
			return Point();
		}
}

bool Dataset::isFunctionReferencePoint(const string pointDefinition){
	
	return (pointDefinition.find(',') == string::npos) && (pointDefinition.find('*') == string::npos);
}

bool Dataset::isCustomReferencePoint(const string pointDefinition){
	
	return (pointDefinition.find(',') != string::npos) && (pointDefinition.find('*') == string::npos);
}

bool Dataset::isPatternReferencePoint(const string pointDefinition){
	
	return (pointDefinition.find(',') != string::npos) && (pointDefinition.find('*') == string::npos);
}

vector<Point> Dataset::getReferencePoints(const Properties& properties){

	vector<Point> result;
	string referencePointsDefinition = properties.referencePointsString;
	string tempPointDefinition;
	unsigned long begin = 0;
	unsigned long end;
	unsigned long n;

	while(1){
	
		end = referencePointsDefinition.find(']', begin);
		
		if(end == string::npos){
			break;
		}

		tempPointDefinition = referencePointsDefinition.substr(begin, end-begin+1);

		if(isFunctionReferencePoint(tempPointDefinition)){
		
			if(tempPointDefinition == "[max]"){
				
				result.push_back(getMaxPoint());
			}
			else
				if(tempPointDefinition == "[min]"){
				
					result.push_back(getMinPoint());
				}
				else
					if(tempPointDefinition == "[rand]"){
					
						result.push_back(getRandomPoint());
					}
					else
						if(tempPointDefinition == "[0]"){
					
							result.push_back(getZeroPoint(*this));
						}
						else
							/*
							* Fill in n definition, like [98].
							*/
							{
								n = generateNFromDefinition(tempPointDefinition);

								result.push_back(getNPoint(*this, n));
							}	
		}
		else{
		
			result.push_back(getCustomPoint(tempPointDefinition));
		}
				
		begin = end+1;
	}

	return result;
}

string Dataset::deleteWhiteChars(const string& str){
	
	string result;
	unsigned long size = str.size();
	char c;
	
	for(unsigned long i = 0; i<size; i++){
		c = str[i];

		if(c!=' ' && c!='\t'){
			result.push_back(c);
		}
	}

   return result;
}

string Dataset::deleteMultipleSpaceCharacters(const string& str){

	string result;
	unsigned long size = str.size();
	char c;
	unsigned long spaceCounter = 0;

	for(unsigned long i = 0; i<size; i++){
		
		c = str[i];

		if(c==' '){
			
			spaceCounter++;
		}
		else{
		
			spaceCounter = 0;
		}

		if(spaceCounter<2){
		
			result.push_back(c);
		}
	}

	return result;
}

vector<pair<unsigned long, string>> Dataset::customPointDefinitionToInstructionVector(const string& definition, bool isDense){

	size_t begin = 1;
	size_t end;
	string tempString = Dataset::deleteWhiteChars(definition);
	bool isDimension = true;
	unsigned long counter = 0;
	unsigned long dimension;
	vector<pair<unsigned long, string>> result;

	if(isDense){
	
		while(1){
	
			end = tempString.find(',', begin);
			
			if(end==string::npos){
				result.push_back(pair<unsigned long, string>(counter,tempString.substr(begin, tempString.size()-1-begin)));
				break;
			}
			else{
			
				result.push_back(pair<unsigned long, string>(counter,tempString.substr(begin, end-begin)));
				counter++;
				begin = end + 1;
			}
		}
	}
	else{
	
		while(1){
	
			end = tempString.find(',', begin);

			if(end==string::npos){
			
				result.push_back(pair<unsigned long, string>(dimension,tempString.substr(begin, tempString.size()-1-begin)));
				break;
			}
			else{
			
				if(isDimension){
					dimension = strtol(tempString.substr(begin, end - begin).c_str(), NULL, 10);
				}
				else{
					
					result.push_back(pair<unsigned long, string>(dimension,tempString.substr(begin, end-begin)));
				}
				
				begin = end + 1;
			}
			
			isDimension = !isDimension;
		}
	}

	return result;
}

void Dataset::normalize(double alfa){

	Point zeroPoint = getZeroPoint(*this);
	double distance;
	unsigned long datasetSize;
	unsigned long vectorSize;

	if(this->isDense){		
		datasetSize = this->datasetPoint.size();
		Point *point;
		vector<double> *pointVector;

		for(unsigned long i=0; i<datasetSize; i++){

			point = &datasetPoint[i];
			pointVector = &(point->denseFormatPoint);
			distance = Point::minkowskiDistance(zeroPoint, *point, 2);
	
			if(distance != 0){
			
				vectorSize = pointVector->size();

				for(unsigned long j=0; j<vectorSize; j++){
					
					pointVector->at(j)=(pointVector->at(j))*(alfa/distance);
				}
			}
		}
	}
	else{
		datasetSize = this->datasetPoint.size();
		Point *point;
		vector<SparsePoint> *pointVector;

		for(unsigned long i=0; i<datasetSize; i++){

			point = &datasetPoint[i];
			pointVector = &(point->sparseFormatPoint);
			distance = Point::minkowskiDistance(zeroPoint, *point, 2);

			if(distance != 0){
			
				vectorSize = pointVector->size();

				for(unsigned long j=0; j<vectorSize; j++){
				
					pointVector->at(j).value=(pointVector->at(j).value)*(alfa/distance);
				}
			}
		}
	}
}

vector<Point> Dataset::generateSample(const vector<Point>& dataset, const Properties& properties){

	vector<Point> result;
	unsigned long datasetSize = dataset.size();
	unsigned long factor = 100 / properties.classificationSubsetFactor;
	

	for(unsigned long i=0; i< datasetSize; i++){
		
		if(i%factor == 0){
			
			result.push_back((Point) dataset[i]);
		}
	}

	return result;
}

void Dataset::printClusteringSumUp(ofstream& os){

	
	unsigned long clusterId = DbscanPoint::NOISE;
	unsigned long counter = 0;

	os<<"********************************************"<<endl;
	os<<"              Clustering Sum Up"<<endl;
	os<<"********************************************"<<endl;
	os<<endl;
	os<<endl;
	os<<endl;
	os<<endl;

	if(this->algorithmGroup == Properties::DBSCAN){
	
		vector<DbscanPoint>::iterator it;
		vector<DbscanPoint>::iterator end;

		sort(this->datasetDbscanPoint.begin(), this->datasetDbscanPoint.end(), DbscanPoint::clusterIdComparator);
		end = this->datasetDbscanPoint.end();

		os<<"Number of clusters : "<<this->datasetDbscanPoint[this->datasetDbscanPoint.size()-1].clusterId<<endl<<endl<<endl;

		for( it = this->datasetDbscanPoint.begin(); it != end; it++){

			if(clusterId != it->clusterId){
											
				if(clusterId == DbscanPoint::NOISE){
				
					os<<"Cluster ID\t\t:\tNOISE"<<endl;
				}
				else{

					os<<"Cluster ID\t\t:\t"<<clusterId<<endl;
				}
				
				os<<"Cluster size\t\t:\t"<<counter<<endl<<endl;

				counter = 1;
				clusterId = it->clusterId;
			}
			else{
			
				counter++;
			}
		}

		if(clusterId == DbscanPoint::NOISE){
			
			os<<"Cluster ID\t\t:\tNOISE"<<endl;
			os<<"Cluster size       : "<<counter<<endl<<endl;
		}
		else{
		
			os<<"Cluster ID\t\t:\t"<<clusterId<<endl;
			os<<"Cluster size\t\t:\t"<<counter<<endl<<endl;
		}
	}
	else
		if(this->algorithmGroup == Properties::K_NEIGHBORHOOD){
		
			multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>::iterator neighborsIt;
			multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator>::iterator neighborsEnd;

			os<<"\tminEps\t\t:\t"<<this->minEps<<endl;
			os<<"\tavgEps\t\t:\t"<<this->avgEps<<endl;
			os<<"\tmaxEps\t\t:\t"<<this->maxEps<<endl;
			os<<endl;
			os<<endl;

			vector<pair<Point, Point*>>::iterator it;
			vector<pair<Point, Point*>>::iterator end = this->classificationDataset.end();

			for(it = this->classificationDataset.begin(); it != end; it++){
				
				os<<"Classification point ID         : "<<it->first.id<<endl;
				os<<"Equivalent point ID             : "<<it->second->id<<endl;
				os<<"    eps          : "<<((KNeighborhoodPoint*)(it->second))->eps<<endl;
				os<<"    neighbors nr : "<<((KNeighborhoodPoint*)(it->second))->neighbors.size()<<endl;
				os<<"    neighbors    : "<<endl;

				neighborsEnd = ((KNeighborhoodPoint*)(it->second))->neighbors.end();
				
				for(neighborsIt = ((KNeighborhoodPoint*)(it->second))->neighbors.begin(); neighborsIt != neighborsEnd; neighborsIt++){

					os<<"        ID: "<<neighborsIt->second->id<<"\tdistance: "<<neighborsIt->first<<endl;
				}
				os<<endl;
			}
				
		}
		else
			if(this->algorithmType == Properties::CLASSIFICATION && (this->algorithmName == Properties::VP_TREE || this->algorithmName == Properties::VPS_TREE)){
				
				vector<pair<Point*, list<Point*>>>::iterator it = this->classificationResult.begin();
				vector<pair<Point*, list<Point*>>>::iterator end = this->classificationResult.end();
				list<Point*>::iterator listIt;
				list<Point*>::iterator listEnd;

				while(it != end){
				
					os<<"Classification point ID         : "<<it->first->id<<endl;
					os<<"    neighbors nr : "<<it->second.size()<<endl;
					os<<"    neighbors    : "<<endl;
					listIt = it->second.begin();
					listEnd = it->second.end();
					
					while(listIt != listEnd){
						
						os<<"        ID: "<<(*listIt)->id<<"\tdistance: "<<Point::minkowskiDistance(*it->first, *(*listIt), 2) <<endl;
						listIt++;
					}

					os<<endl;

					it++;
				}
			}
}

vector<KNeighborhoodPoint>::iterator Dataset::getPlacementBinary(const Point& point){
	
	unsigned long minIndex = 0;
	unsigned long maxIndex = datasetKNeighborhoodPoint.size() - 1;
	unsigned long currentIndex = 0;	
	unsigned long previousIndex = datasetKNeighborhoodPoint.size();
	vector<KNeighborhoodPoint>::iterator result = datasetKNeighborhoodPoint.begin();
	double tempValue;
	
	/*
	 * Criteria is the first calculated distance.
	 */
	double criteriaValue = point.distance[0];
	
	/*
	 * Find the first point as distant from reference poinnt
	 * as query point using binary search.
	 */
	while(1){
		
		previousIndex = currentIndex;

		currentIndex = (unsigned long)ceil(((double)(minIndex + maxIndex)) / 2);

		if(previousIndex == currentIndex){
			
			break;
		}

		tempValue = datasetKNeighborhoodPoint[currentIndex].distance[0];

		if(tempValue <= criteriaValue){
			
			minIndex = currentIndex;
		}
		else{
			
			maxIndex = currentIndex -1;
		}
	}

	return datasetKNeighborhoodPoint.begin() + currentIndex;	
}

vector<vector<KNeighborhoodPoint>::iterator>::iterator Dataset::indexGetPlacementBinary(vector<vector<KNeighborhoodPoint>::iterator>& datasetIndex, const Point& point){
	
	unsigned long minIndex = 0;
	unsigned long maxIndex = datasetIndex.size() - 1;
	unsigned long currentIndex = 0;	
	unsigned long previousIndex = datasetIndex.size();
	vector<vector<KNeighborhoodPoint>::iterator>::iterator result = datasetIndex.begin();
	double tempValue;
	
	/*
	 * Criteria is the first calculated distance.
	 */
	double criteriaValue = point.distance[0];
	
	/*
	 * Find the first point as distant from reference poinnt
	 * as query point using binary search.
	 */
	while(1){
		
		previousIndex = currentIndex;

		currentIndex = (unsigned long)ceil(((double)(minIndex + maxIndex)) / 2);

		if(previousIndex == currentIndex){
			
			break;
		}

		tempValue = (*datasetIndex[currentIndex]).distance[0];

		if(tempValue <= criteriaValue){
			
			minIndex = currentIndex;
		}
		else{
			
			maxIndex = currentIndex -1;
		}
	}

	return datasetIndex.begin() + currentIndex;
}
vector<KNeighborhoodPoint>::iterator Dataset::getPlacementBinary(vector<KNeighborhoodPoint>& datasetIndex, const Point& point){
	
	unsigned long minIndex = 0;
	unsigned long maxIndex = datasetIndex.size() - 1;
	unsigned long currentIndex = 0;	
	unsigned long previousIndex = datasetIndex.size();
	vector<KNeighborhoodPoint>::iterator result = datasetIndex.begin();
	double tempValue;
	
	/*
	 * Criteria is the first calculated distance.
	 */
	double criteriaValue = point.distance[0];
	
	/*
	 * Find the first point as distant from reference poinnt
	 * as query point using binary search.
	 */
	while(1){
		
		previousIndex = currentIndex;

		currentIndex = (unsigned long)ceil(((double)(minIndex + maxIndex)) / 2);

		if(previousIndex == currentIndex){
			
			break;
		}

		tempValue = datasetIndex[currentIndex].distance[0];

		if(tempValue <= criteriaValue){
			
			minIndex = currentIndex;
		}
		else{
			
			maxIndex = currentIndex -1;
		}
	}

	return datasetIndex.begin() + currentIndex;
}

vector<KNeighborhoodPoint>::iterator Dataset::getPlacementLineary(const Point& point){
	
	unsigned long minIndex = 0;
	unsigned long maxIndex = datasetKNeighborhoodPoint.size() - 1;
	unsigned long currentIndex = 0;
	unsigned long previousIndex = datasetKNeighborhoodPoint.size();
	vector<KNeighborhoodPoint>::iterator it;
	vector<KNeighborhoodPoint>::iterator end = datasetKNeighborhoodPoint.end();
	vector<KNeighborhoodPoint>::iterator result;
	double value = DBL_MIN;
	double diff = DBL_MAX;
	double currentDiff = DBL_MAX;

	/*
	 * multimap complexity:
	 *	insert	O(n),
	 *	search	O(n).
	 */
	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator> candidates;
	
	/*
	 * Criteria is the first distance.
	 */
	double criteriaValue = point.distance[0];
	
	result = it;

	/*
	 * Find the first point as distant from reference poinnt
	 * as query point using lineary search.
	 */
	for(it = datasetKNeighborhoodPoint.begin(); it != end; it++){
	
		currentDiff = abs(it->distance[0] - criteriaValue);

		if(diff >= currentDiff){
		
			diff = currentDiff;
		}
		else{
			
			break;
		}

		result = it;
	}

	return result;
}

vector<vector<KNeighborhoodPoint>::iterator>::iterator Dataset::indexGetPlacementLineary(vector<vector<KNeighborhoodPoint>::iterator>& datasetIndex, const Point& point){
	
	unsigned long minIndex = 0;
	unsigned long maxIndex = datasetIndex.size() - 1;
	unsigned long currentIndex = 0;
	unsigned long previousIndex = datasetIndex.size();
	vector<vector<KNeighborhoodPoint>::iterator>::iterator it;
	vector<vector<KNeighborhoodPoint>::iterator>::iterator end = datasetIndex.end();
	vector<vector<KNeighborhoodPoint>::iterator>::iterator result;
	double value = DBL_MIN;
	double diff = DBL_MAX;
	double currentDiff = DBL_MAX;
	
	/*
	 * Criteria is the first distance.
	 */
	double criteriaValue = point.distance[0];
	
	result = it;

	/*
	 * Find the first point as distant from reference poinnt
	 * as query point using lineary search.
	 */
	for(it = datasetIndex.begin(); it != end; it++){
	
		currentDiff = abs((*it)->distance[0] - criteriaValue);

		if(diff >= currentDiff){
		
			diff = currentDiff;
		}
		else{
			
			break;
		}

		result = it;
	}

	return result;
}
vector<KNeighborhoodPoint>::iterator Dataset::getPlacementLineary(vector<KNeighborhoodPoint>& datasetIndex, const Point& point){
	
	unsigned long minIndex = 0;
	unsigned long maxIndex = datasetIndex.size() - 1;
	unsigned long currentIndex = 0;
	unsigned long previousIndex = datasetIndex.size();
	vector<KNeighborhoodPoint>::iterator it;
	vector<KNeighborhoodPoint>::iterator end = datasetIndex.end();
	vector<KNeighborhoodPoint>::iterator result;
	double value = DBL_MIN;
	double diff = DBL_MAX;
	double currentDiff = DBL_MAX;
	
	/*
	 * Criteria is the first distance.
	 */
	double criteriaValue = point.distance[0];
	
	result = it;

	/*
	 * Find the first point as distant from reference poinnt
	 * as query point using lineary search.
	 */
	for(it = datasetIndex.begin(); it != end; it++){
	
		currentDiff = abs(it->distance[0] - criteriaValue);

		if(diff >= currentDiff){
		
			diff = currentDiff;
		}
		else{
			
			break;
		}

		result = it;
	}

	return result;
}

void Dataset::fillKNeighborhoodPointVector(vector<KNeighborhoodPoint>& result, const vector<Point>& dataset){

	vector<Point>::const_iterator it = dataset.begin();
	vector<Point>::const_iterator end = dataset.end();	

	for(; it != end; it++){
		
		result.push_back(KNeighborhoodPoint((*it)));
	}
	
	result.shrink_to_fit();
}

void Dataset::fillDbscanPointVector(vector<DbscanPoint>& result, const vector<Point>& dataset){

	vector<Point>::const_iterator it = dataset.begin();
	vector<Point>::const_iterator end = dataset.end();

	for(; it != end; it++){
		
		result.push_back(DbscanPoint(*it));
	}

	result.shrink_to_fit();
}

void Dataset::fillVpsPointVector(vector<VpsPoint>& result, const vector<Point>& dataset, const bool isDense){

	vector<Point>::const_iterator it = dataset.begin();
	vector<Point>::const_iterator end = dataset.end();

	if(isDense){

		for(; it != end; it++){
		
			result.push_back(VpsPoint(it->id, it->denseFormatPoint));
		}
	}
	else{
	
		for(; it != end; it++){
		
			result.push_back(VpsPoint(it->id, it->sparseFormatPoint));
		}
	}

	result.shrink_to_fit();
}

void Dataset::fillVpsPointList(list<VpsPoint>& result, const vector<Point>& dataset, const bool isDense){

	vector<Point>::const_iterator it = dataset.begin();
	vector<Point>::const_iterator end = dataset.end();

	if(isDense){

		for(; it != end; it++){
		
			result.push_back(VpsPoint(it->id, it->denseFormatPoint));
		}
	}
	else{
	
		for(; it != end; it++){
		
			result.push_back(VpsPoint(it->id, it->sparseFormatPoint));
		}
	}
}

void Dataset::calculateKNeighborhoodEps(double& minEpsP, double& avgEpsP, double& maxEpsP){

	vector<KNeighborhoodPoint> dataset = this->datasetKNeighborhoodPoint;
	vector<KNeighborhoodPoint>::iterator it;
	vector<KNeighborhoodPoint>::iterator end = dataset.end();

	double minEps = DBL_MAX;
	double maxEps = DBL_MIN;
	double avgEps = 0;
	
	double tempEps;

	unsigned long counter = 0;

	for(it = dataset.begin(); it != end; it++){
	
		if(it->neighbors.size() > 0){
		
			tempEps = it->eps;

			avgEps = avgEps + tempEps;

			if(tempEps < minEps && tempEps != 0){
		
				minEps = tempEps;
			}

			if(tempEps > maxEps){
		
				maxEps = tempEps;
			}

			counter ++;
		}
	}

	avgEps = avgEps / counter;

	this->minEps = minEps;
	this->avgEps = avgEps;	
	this->maxEps = maxEps;
	
	minEpsP = minEps;
	avgEpsP = avgEps;
	maxEpsP = maxEps;
}