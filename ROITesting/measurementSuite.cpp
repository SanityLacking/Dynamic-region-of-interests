#include "measurementSuite.h"
#include <time.h> // to calculate time needed
#include <limits.h> // to get INT_MAX, to protect against overflow

#include "stdafx.h"


void MeasureTool::start(){
	if (counter == 0){
		time(&tstart);
	}

}

void MeasureTool::end(){
	time(&tend);

	counter++;
	sec = difftime(tend, tstart);
	fps = round(counter / sec);
	//cout << (int)fps << endl;
	//cout << tstart << " " << tend << endl;
	// overflow protection
	if (counter == (INT_MAX - 1000))
		counter = 0;
}

double MeasureTool::getFPS(){
	return fps;
}
