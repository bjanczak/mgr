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

using namespace std;

int main(){

	AlgorithmsEngine algorithmsEngine = AlgorithmsEngine::getInstance();
	algorithmsEngine.run();

	return 0;
}



