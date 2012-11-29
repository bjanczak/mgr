/*
 ============================================================================
 Name        : Properties.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-03
 Version     : 2.0
 Copyright   : Your copyright notice
 Description : Implementation of Properties.h
 ============================================================================
 */
#include "Properties.h"

#include <fstream>
#include <iostream>

/*
 * Parameters names.
 */
const string Properties::ALGORITHM_TYPE_PARAMETER_NAME = "algorithm_type";
const string Properties::ALGORITHM_GROUP_PARAMETER_NAME = "algorithm_group";
const string Properties::ALGORITHM_NAME_PARAMETER_NAME = "algorithm_name";
const string Properties::EPS_PARAMETER_NAME = "eps";
const string Properties::MIN_PTS_PARAMETER_NAME = "min_pts";
const string Properties::K_PARAMETER_NAME = "k";
const string Properties::USE_COSINE_SIMILARITY_PARAMETER_NAME = "use_cosine_similarity";
const string Properties::IS_DATASET_FILE_FORMAT_DENSE_PARAMETER_NAME = "dataset_file_format";
const string Properties::DATASET_FILE_PATH_PARAMETER_NAME = "dataset_file_path";
const string Properties::DATASET_DIMENSION_PARAMETER_NAME = "dataset_dimension";
const string Properties::DATASET_DIMENSION_VALUE_TRESHOLD_PARAMETER_NAME = "dataset_dimension_value_threshold";
const string Properties::DATASET_ELEMENTS_NUMBER_PARAMETER_NAME = "dataset_elements_number";
const string Properties::IS_DATASET_INTERNAL_REPRESENTATION_DENSE_PARAMETER_NAME = "dataset_internal_format";
const string Properties::REFERENCE_POINT_PARAMETER_NAME = "reference_point";
const string Properties::IS_REFERENCE_POINT_FORMAT_DENSE_PARAMETER_NAME = "reference_point_format";
const string Properties::PROJECTION_DIMENSIONS_PARAMETER_NAME = "projection_dimensions";
const string Properties::PROJECTION_SOURCE_SEQUENCE_PARAMETER_NAME = "projection_source_sequence";
const string Properties::CLASSIFICATION_SUBSET_FACTOR_PARAMETER_NAME = "classification_subset_factor";
const string Properties::USE_BINARY_PLACEMENT_PARAMETER_NAME = "use_binary_placement";
const string Properties::P_SAMPLE_INDEX_PARAMETER_NAME = "p_sample_index";
const string Properties::S_SAMPLE_INDEX_PARAMETER_NAME = "s_sample_index";
const string Properties::SEARCH_METHOD_PARAMETER_NAME = "search_method";
const string Properties::IS_USE_DATASET_USE_INDEX = "use_dataset_index_access";

/*
 * Algorithms names.
 */
const string Properties::DBSCAN = "dbscan";
const string Properties::TI_DBSCAN = "ti_dbscan";
const string Properties::TI_DBSCAN_REF = "ti_dbscan_ref";
const string Properties::TI_DBSCAN_REF_PROJECTION = "ti_dbscan_ref_projection";
const string Properties::K_NEIGHBORHOOD = "k_neighborhood";
const string Properties::TI_K_NEIGHBORHOOD = "ti_k_neighborhood";
const string Properties::TI_K_NEIGHBORHOOD_REF = "ti_k_neighborhood_ref";
const string Properties::TI_K_NEIGHBORHOOD_REF_PROJECTION = "ti_k_neighborhood_ref_projection";
const string Properties::VP_TREE = "vp_tree";
const string Properties::VPS_TREE = "vps_tree";

/*
 * Algorithms and groups IDs.
 */
const unsigned long Properties::DBSCAN_ID = 1;
const unsigned long Properties::TI_DBSCAN_ID = 2;
const unsigned long Properties::TI_DBSCAN_REF_ID = 3;
const unsigned long Properties::TI_DBSCAN_REF_PROJECTION_ID = 4;
const unsigned long Properties::K_NEIGHBORHOOD_ID = 5;
const unsigned long Properties::TI_K_NEIGHBORHOOD_ID = 6;
const unsigned long Properties::TI_K_NEIGHBORHOOD_REF_ID = 7;
const unsigned long Properties::TI_K_NEIGHBORHOOD_REF_PROJECTION_ID = 8;
const unsigned long Properties::VP_TREE_ID = 9;
const unsigned long Properties::VPS_TREE_ID = 10;
const unsigned long Properties::OTHER_ID = 11;

