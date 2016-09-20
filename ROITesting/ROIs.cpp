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
// expand rectangle r by amount s. s is applied to each side. so if 10 is given for s, the rectangle will grow by 20.
void ROI::expandRect(Rect& r, int s, Size frameSize){
	r = Rect(
		r.x - s,
		r.y - s,
		min(r.width + s * 2, abs(frameSize.width - r.x)),
		min(r.height + s * 2, abs(frameSize.height - r.y))
		);
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
/* Rescan the entire image
*/
int DynamicRentering::fallback(vector<Rect>&objects, Size s){
	objects.clear();
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
/* Fallsback to Rescan the entire image
*/
int StaticRentering::fallback(vector<Rect>&objects, Size s){ 
	objects.clear(); 
	return 1;
}

/*Expanding Box resizes the box if an object was not able to be found within the ROI the first time.
*/
int ExpandingBox::setROI(vector<Rect>objects, double time, Size s){
	

	return 1;
}
/* Expands the box again.
*/
int ExpandingBox::fallback(vector<Rect>&objects, Size s){
	for (int i = 0; i < objects.size(); i++){
		expandRect(objects[i], 50, s);
	}
	return 1;
}

/*BlockMatching 
*/
int BlockMatching::setROI(vector<Rect>objects, double time, Size s){
	
	return 1;
}
/* Rescans the image?
*/
int BlockMatching::fallback(vector<Rect>&objects, Size s){ 
	objects.clear();
	return 1; 
}

/*Kalman Predictive expands on the recentering method by using Kalman filtering to predict the movement of the object and moving the center of the ROI 
respectively. if the prediction was incorrect, the entire image needs to be searched again.
*/
int KalmanPredictive::setROI(vector<Rect>objects, double time, Size s){
	
	return 1;
}
/* Rescan the image?
*/
int KalmanPredictive::fallback(vector<Rect>&objects, Size s){
	objects.clear();
	return 1;
}
/*Monte Carlo Predictive expands on the recentering method by using Monte Carlo Filtering to predict the movement of the object and moving the center of the ROI
respectively. if the prediction was incorrect, the entire image needs to be searched again.
*/
int MonteCarloPredictive::setROI(vector<Rect>objects, double time, Size s){
	
	return 1;
}
/* Rescan the image?
*/
int MonteCarloPredictive::fallback(vector<Rect>&objects, Size s){
	objects.clear();
	return 1;
}



