
#ifndef ROIs_H
#define ROIs_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

void staticROI();

void dyamicROI(Rect rec, enum type);

#endif