/*
 * Constants.
 */
const string Properties::GROUPING = "grouping";
const string Properties::CLASSIFICATION = "classification";
const string Properties::DENSE = "dense";
const string Properties::SPARSE = "sparse";
const string Properties::VP_CONSTANT = "vp_constant";
const string Properties::VP_CALCULATED = "vp_calculated";
const string Properties::TRUE = "true";
const string Properties::FALSE = "false";
const string Properties::OTHER = "other";
const string Properties::RANGE = "range";

Properties::~Properties(){

	this->referencePoints.clear();
	this->projectionDimensions.clear();
	this->projectionSourceSequence.clear();
}

void Properties::clear(){

	propertiesFileName = "";
	algorithmType = "";
	algorithmGroup = "";
	algorithmGroupId = 0;
	algorithmName = "";
	algorithmNameId = 0;
	eps = DBL_MIN;
	minEps = DBL_MIN;
	avgEps = DBL_MIN;
	maxEps = DBL_MIN;
	minPts = 0;
	k = 0;
	useCosineSimilarity = false;
	isDatasetFileFormatDense = false;
	datasetFilePath = "";
	datasetDimension = 0;
	datasetDimensionValueTreshold = 0;
	datasetElementsNumber = 0;
	isDatasetInternalFormatDense = false;
	referencePointsString = "";
	referencePoints.clear();
	isReferencePointFormatDense = false;
	projectionDimensions.clear();
	projectionSourceSequence.clear();
	classificationSubsetFactor = 0;
	useBinaryPlacement = false;
	pSampleIndex = 0;
	sSampleIndex = 0;
	constantVantagePointString = "";
	projectionSortingCriteriaString = "";
	projectionDimensionsString = "";
	isUseDatasetIndexAcess = false;
}


unsigned long Properties::getAlgorithmGroupNameId(string algorithmGroupName){

	unsigned long result;

	if(algorithmGroupName == Properties::DBSCAN){
		
		result = Properties::DBSCAN_ID;
	}
	else
		if(algorithmGroupName == Properties::K_NEIGHBORHOOD){
			
			result = Properties::K_NEIGHBORHOOD_ID;
		}
		else
			if(algorithmGroupName == Properties::OTHER){
				
				result = Properties::OTHER_ID;
			}

	return result;
}

string Properties::getAlgorithmGroupName(string algorithmName){

	string result;

	if(algorithmName == Properties::DBSCAN){

			result = Properties::DBSCAN;
		}
		else
			if(algorithmName == Properties::TI_DBSCAN){

					result = Properties::DBSCAN;
			}
			else
				if(algorithmName == Properties::TI_DBSCAN_REF){
						
					result = Properties::DBSCAN;
				}
				else
					if(algorithmName == Properties::TI_DBSCAN_REF_PROJECTION){

						result = Properties::DBSCAN;
					}
					else
						if(algorithmName == Properties::K_NEIGHBORHOOD){

							result = Properties::K_NEIGHBORHOOD;
						}
						else
							if(algorithmName == Properties::TI_K_NEIGHBORHOOD){
	
								result = Properties::K_NEIGHBORHOOD;
							}
							else
								if(algorithmName == Properties::TI_K_NEIGHBORHOOD_REF){
	
									result = Properties::K_NEIGHBORHOOD;
								}
								else
									if(algorithmName == Properties::TI_K_NEIGHBORHOOD_REF_PROJECTION){
	
										result = Properties::K_NEIGHBORHOOD;
									}
									else
										if(algorithmName == Properties::VP_TREE){
	
											result = Properties::OTHER;
										}
										else
											if(algorithmName == Properties::VPS_TREE){
	
												result = Properties::OTHER;
											}

	return result;
}

