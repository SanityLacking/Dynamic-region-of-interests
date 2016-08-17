
#ifndef Eyes_H
#define Eyes_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

class Eyes{
public:
	int partnerId = -1;
	Rect rect;
	Scalar colour = Scalar(255, 255, 255);
	Point center();
	Eyes(Rect, int, Scalar); //constructor
	Eyes(Rect, int); //constructor
	Eyes(Rect);
	int counter = 0;
};


#endif