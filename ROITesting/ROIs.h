#ifndef ROIS_H
#define ROIS_H

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


	//constructor
	ROI(Size s);

	
	//functions
	vector<Rect> currentROI;
	vector<Rect> pastROI;
	void setROI(vector<Rect>objects,Size s);
	void staticROI();
	void dyamicROI(Rect rec, enum type);
};



#endif