string Properties::getAlgorithmType(string algorithmName){

	string result;

	if(algorithmName == Properties::DBSCAN
		|| algorithmName == Properties::TI_DBSCAN
		|| algorithmName == Properties::TI_DBSCAN_REF
		|| algorithmName == Properties::TI_DBSCAN_REF_PROJECTION){

			result = Properties::GROUPING;
		}
		else
			if(algorithmName == Properties::K_NEIGHBORHOOD
				|| algorithmName == Properties::TI_K_NEIGHBORHOOD
				|| algorithmName == Properties::TI_K_NEIGHBORHOOD_REF
				|| algorithmName == Properties::TI_K_NEIGHBORHOOD_REF_PROJECTION
				|| algorithmName == Properties::VP_TREE
				|| algorithmName == Properties::VPS_TREE){

					result = Properties::CLASSIFICATION;
			}
						
	return result;
}

unsigned long Properties::getAlgorithmNameId(string algorithmName){
	
	unsigned long result = 0;
	
	if(algorithmName == Properties::DBSCAN){

			result = Properties::DBSCAN_ID;
		}
		else
			if(algorithmName == Properties::TI_DBSCAN){

					result = Properties::TI_DBSCAN_ID;
			}
			else
				if(algorithmName == Properties::TI_DBSCAN_REF){
						
					result = Properties::TI_DBSCAN_REF_ID;
				}
				else
					if(algorithmName == Properties::TI_DBSCAN_REF_PROJECTION){

						result = Properties::TI_DBSCAN_REF_PROJECTION_ID;
					}
					else
						if(algorithmName == Properties::K_NEIGHBORHOOD){

							result = Properties::K_NEIGHBORHOOD_ID;
						}
						else
							if(algorithmName == Properties::TI_K_NEIGHBORHOOD){
	
								result = Properties::TI_K_NEIGHBORHOOD_ID;
							}
							else
								if(algorithmName == Properties::TI_K_NEIGHBORHOOD_REF){
	
									result = Properties::TI_K_NEIGHBORHOOD_REF_ID;
								}
								else
									if(algorithmName == Properties::TI_K_NEIGHBORHOOD_REF_PROJECTION){
	
										result = Properties::TI_K_NEIGHBORHOOD_REF_PROJECTION_ID;
									}
									else
										if(algorithmName == Properties::VP_TREE){
	
											result = Properties::VP_TREE_ID;
										}
										else
											if(algorithmName == Properties::VPS_TREE){
	
												result = Properties::VPS_TREE_ID;
											}
	
	return result;
}

Properties::Properties(){
	
	propertiesFileName = "";
	algorithmType = "";
	algorithmGroup = "";
	algorithmGroupId = 0;
	algorithmName = "";
	algorithmNameId = 0;
	eps = DBL_MIN;
	minEps = DBL_MIN;
	avgEps = DBL_MIN;
	maxEps = DBL_MIN;
	minPts = 0;
	k = 0;
	useCosineSimilarity = false;
	isDatasetFileFormatDense = false;
	datasetFilePath = "";
	datasetDimension = 0;
	datasetDimensionValueTreshold = 0;
	datasetElementsNumber = 0;
	isDatasetInternalFormatDense = false;
	referencePointsString = "";
	referencePoints.clear();
	isReferencePointFormatDense = false;
	projectionDimensions.clear();
	projectionSourceSequence.clear();
	classificationSubsetFactor = 0;
	useBinaryPlacement = false;
	pSampleIndex = 0;
	sSampleIndex = 0;
	constantVantagePointString = "";
	projectionSortingCriteriaString = "";
	projectionDimensionsString = "";
	isUseDatasetIndexAcess = false;
}

