#ifndef ObservableData_H
#define ObservableData_H
#include <vector>
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;
//very simple POCO? class that accepts data in and stores it, and allows the current, and most recent data to be read out.
class obsData{
public:
	//constructor
	obsData();
	//functions
	void set(vector<Rect> objects, Size s);
	vector<Rect>  getCurrent();
	vector<Rect>  getPast();
private:
	Size frameSize;
	vector<Rect> currentROI;
	vector<Rect> pastROI;
	void store(vector<Rect>objects);
};
#endif