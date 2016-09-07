#include "stdafx.h"
#include "ROIs.h"
#include <iostream>
using namespace std;

/*
Types of Regions of Interests are defined here and can be called from outside the class.
*/
ROI::ROI(Size s){
	//frameSize = s;
	expansion = 20;
}

void ROI::setROI(vector<Rect>objects,Size s){
	frameSize = s;
	pastROI.clear();
	for (int i = 0; i < objects.size(); i++){
		cout << "X: " << max(objects[i].x - expansion, 0) << " Y: " << max(objects[i].y - expansion, 0) <<endl;
		cout << "width: " << min(objects[i].width + expansion * 2, abs(frameSize.width - objects[i].x )) << " Height: " << min(objects[i].height + expansion * 2, abs(frameSize.height -objects[i].y)) << endl;
		Rect r = Rect(
			objects[i].x - expansion,
			objects[i].y - expansion,
			min(objects[i].width + expansion * 2, abs(frameSize.width - objects[i].x)),
			min(objects[i].height + expansion * 2, abs(frameSize.height - objects[i].y))
			);
		pastROI.push_back(r);
	}
}


void ROI::staticROI(){

}

void ROI::dyamicROI(Rect rec, enum type){

}

