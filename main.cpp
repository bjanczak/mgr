#include "Algorithm.h"
#include "AlgorithmsEngine.h"
#include "Dataset.h"
#include "Dbscan.h"
#include "KNeighborhood.h"
#include "Properties.h"
#include "TiDbscan.h"
#include "TiDbscanRef.h"
#include "TiDbscanRefProjection.h"
#include "TiKNeighborhood.h"
#include "TiKNeighborhoodRef.h"
#include "TiKNeighborhoodRefProjection.h"
#include "Utils.h"
#include "VpsTree.h"
#include "VpTree.h"

#include <fstream>
#include <time.h>
#include <math.h>

#define NEW_MAIN
//#define OLD_MAIN


using namespace std;

#ifdef NEW_MAIN

int main(){

	AlgorithmsEngine algorithmsEngine = AlgorithmsEngine::getInstance();
	algorithmsEngine.run();

	return 0;
}

#endif

#ifdef OLD_MAIN

int main(){


	ofstream reportFile;
	string reportName = Utils::reportNameGenerator();
	Properties properties = Properties::getInstance();
		
	reportFile.open(reportName);

	/*
	 * Classification can not be done with DBSCAN algorithms.
	 */
	if((properties.algorithmGroup == Properties::DBSCAN) && properties.algorithmType == Properties::CLASSIFICATION){
	
		properties.print(reportFile);
		reportFile<<endl;
		reportFile<<endl;
		reportFile<<endl;
		reportFile<<endl;
		reportFile<<"********************************************"<<endl;
		reportFile<<"                   Error"<<endl;
		reportFile<<"********************************************"<<endl;
		reportFile<<endl;
		reportFile<<endl;
		reportFile<<endl;
		reportFile<<endl;
		reportFile<<"Classification can not be done with DBSCAN algorithm variants!"<<endl;
		reportFile.close();
		return 0;
	}

	Dataset dataset = Dataset::getInstance();
	dataset.setProperties(properties);
	clock_t normalizationStart = 0;
	clock_t normalizationFinish = 0;

	dataset.readData(properties);

	if(properties.useCosineSimilarity){
	
		properties.eps = Utils::getCosEps(properties.eps);

		normalizationStart = clock();
		
		dataset.normalize();
		
		normalizationFinish = clock();
	}
	
	properties.referencePoints = dataset.getReferencePoints(properties);
	
	TimeReport timeReport;
	Algorithm *algorithm;

#ifdef TEST
	Point zeroPoint = Dataset::getZeroPoint(dataset);
	Point minPoint = dataset.getMinPoint();
	Point maxPoint = dataset.getMaxPoint();
	Point customPoint1 = dataset.getCustomPoint("[1,1,2,2,3,3,4,4]", properties.isReferencePointFormatDense);
	Point customPoint2 = dataset.getCustomPoint("[1,min,2,max,3,0]", properties.isReferencePointFormatDense);
	Point customPoint3 = dataset.getCustomPoint("[1,0,2,0,3,max]", properties.isReferencePointFormatDense);
	properties.referencePoints = dataset.getReferencePoints(properties);
	/*ofstream myfile("example.txt");
	properties.print(myfile);
	myfile.close();*/
	double x = 1/Point::invSqrt2(2);
	double y = 1/Point::invSqrt2(4);
	double z = 1/Point::invSqrt2(9);
	double a = 1/Point::invSqrt2(25);
	double x2 = sqrt(2.0);
	double y2 = sqrt(4.0);
	double z2 = sqrt(9.0);
	double a2 = sqrt(25.0);
	/*
	Point f = dataset.datasetKNeighborhoodPoint.at(0);
	Point b = dataset.datasetKNeighborhoodPoint.at(1);
	Point c = dataset.datasetKNeighborhoodPoint.at(2);
	Point d = dataset.datasetKNeighborhoodPoint.at(3);
	Point e = dataset.datasetKNeighborhoodPoint.at(4);
	double aa = Point::minkowskiDistance(f,b,2);
	double ab = Point::minkowskiDistance(b,c,2);
	double ac = Point::minkowskiDistance(c,d,2);
	double ad = Point::minkowskiDistance(d,e,2);
	*/
	vector<Point> subset = dataset.getSample(properties);
	dataset.normalize();
#endif

	switch(properties.algorithmGroupId){
	
		case 1:

			Dataset::fillDbscanPointVector(dataset.datasetDbscanPoint, dataset.datasetPoint, dataset.isDense);
			break;
		case 5:

			Dataset::fillKNeighborhoodPointVector(dataset.datasetKNeighborhoodPoint, dataset.datasetPoint, dataset.isDense);
			break;
	}

	switch(properties.algorithmNameId){
	
		case 1:		
			algorithm = &Dbscan();
			break;
		
		case 2:
			algorithm = &TiDbscan();
			break;
		
		case 3:
			algorithm = &TiDbscanRef();
			break;
		
		case 4:
			algorithm = &TiDbscanRefProjection();
			break;
		
		case 5:
			algorithm = &KNeighborhood();
			break;
		
		case 6:
			algorithm = &TiKNeighborhood();
			break;

		case 7:		
			algorithm = &TiKNeighborhoodRef();
			break;
		
		case 8:
			algorithm = &TiKNeighborhoodRefProjection();
			break;
		
		case 9:
			algorithm = &VpTree();
			break;
		
		case 10:
			algorithm = &VpsTree();
			break;
		
		default:
			algorithm = &Algorithm();
			break;
	}

	if(properties.algorithmType == Properties::GROUPING){
	
		
	}
	else
		if(properties.algorithmType == Properties::CLASSIFICATION){
			
			Point* nullPointer;
			vector<Point> sample = Dataset::generateSample(dataset.datasetPoint, properties);
			vector<Point>::iterator sampleIt;
			vector<Point>::iterator sampleEnd = sample.end();

			for(sampleIt = sample.begin();sampleIt != sampleEnd; sampleIt++){
			
				dataset.classificationDataset.push_back(pair<Point, Point*>(*sampleIt, nullPointer));
			}
		}
	
	timeReport = algorithm->run(properties, dataset);

	if(normalizationStart != normalizationFinish){

		timeReport.normalizingDatasetExecutionTime = ((double)(normalizationFinish - normalizationStart))/CLOCKS_PER_SEC;
	}
	
	if(reportFile.is_open()){
		
		properties.print(reportFile);
		timeReport.print(reportFile);
		dataset.printClusteringSumUp(reportFile);
	
		reportFile.close();
	}
}

#endif