void Properties::readProperties(string propertiesFileName){

	string bufor;
	string parameterName;
	string parameterValue;
	size_t found;

	this->propertiesFileName = propertiesFileName;
	ifstream inputFileStream (propertiesFileName.c_str());

	if (!inputFileStream){
		cerr<<"Error: properties file: "<<propertiesFileName<<" could not be opened"<<endl;
	}

	/*
	 * Iterate all file text lines.
	 */
	if(inputFileStream.is_open()){
	
		while(inputFileStream.good()){
		
			getline(inputFileStream,bufor);
				
			found = bufor.find('=');
			
			if(found!=string::npos){
				parameterName = bufor.substr(0,found);
				parameterValue = bufor.substr(found+1,bufor.size());

				if(parameterName==ALGORITHM_TYPE_PARAMETER_NAME){
					
					algorithmType = parameterValue;
				}
				else
				if(parameterName==ALGORITHM_NAME_PARAMETER_NAME){

					algorithmName = parameterValue;
					algorithmNameId = getAlgorithmNameId(algorithmName);
					algorithmGroup = getAlgorithmGroupName(algorithmName);
					algorithmGroupId = getAlgorithmGroupNameId(algorithmGroup);
					if(algorithmType == "")
						algorithmType = getAlgorithmType(algorithmName);
				}
				else
				if(parameterName==EPS_PARAMETER_NAME){

					eps = strtod(parameterValue.c_str(), NULL);
				}
				else
				if(parameterName==MIN_PTS_PARAMETER_NAME){

					minPts = strtol(parameterValue.c_str(), NULL, 10);
				}
				else
				if(parameterName==K_PARAMETER_NAME){

					k = strtol(parameterValue.c_str(), NULL, 10);
				}
				else
				if(parameterName==USE_COSINE_SIMILARITY_PARAMETER_NAME){

					if(parameterValue==TRUE){
					
						useCosineSimilarity = true;
					}
					else
						if(parameterValue==FALSE){
						
							useCosineSimilarity = false;
					}
				}
				else
				if(parameterName==IS_DATASET_FILE_FORMAT_DENSE_PARAMETER_NAME){

					if(parameterValue==DENSE){
					
						isDatasetFileFormatDense = true;
					}
					else
						if(parameterValue==SPARSE){
						
							isDatasetFileFormatDense = false;
					}
				}
				else
				if(parameterName==DATASET_FILE_PATH_PARAMETER_NAME){

					datasetFilePath = parameterValue;
				}
				else
				if(parameterName==DATASET_DIMENSION_PARAMETER_NAME){

					datasetDimension = strtol(parameterValue.c_str(), NULL, 10);
				}
				else
				if(parameterName==DATASET_DIMENSION_VALUE_TRESHOLD_PARAMETER_NAME){

					datasetDimensionValueTreshold = strtol(parameterValue.c_str(), NULL, 10);
				}
				else
				if(parameterName==DATASET_ELEMENTS_NUMBER_PARAMETER_NAME){

					datasetElementsNumber = strtol(parameterValue.c_str(), NULL, 10);
				}
				else
				if(parameterName==IS_DATASET_INTERNAL_REPRESENTATION_DENSE_PARAMETER_NAME){
					
					if(parameterValue==DENSE){
					
						isDatasetInternalFormatDense = true;
					}
					else
						if(parameterValue==SPARSE){
						
							isDatasetInternalFormatDense = false;
					}
				}
				else					
				if(parameterName==REFERENCE_POINT_PARAMETER_NAME){

					referencePointsString = parameterValue;
				}
				else
				if(parameterName==IS_REFERENCE_POINT_FORMAT_DENSE_PARAMETER_NAME){
					
					if(parameterValue==DENSE){
					
						isReferencePointFormatDense = true;
					}
					else
						if(parameterValue==SPARSE){
						
							isReferencePointFormatDense = false;
					}
				}
				else
				if(parameterName==PROJECTION_DIMENSIONS_PARAMETER_NAME){
					
					size_t size = parameterValue.size();
					size_t begin = 0;
					size_t end;

					projectionDimensionsString = parameterValue;

					while(1){
					
						end = parameterValue.find(',', begin);
						
						if(end==string::npos){
							projectionDimensions.push_back(strtol(parameterValue.substr(begin,size - begin).c_str(), NULL, 10));
							break;
						}
						else{
							projectionDimensions.push_back(strtol(parameterValue.substr(begin,end - begin).c_str(), NULL, 10));
							begin = end + 1;
						}
					}
				}
				else
				if(parameterName==PROJECTION_SOURCE_SEQUENCE_PARAMETER_NAME){
					size_t size = parameterValue.size();
					size_t begin = 0;
					size_t end;					

					projectionSortingCriteriaString = parameterValue;

					while(1){
					
						end = parameterValue.find(',', begin);
						
						if(end==string::npos){
							projectionSourceSequence.push_back(pair<char, unsigned long>(parameterValue.at(begin),strtol(parameterValue.substr(begin+1,size - begin).c_str(), NULL, 10)));
							break;
						}
						else{
							projectionSourceSequence.push_back(pair<char, unsigned long>(parameterValue.at(begin),strtol(parameterValue.substr(begin+1,end - begin).c_str(), NULL, 10)));
							begin = end + 1;
						}
					}
				}
				else
				if(parameterName==CLASSIFICATION_SUBSET_FACTOR_PARAMETER_NAME){

					classificationSubsetFactor = strtol(parameterValue.c_str(), NULL, 10);
				}
				else
				if(parameterName==USE_BINARY_PLACEMENT_PARAMETER_NAME){

					if(parameterValue == TRUE){
					
						useBinaryPlacement = true;
					}
					else
						if(parameterValue == FALSE){
					
							useBinaryPlacement = false;
						}
				}
				else
				if(parameterName==P_SAMPLE_INDEX_PARAMETER_NAME){

					pSampleIndex = strtol(parameterValue.c_str(), NULL, 10);
				}
				else
				if(parameterName==S_SAMPLE_INDEX_PARAMETER_NAME){

					sSampleIndex = strtol(parameterValue.c_str(), NULL, 10);
				}
				else
				if(parameterName==SEARCH_METHOD_PARAMETER_NAME){

					searchMethod = parameterValue;
				}
				else
				if(parameterName==IS_USE_DATASET_USE_INDEX){

					if(parameterValue==DENSE){
					
						isUseDatasetIndexAcess = true;
					}
					else
						if(parameterValue==SPARSE){
						
							isUseDatasetIndexAcess = false;
					}
				}
			}
		}

		inputFileStream.close();
	}
}

