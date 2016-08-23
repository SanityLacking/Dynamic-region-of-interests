#include "stdafx.h"
#include "ROIs.h"
using namespace std;

/*
Types of Regions of Interests are defined here and can be called from outside the class.
*/


void ROI::setROI(vector<Rect>objects){
	pastROI.clear();
	for (int i = 0; i < objects.size(); i++){
		pastROI.push_back(objects[i]);
	}
}


void ROI::staticROI(){

}

void ROI::dyamicROI(Rect rec, enum type){

}
