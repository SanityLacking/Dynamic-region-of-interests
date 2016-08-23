// ROITesting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Eyes.h"
#include "ROIs.h"
#include "measurementSuite.h"
#include "faceDetection.h"
#include <time.h> // to calculate time needed
#include <limits.h> // to get INT_MAX, to protect against overflow

using namespace cv;
using namespace std;
FaceDetect faceDetect2;
int displayCamera(VideoCapture& camera);
void processImage(Mat& frame, ROI& roi, MeasureTool mTool, FaceDetect faceDetect);
Point frameP(Point resize);

int _tmain(int argc, _TCHAR* argv[]){
	VideoCapture camera;
	if (!camera.open(0)){ 
		cout << "Camera Is not able to be Opened, is it connected?" << endl; 
		return 0;
	}
	displayCamera(camera);
	return 0;
}

int displayCamera(VideoCapture& camera){
	Mat frame;
	vector<Rect> rec;
	MeasureTool mTool;
	ROI roi;
	FaceDetect faceDetect;
	for (;;){
		camera >> frame;
		mTool.start();//fps counter start
		processImage(frame, roi, mTool,faceDetect);
		mTool.end();// fps counter end
		
		putText(frame, "fps: " + to_string(mTool.getFPS()), Point(5, 15), FONT_HERSHEY_PLAIN, 1.2, Scalar(0, 0, 255, 255), 2);
		imshow("output", frame);
		if (waitKey(1) == 27) break;
	}
	return 0;
}

//do the preliminary processing in this function, this function calls specific processing functions as well.
void processImage(Mat& frame, ROI& roi, MeasureTool mTool, FaceDetect faceDetect){	
	Size frameSize = frame.size();
	Mat processImg = frame;
	vector<Rect> objects;
	
	//preprocess Blur, color correct, etc	

	for (int i = 0; i < roi.pastROI.size(); ++i){
		rectangle(frame, roi.pastROI[i], Scalar(0, 155, 255),2);
	}


	//call Detection Method
	objects = faceDetect.detectFaces(processImg, roi.pastROI);
	// Draw Detections

	for (int i = 0; i < objects.size(); ++i){
		//rectangle(frame, objects[i], Scalar(255, 255, 255));
	}

	cout << "new ROIs" << objects.size() << endl;
	//Set new ROI
	roi.setROI(objects);

	//store results if need be.	
}

/*
	detection comparasion
	for each run, we want to compare the success of the detection method and any speed improvement by using an ROIs method.

	To do this 
	-> measure the amount of succesful detections (accuracy)
	-> measure the amount of time per detection (performance)
	-> measure the amount of correctly located detected objects (precision)

	The setup will need to run through the test, display real time results, display results on end, and store results.
	FPS

	Flags I need:
	Measure: T/F
	Save Results: T/F
*/

// convert point generated for resized frame to the correct position for the original frame.
//Point frameP(Point resize){
	//return Point(resize.x * (frameSize.width / frameWidth), resize.y * (frameSize.height / frameHeight));
//}