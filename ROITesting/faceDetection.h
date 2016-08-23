
#ifndef FACEDETECTION_H
#define FACEDETECTION_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Eyes.h"

using namespace cv;
using namespace std;
class FaceDetect{
public:
	cv::String eyes_cascade_name = "haarcascades/haarcascade_eye_tree_eyeglasses.xml";
	cv::String face_cascade_name = "haarcascades/haarcascade_frontalface.xml";

	CascadeClassifier face_cascade;
	CascadeClassifier eyes_cascade;

	//constructor
	FaceDetect();
	//functions
	vector<Rect> detectFaces(Mat input, vector<Rect>pastROI);
	double PointDist(Point a, Point b);
	//vector<Eyes> partnerEyes(vector<Rect>eyes);
	vector<Rect> detectEyes(Mat& frame);
	vector<Rect> detectEyes(Mat& frame, vector<Rect>& Roi);
	void trackEyes(vector<Eyes> currentEyes, vector<Eyes>& pastEyes);
	Rect frameRoi(Rect obj, Point roiPoint);
};

#endif

