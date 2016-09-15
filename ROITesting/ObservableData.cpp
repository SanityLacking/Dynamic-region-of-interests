#include "ObservableData.h"
#include <iostream>
using namespace std;

obsData::obsData(){

}

void obsData::set(vector<Rect>objects, Size s){
	frameSize = s;
	pastROI.clear();
	for (int i = 0; i < objects.size(); i++){
		pastROI.push_back(objects[i]);
	}
}
/*
vector<Rect>  obsData::getCurrent(){
	return pastROI;
}
*/
vector<Rect> obsData::getPast(){
	return pastROI;
}

void obsData::store(vector<Rect>objects){

}


