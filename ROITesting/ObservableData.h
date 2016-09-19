#ifndef ObservableData_H
#define ObservableData_H


#include "PersistentData.h"
#include <vector>
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;
//very simple POCO? class that accepts data in and stores it, and allows the current, and most recent data to be read out.
class ObsData{
public:
	//constructor
	ObsData();
	ObsData(persistentData& persistData);

	//functions
	void set(const vector<Rect>& objects, const double& time);
	vector<Rect>  getCurrent();
	vector<Rect>  getPast();
	
	persistentData& persistData;
	
private:
	vector<Rect> currentROI;
	vector<Rect> pastROI;
	persistentData DefaultPersistData;	//if no reference to an Obs Data, the class will use this one.
	double pastTime;
	
	
	void store();
};
#endif