void Properties::printReferencePoint(ofstream& os){
	
	map<string,int> printedReferencePointsDefinitions;
	vector<string> referencePointsDefinitions = parseReferencePointsString();
	unsigned long size = this->referencePoints.size();

	for(unsigned long i = 0; i < size; i++){
	
		if(printedReferencePointsDefinitions.count(referencePointsDefinitions[i]) != 1){
		
			os<<referencePointsDefinitions[i]<<endl;
			this->printPoint(this->referencePoints[i], os);
			printedReferencePointsDefinitions.insert(pair<string,int>(referencePointsDefinitions[i], 0));
			os<<endl;
			os<<endl;
		}
	}
}

void Properties::print(ofstream& os){
	
	string useCosineSimilarityValue = this->useCosineSimilarity ? "true" : "false";
	string datasetFileFormatValue = this->isDatasetFileFormatDense ? "dense" : "sparse";
	string datasetInternalFormatValue = this->isDatasetInternalFormatDense ? "dense" : "sparse";
	string datasetReferencePointFormatValue = this->isReferencePointFormatDense ? "dense" : "sparse";
	string useBinaryPlacementValue = this->useBinaryPlacement ? "true" : "false";


	os<<"********************************************"<<endl;
	os<<"               Properties"<<endl;
	os<<"********************************************"<<endl;
	os<<endl;
	os<<endl;
	os<<endl;
	os<<endl;
	os<<"/*Input dataset settings*/"<<endl;
	os<<"dataset_file_format               = "<<datasetFileFormatValue<< endl;
	os<<"dataset_file_path                 = "<<this->datasetFilePath<< endl;
	os<<"dataset_dimension                 = "<<this->datasetDimension<< endl;
	os<<"dataset_dimension_value_threshold = "<<this->datasetDimensionValueTreshold<< endl;
	os<<"dataset_elements_number           = "<<this->datasetElementsNumber<< endl;
	os<<endl;
	os<<endl;
	os<<"/*Internal data representation settings*/"<<endl;
	os<<"dataset_internal_format           = "<<datasetInternalFormatValue<< endl;
	os<<endl;
	os<<endl;
	os<<"/*Cosine similarity settings*/"<<endl;
	os<<"use_cosine_similarity             = "<<useCosineSimilarityValue<< endl;
	os<<endl;
	os<<endl;
	os<<"/*Algorithm settings*/"<<endl;
	os<<"algorithm_type                    = "<<this->algorithmType<< endl;
	os<<"algorithm_name                    = "<<this->algorithmName<< endl;
	os<<endl;
	os<<endl;
	
	switch(this->algorithmNameId){
	
		case 1:	//DBSCAN	
			
			os<<"/*Neighborhood settings*/"<<endl;
			os<<"eps                               = "<<this->eps<< endl;
			os<<"min_pts                           = "<<this->minPts<< endl;
			break;
		
		case 2:	//TI-DBSCAN
			
		case 3:	//TI-DBSCAN-REF

			os<<"/*Neighborhood settings*/"<<endl;
			os<<"eps                               = "<<this->eps<< endl;
			os<<"min_pts                           = "<<this->minPts<< endl;
			os<<endl;
			os<<endl;
			os<<"/*Reference point settings*/"<<endl;
			os<<"reference_point_format            = "<<datasetReferencePointFormatValue<< endl;
			os<<"reference_point                   = "<<this->referencePointsString<< endl;
			os<<endl;
			os<<endl;
			os<<"/*Reference point value*/"<<endl;
			os<<endl;
			printReferencePoint(os);
			break;
		case 4:	//TI-DBSCAN-REF-PROJECTION

			os<<"/*Neighborhood settings*/"<<endl;
			os<<"eps                               = "<<this->eps<< endl;
			os<<"min_pts                           = "<<this->minPts<< endl;
			os<<endl;
			os<<endl;			
			os<<"/*Projection settings*/"<<endl;
			os<<"projection_dimensions             = "<<projectionDimensionsString<<endl;
			os<<"projection_source_sequence        = "<<projectionSortingCriteriaString<<endl;
			os<<endl;
			os<<endl;
			os<<"/*Reference point settings*/"<<endl;
			os<<"reference_point_format            = "<<datasetReferencePointFormatValue<< endl;
			os<<"reference_point                   = "<<this->referencePointsString<< endl;
			os<<endl;
			os<<endl;
			os<<"/*Reference point value*/"<<endl;
			os<<endl;
			printReferencePoint(os);
			break;
		case 5:	//K-NEIGHBORHOOD
			
			os<<"/*Classification*/"<<endl;
			os<<"classification_subset_factor      = "<<this->classificationSubsetFactor<<endl;
			os<<"use_binary_placement              = "<<useBinaryPlacementValue<<endl;
			os<<endl;
			os<<endl;
			os<<"/*Neighborhood settings*/"<<endl;
			os<<"k                                 = "<<this->k<< endl;
			break;		
		case 6:	//TI-K-NEIGHBORHOOD
	
		case 7:	//TI-K-NEIGHBORHOOD-REF
			
			os<<"/*Classification*/"<<endl;
			os<<"classification_subset_factor      = "<<this->classificationSubsetFactor<<endl;
			os<<"use_binary_placement              = "<<useBinaryPlacementValue<<endl;
			os<<endl;
			os<<endl;
			os<<"/*Neighborhood settings*/"<<endl;
			os<<"k                                 = "<<this->k<< endl;
			os<<endl;
			os<<endl;
			os<<"/*Reference point settings*/"<<endl;
			os<<"reference_point_format            = "<<datasetReferencePointFormatValue<< endl;
			os<<"reference_point                   = "<<this->referencePointsString<< endl;
			os<<endl;
			os<<endl;
			os<<"/*Reference point value*/"<<endl;
			os<<endl;
			printReferencePoint(os);
			break;	
		case 8:	//TI-K-NEIGHBORHOOD-REF-PROJECTION
			
			os<<"/*Classification*/"<<endl;
			os<<"classification_subset_factor      = "<<this->classificationSubsetFactor<<endl;
			os<<"use_binary_placement              = "<<useBinaryPlacementValue<<endl;
			os<<endl;
			os<<endl;
			os<<"/*Neighborhood settings*/"<<endl;
			os<<"k                                 = "<<this->k<< endl;
			os<<endl;
			os<<endl;
			os<<"/*Projection settings*/"<<endl;
			os<<"projection_dimensions             = "<<projectionDimensionsString<<endl;
			os<<"projection_source_sequence        = "<<projectionSortingCriteriaString<<endl;			
			os<<endl;
			os<<endl;
			os<<"/*Reference point settings*/"<<endl;
			os<<"reference_point_format            = "<<datasetReferencePointFormatValue<< endl;
			os<<"reference_point                   = "<<this->referencePointsString<< endl;
			os<<endl;
			os<<endl;
			os<<"/*Reference point value*/"<<endl;
			os<<endl;
			printReferencePoint(os);
			break;	
		case 9: //VP-TREE

			if(this->algorithmType == Properties::CLASSIFICATION){
			
				os<<"/*Classification*/"<<endl;
				os<<"classification_subset_factor      = "<<this->classificationSubsetFactor<<endl;
				os<<endl;
				os<<endl;
				os<<"/*Neighborhood settings*/"<<endl;
				os<<"k                                 = "<<this->k<< endl;
				os<<endl;
				os<<endl;
			}
			else{
			
				os<<"/*Neighborhood settings*/"<<endl;
				os<<"eps                               = "<<this->eps<< endl;
				os<<endl;
				os<<endl;
				os<<"/*VPS-Tree settings*/"<<endl;
				os<<"p_sample_index                    = "<<this->pSampleIndex<<endl;
				os<<"s_sample_index                    = "<<this->sSampleIndex<<endl;
				os<<"search_method                     = "<<this->searchMethod<<endl;
			}
			break;
		
		default:
			break;
	}
}

