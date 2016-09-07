#include "measurementSuite.h"
#include <time.h> // to calculate time needed
#include <limits.h> // to get INT_MAX, to protect against overflow

#include "stdafx.h"
#include <iostream>
using std::string;


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

//private function that updates the stats that the measure tool class stores.

void MeasureTool::updateStats(vector<Rect> pastRoi){
	frameCounter++;
	frameProcessTime.push_back(sec);
	ROIsFound.push_back(pastRoi.size());
	PastROIs.push_back(pastRoi);

}

string MeasureTool::outputResults(){
	string output = "";
	for (int i = 0; i < ROIsFound.size(); i++){
		output += "" + to_string(i) + "," + to_string(ROIsFound[i]) + ",";
		if (PastROIs[i].size() > 0){
			string loc; 
			//when decoding this part, delimit by a space " ", then split each one by "_". 
			//this will give you the (x,y) and (width,height) for each of the ROI found in that frame.
			for (int j = 0; j < PastROIs[i].size(); j++){
				loc += to_string(PastROIs[i][j].x) + "_" + to_string(PastROIs[i][j].y)+" ";
			}
			loc += ",";
			for (int j = 0; j < PastROIs[i].size(); j++){
				loc += to_string(PastROIs[i][j].width) + "_" + to_string(PastROIs[i][j].height)+" ";
			}
			output += loc;
		}
		output += "," + to_string(frameProcessTime[i]);
		output += "\n";
	}
	return output;

}

