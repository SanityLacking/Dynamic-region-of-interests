#include "stdafx.h"
#include "ROIs.h"
#include "ObservableData.h"
#include <iostream>
using namespace std;

/*
Types of Regions of Interests are defined here and can be called from outside the class.
*/
ROI::ROI(Size s, ObsData& obs): obsData(obs) {
	expansion = 20;
}

ROI::ROI(Size s): obsData(DefaultObsData){
	//frameSize = s;
	expansion = 20;
}
vector<Rect> ROI::getROI(){
	return obsData.getPast();
}

/* Dynamic Recenter ROI: each time an object is detected the ROI draws itself around the object. 
*/
int DynamicRentering::setROI(vector<Rect>objects, double time, Size s){
	outputROI.clear();
	for (int i = 0; i < objects.size(); i++){
		Rect r = Rect(
			objects[i].x - expansion,
			objects[i].y - expansion,
			min(objects[i].width + expansion * 2, abs(frameSize.width - objects[i].x)),
			min(objects[i].height + expansion * 2, abs(frameSize.height - objects[i].y))
			);
		outputROI.push_back(r);
	}
	obsData.set(outputROI, time);
	return 1;
}

/*Static ROI, does not recenter the ROI over the object if it has moved slightly but is still detected. Most basic implementation.
do not clear the past roi until we check to see if a current face was detected within it.
*/
int StaticRentering::setROI(vector<Rect>objects, double time, Size s){
	outputROI.clear();
	//loop through all past ROI and try to fit an object to it. if you can, keep the pastROI.
	for (int i = 0; i < pastROI.size(); i++){
		bool keepROI = false;
		for (int j = 0; j < objects.size(); j++){
			if (objects[j].x >= pastROI[i].x && objects[j].x < pastROI[i].x + pastROI[i].width && objects[j].y >= pastROI[i].y && objects[j].y < pastROI[i].y + pastROI[i].height)
				keepROI = true;
			break;
		}
		if (keepROI != true){
			outputROI.push_back(pastROI[i]);
		}
	}
	//loop through all objects and see if the object is within an ROI. if not add a new ROI for it.
	for (int j = 0; j < objects.size(); j++){
		bool newObj = true;
		for (int i = 0; i < pastROI.size(); i++){
			if (objects[j].x >= pastROI[i].x && objects[j].x < pastROI[i].x + pastROI[i].width && objects[j].y >= pastROI[i].y && objects[j].y < pastROI[i].y + pastROI[i].height)
				newObj = false;
		}
		if (newObj == true){
			Rect r = Rect(
				objects[j].x - expansion,
				objects[j].y - expansion,
				min(objects[j].width + expansion * 2, abs(frameSize.width - objects[j].x)),
				min(objects[j].height + expansion * 2, abs(frameSize.height - objects[j].y))
				);
			outputROI.push_back(r);
		}
	}

	for (int i = 0; i < objects.size(); i++){
		Rect r = Rect(
			objects[i].x - expansion,
			objects[i].y - expansion,
			min(objects[i].width + expansion * 2, abs(frameSize.width - objects[i].x)),
			min(objects[i].height + expansion * 2, abs(frameSize.height - objects[i].y))
			);
		pastROI.push_back(r);
	}
	return 1;
}