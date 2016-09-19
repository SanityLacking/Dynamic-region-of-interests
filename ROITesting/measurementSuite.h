#pragma once
#ifndef MEASUREMENTSUITE_H
#define MEASUREMENTSUITE_H

 #include "stdafx.h"

#include <time.h> // to calculate time needed
#include <limits.h> // to get INT_MAX, to protect against overflow
#include <iostream>
#include <vector>

using namespace std;
class MeasureTool{
public:

	MeasureTool();
	

	// fps counter begin
	time_t tstart, tend;
	int counter = 0;
	double sec;
	double fps;
	// fps counter end
	//MeasureTool();//constructor
	double getFPS();
	void start();
	double end();
	string outputResults();

private:
	int frameCounter = 0;
	std::vector<double> frameProcessTime;
	std::vector<int> ROIsFound;
	
};

/*
void MeasureTool::Iterate(){
	time(&end);
	counter++;
	sec = difftime(end, start);
	fps = counter / sec;
	if (counter > 30)

		// overflow protection
		if (counter == (INT_MAX - 1000))
			counter = 0;
	// fps counter end

}
*/
#endif
