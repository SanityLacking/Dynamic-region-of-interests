
#ifndef MEASUREMENTSUITE_H
#define MEASUREMENTSUITE_H


#include <time.h> // to calculate time needed
#include <limits.h> // to get INT_MAX, to protect against overflow

#include "stdafx.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;

#include <iostream>
using std::string;

class MeasureTool{
public:
	// fps counter begin
	time_t tstart, tend;
	int counter = 0;
	double sec;
	double fps;
	// fps counter end
	//MeasureTool();//constructor
	double getFPS();
	void start();
	void end();
	string outputResults();
	void updateStats(vector<Rect>PastRoi);

private:
	int frameCounter = 0;
	vector<double> frameProcessTime;
	vector<int> ROIsFound;
	vector<vector <Rect> > PastROIs;
	
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