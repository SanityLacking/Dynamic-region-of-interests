#include "ObservableData.h"
#include "PersistentData.h"
#include <iostream>
using namespace std;

ObsData::ObsData(persistentData& pData):persistData(pData){

}

ObsData::ObsData() : persistData(DefaultPersistData) {
	
}


void ObsData::set(const vector<Rect>& objects, const double& t){
	pastROI.clear();
	pastTime = t;
	for (int i = 0; i < objects.size(); i++){
		pastROI.push_back(objects[i]);
	}
	store();
	cout << pastROI.size() << " ROIs saved" << endl;
}
/*
vector<Rect>  obsData::getCurrent(){
	return pastROI;
}
*/
vector<Rect> ObsData::getPast(){
	return pastROI;
}

void ObsData::store(){
	//cout << "ObsData: " << persistData.currentData.size() << endl;
	persistData.store(pastROI,pastTime);
}


