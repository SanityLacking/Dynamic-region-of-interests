
#ifndef ROIs_H
#define ROIs_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;

class ROI{
public:
	vector<Rect>pastROI;

	vector<Rect>setROI(vector<Rect>objects);
	
	void staticROI();

	void dyamicROI(Rect rec, enum type);

};



#endif