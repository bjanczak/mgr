/*
 ============================================================================
 Name        : Utils.cpp
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-20
 Version     : 2.0
 Copyright   : Your copyright notice
 Description : Implementation of Utils.h
 ============================================================================
 */

#include "Utils.h"

#include <ctime>
#include <iostream>
#include <math.h>
#include <sstream>

double Utils::getCosEps(double eps){

	return sqrt(2-2*eps);
}

string Utils::reportNameGenerator(){
	
	string base = "logs/run_report_";
	string rest;

	ostringstream ss;
	time_t t = time(0);
    struct tm * now = localtime( & t );
	
	ss << (now->tm_year + 1900)<<"_";
	ss << (now->tm_mon + 1)<<"_";
	ss << (now->tm_mday)<<"_";
	ss << (now->tm_hour)<<"_";
	ss << (now->tm_min)<<"_";
	ss << (now->tm_sec);


	return base+ss.str()+".txt";
}

string Utils::ultimateReportNameGenerator(){
	
	string base = "logs/ultimate_run_report_";
	string rest;

	ostringstream ss;
	time_t t = time(0);
    struct tm * now = localtime( & t );
	
	ss << (now->tm_year + 1900)<<"_";
	ss << (now->tm_mon + 1)<<"_";
	ss << (now->tm_mday)<<"_";
	ss << (now->tm_hour)<<"_";
	ss << (now->tm_min)<<"_";
	ss << (now->tm_sec);


	return base+ss.str()+".csv";
}

double Utils::median(const vector<double>& values){

	double median;
	unsigned long size = values.size();
	bool isEven = (size%2) == 0;

	if(isEven){
	
		unsigned long firstIndex = (unsigned long)(((double)size)/2);
		unsigned long secondIndex = firstIndex - 1;

		median = (values[firstIndex] + values[secondIndex])/2;

	}
	else{

		unsigned long index = (unsigned long)(((double)(size - 1))/2);
	
		median = values[index];
	}

	return median;
}

vector<double> Utils::medianWithNeighbors(const vector<pair<double, list<VpsPoint>::iterator>>& values){
	
	unsigned long size = values.size();
	bool isEven = (size%2) == 0;
	vector<double> result;

	if(values.size() > 1){
		
		if(isEven){
	
			unsigned long firstIndex = (unsigned long)(((double)size)/2);
			unsigned long secondIndex = firstIndex - 1;

			/*
			* Median.
			*/
			result.push_back((values[firstIndex].first + values[secondIndex].first)/2);
			/*
			* Median lesser neighbor.
			*/
			result.push_back(values[secondIndex].first);
			/*
			* Median greater neighbor.
			*/
			result.push_back(values[firstIndex].first);

		}
		else{

			unsigned long index = (unsigned long)(((double)(size - 1))/2);
	
			/*
			* Median.
			*/
			result.push_back(values[index].first);
			/*
			* Median lesser neighbor.
			*/
			result.push_back(values[index - 1].first);
			/*
			* Median greater neighbor.
			*/
			result.push_back(values[index + 1].first);
		}
	}
	else{
		
			/*
			* Median.
			*/
			result.push_back(values[0].first);
			/*
			* Median lesser neighbor.
			*/
			result.push_back(DBL_MIN);
			/*
			* Median greater neighbor.
			*/
			result.push_back(DBL_MAX);
	}

	return result;
}

double Utils::variance(const vector<double>& values,const double median){

	vector<double>::const_iterator it;
	vector<double>::const_iterator end = values.end();
	double size = (double)values.size();
	double tempValue = 0;
	double result = 0;

	for(it = values.begin(); it != end; it++){
	
		tempValue = (*it) - median;
		result = result + (tempValue * tempValue);
	}

	result = sqrt(result/size);

	return result;
}

bool Utils::myComparator(const pair<double, list<VpsPoint>::iterator>& a, const pair<double, list<VpsPoint>::iterator>& b){

	return a.first < b.first;
}