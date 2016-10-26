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
	return outputROI;
	//return obsData.getPast();
}


// expand rectangle r by amount s. s is applied to each side. so if 10 is given for s, the rectangle will grow by 20.
void ROI::expandRect(Rect& r, int s, Size frameSize){
	r = Rect(
		max(r.x - s,0),
		max(r.y - s,0),
		min(r.width + s * 2, abs(frameSize.width - r.x)),
		min(r.height + s * 2, abs(frameSize.height - r.y))
		);
}

int Control::setROI(vector<Rect>objects, double time, Size s){
	obsData.set(objects, time);
	return 1;
}
int Control::fallback(vector<Rect>&objects, Size s){
	return 1;
}

/* Dynamic Recenter ROI: each time an object is detected the ROI draws itself around the object. 
*/
int DynamicRentering::setROI(vector<Rect>objects, double time, Size s){
	outputROI.clear();
	for (int i = 0; i < objects.size(); i++){
		Rect r = Rect(
			max(objects[i].x - expansion,0),
			max(objects[i].y - expansion,0),
			min(objects[i].width + expansion * 2, abs(s.width - objects[i].x -1)),
			min(objects[i].height + expansion * 2, abs(s.height - objects[i].y -1))
			);
		cout << "SetROI: " << r.x << "," << r.y << "," << r.width << "," << r.height << endl;
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
int Static::setROI(vector<Rect>objects, double time, Size s){
	if (objects.size() > 0){
		if (outputROI.size() <= 0)
			for (size_t i = 0; i < objects.size(); i++)	{
				Rect r = Rect(
					max(objects[i].x - expansion, 0),
					max(objects[i].y - expansion, 0),
					min(objects[i].width + expansion * 2, abs(s.width - objects[i].x - 1)),
					min(objects[i].height + expansion * 2, abs(s.height - objects[i].y - 1))
					);
				outputROI.push_back(r);
			} //else do nothing
	}else{ 
		// no objects detected at all
		outputROI.clear();
	}
	obsData.set(outputROI, time);
	return 1;
}

int Static::fallback(vector<Rect>&objects, Size s){
	objects.clear();
	outputROI.clear();
	return 1;
}



int StaticRentering::setROI(vector<Rect>objects, double time, Size s){
	outputROI.clear();

	//if object is detected, within the roi, 

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
	obsData.set(outputROI, time);
	return 1;
}
/* Fallsback to Rescan the entire image
*/
int StaticRentering::fallback(vector<Rect>&objects, Size s){ 
	objects.clear(); 
	return 1;
}

int StaticExpanding::setROI(vector<Rect>objects, double time, Size s){
	if (objects.size() > 0){
		if (outputROI.size() <= 0)
			for (size_t i = 0; i < objects.size(); i++)	{
				Rect r = Rect(
					max(objects[i].x - expansion, 0),
					max(objects[i].y - expansion, 0),
					min(objects[i].width + expansion * 2, abs(s.width - objects[i].x - 1)),
					min(objects[i].height + expansion * 2, abs(s.height - objects[i].y - 1))
					);
				outputROI.push_back(r);
			} //else do nothing
	}
	else{
		// no objects detected at all
		outputROI.clear();
	}
	return 1;
}
/* Fallsback to expand the ROI by expansionAmount
*/
int StaticExpanding::fallback(vector<Rect>&objects, Size s){
	//objects.clear();
	vector<Rect>output;
	for (size_t i = 0; i < objects.size(); i++)
	{
		Rect r = Rect(
			max(objects[i].x - expansionAmount, 0),
			max(objects[i].y - expansionAmount, 0),
			min(objects[i].width + expansionAmount * 2, abs(s.width - objects[i].x - 1)),
			min(objects[i].height + expansionAmount * 2, abs(s.height - objects[i].y - 1))
			);
		output.push_back(r);
	}
	objects = output;
	return 1;
}

/*Expanding Box resizes the box if an object was not able to be found within the ROI the first time.
Nothing is done in the first step, the boxes are only expanded if the objects were not detected in the first attempt.
*/
int ExpandingBox::setROI(vector<Rect>objects, double time, Size s){
	

	return 1;
}
/* Expands the box to try and find the object's new location.
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
	// check that there is a prior state to call upon. if none exists, initalize for next iteration.
	for (int i = 0; i < objects.size(); i++){// deal with how I handle tracking multiple objects somehow.
		Point prevEst;
		Point center;
		int  y, Px, Py;
		int zX = previousPrediction.x;
		double xVel, xAcel, yVel, yAcel;
		double processNoise;
		double qEst;

		// time update
		int prevX = previousPrediction.x + errMeasurement;
		double prevP = pastP + Q;

		//measurement Update
		kalmanGain = pastP / (pastP + R);
		Px = prevX + kalmanGain * (zX - prevX);
		double P = (1 - kalmanGain) * pastP;

		//store updates
		pastP = P;
		previousPrediction.x = Px;
	}


	/*2d movement.  movement in the x 
		2 equations,
		state update = prior state + input +error
		measurement update = state update + measurement error
	*/







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