void Properties::printPoint(Point& point, ofstream& os){

	unsigned long size;
	bool printed = false;
	
	vector<SparsePoint>::iterator it;
	vector<SparsePoint>::iterator end;

	os<<"\tid\t\t= "<<point.id<<endl;
	os<<"\t\tsparse point definition\t=\t";
	
	if(point.isDense){
	
		size = point.denseFormatPoint.size();
		
		for(unsigned long i = 0; i<size; i++){
			
			if(point.denseFormatPoint[i] != 0){
				
				os<<i+1<<" "<<point.denseFormatPoint[i]<<" ";
			}
		}

		
		
	}
	else{
		
		size = point.sparseFormatPoint.size();
		
		for(unsigned long i = 0; i<size; i++){
				
			os<<point.sparseFormatPoint[i].dimension<<" "<<point.sparseFormatPoint[i].value<<" ";
		}
	}

	os<<endl;

	os<<"\t\tdense point definition\t=\t";

	if(point.isDense){
	
		size = point.denseFormatPoint.size();
		
		for(unsigned long i = 0; i<size; i++){
							
			os<<point.denseFormatPoint[i]<<" ";
		}				
	}
	else{
		
		size = this->datasetDimension;
		it = point.sparseFormatPoint.begin();
		end = point.sparseFormatPoint.end();

		for(unsigned long i = 0; i<size; i++){
							
			if( (it != end) && (i+1 == it->dimension)){
				
				os<<it->value<<" ";
				it++;
			}
			else{
				os<<"0 ";
			}
		}			
	}
}

vector<string> Properties::parseReferencePointsString(){

	vector<string> result;
	string buffer;
	char c;
	unsigned long size = this->referencePointsString.size();
	bool stop = false;
	
	for(unsigned long i = 0; i< size; i++){
	
		c = this->referencePointsString[i];
		
		if((c != '[')||(i == 0)){
		
			buffer.push_back(c);
		}
		else{
		
			result.push_back(buffer);
			buffer.clear();
			buffer.push_back(c);
		}
	}

	result.push_back(buffer);

	return result;
}
