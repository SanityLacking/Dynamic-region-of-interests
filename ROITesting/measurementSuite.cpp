
#include "measurementSuite.h"


#include <time.h> // to calculate time needed
#include <limits.h> // to get INT_MAX, to protect against overflow

#include "stdafx.h"
#include <iostream>
using std::string;


MeasureTool::MeasureTool(){
	//QueryPerformanceFrequency(&timeFrequency);
}
void MeasureTool::start(){
	//QueryPerformanceCounter(&timeStart);
}

double MeasureTool::end(){
	//QueryPerformanceCounter(&timeEnd);
	counter++;
	//LARGE_INTEGER second = { 1 };
	//sec = (timeEnd.QuadPart - timeStart.QuadPart);
	//fps = (timeEnd.QuadPart - timeStart.QuadPart) / (double)timeFrequency.QuadPart;
	//double scale = 0.01;  // i.e. round to nearest one-hundreth
	//fps = floor(fps / scale + 0.5) * scale;
	
	return sec;
}
double MeasureTool::getFPS(){
	return fps;
}

//private function that updates the stats that the measure tool class stores.
