
#ifndef FACEDETECTION_H
#define FACEDETECTION_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;

vector<Rect> detectFaces(Mat input, vector<Rect>pastROI);


#endif

