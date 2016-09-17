#ifndef ROIS_H
#define ROIS_H
#include "ObservableData.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;

class ROI{
public:
	//vars
	Size frameSize;
	int expansion = 10;
	string ROIType = "static";
	vector<Rect> currentROI;
	vector<Rect> pastROI;
	const string typesArray[3];

	//constructor
	ROI(Size s);
	ROI(Size s, ObsData& obsData);

	
	
	//functions
	vector<Rect> getROI();
	int setROI(vector<Rect>objects, double time, Size s);
	//void staticROI(vector<Rect>objects, Size s);
	void dyamicROI(vector<Rect>objects, Size s);

private:
	ObsData obsData;

	void dynamicRecenterROI(vector<Rect>& objects, double time, Size& s);
	void staticROI(vector<Rect>& objects, Size& s);
};



#endif