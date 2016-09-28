// ROITesting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "ROIs.h"
#include "ROIFactory.h"
#include "measurementSuite.h"
#include "faceDetection.h"
#include "PersistentData.h"
#include "ObservableData.h"
#include <time.h> // to calculate time needed
#include <chrono>
#include <limits.h> // to get INT_MAX, to protect against overflow
#include <fstream>

#include <iomanip>
#include <ctime>
#include <sstream>


using namespace cv;
using namespace std;

LARGE_INTEGER timeFrequency;
LARGE_INTEGER timeStart;
LARGE_INTEGER timeEnd;
double elapsedSeconds;

bool displayBool =  true;
bool storeBool = true;
bool fileFinished = true;
string detectMethod = "face";
string roiMethod = "dynamicRecenter";
//depending on which method is run, the output string here is changed.
//eg: dynamic_recentering, static_control, dynamic_kalman.
string outputString = "dynamic_recentering";

string outputHeader = "frame, ROI found, ROI location, ROI size, frame process time \n";
int displayCamera(VideoCapture& camera);
void processImage(Mat& frame, ROI& roi, FaceDetect& faceDetect);
Point frameP(Point resize);
MeasureTool mTool;


/*todo: add parameters as options to allow for the program to be called multiple times by process.
 Parameters:
	What ROI method is to be called, 
	Should the image be shown while processing,
	File to output results to,

*/
int _tmain(int argc, _TCHAR* argv[]){
	VideoCapture camera;
	if (!camera.open("Video_1.mp4")){
		cout << "Camera Is not able to be Opened, is it connected?" << endl; 
		return 0;
	}
	displayCamera(camera);
	return 0;
}

int displayCamera(VideoCapture& camera){
	Mat frame;
	vector<Rect> rec;
	persistentData persistData;
	ObsData obsData(persistData);
	cout << "Frame Processing Start" << endl;
	ROIFactory->
	ROI *roi = ROIFactory.createROI(frame.size(), obsData);
	//DynamicRentering roi(frame.size(),obsData);
	FaceDetect faceDetect;
	QueryPerformanceFrequency(&timeFrequency);

	for (;;){
		frame.release();
		camera >> frame;
		if (frame.empty()){//the file has finished or the web camera has stopped sending frames.
			fileFinished = false;
			break;
		}
		processImage(frame, roi, faceDetect);
		if (displayBool){
			imshow("output", frame);
			if (waitKey(1) == 27) {
				fileFinished = false;
				break;
			}
		}
	}
	persistData.storeToFile(outputString,fileFinished);
	return 0;
}

//do the preliminary processing in this function, this function calls specific processing functions as well.
void processImage(Mat& frame, ROI& roi, FaceDetect& faceDetect){	
	Size frameSize = frame.size();
	Mat processImg = frame;

	vector<Rect> objects;
	//preprocess Blur, color correct, etc	
	
	//mTool.start();//fps counter start
	QueryPerformanceCounter(&timeStart);
	
	objects = faceDetect.detectFaces(processImg, roi);
	
	QueryPerformanceCounter(&timeEnd);
	LARGE_INTEGER second = { 1 };
	double sec = (timeEnd.QuadPart - timeStart.QuadPart);
	elapsedSeconds = (timeEnd.QuadPart - timeStart.QuadPart) / (double)timeFrequency.QuadPart;
	double fps = floor((1 / elapsedSeconds)/0.01 +0.5)*0.01;
	
	roi.setROI(objects, sec, frame.size());
	//mTool.end();
	/*
	else if (detectMethod == "eyes"){
		// implemented only eye detection good test of multiple ROI tracking
		objects = faceDetect.detectFaces(processImg, roi.pastROI);
	}
	else if (detectMethod == "corners"){
		// implemented corner detection. works by detecting a group of corners and determining the smallest rect that contains them.
		// remember to remove outliers.
		objects = faceDetect.detectFaces(processImg, roi.pastROI);
	}
	// Draw Detections
	*/
	for (int i = 0; i < objects.size(); ++i){
		rectangle(frame, objects[i], Scalar(255, 255, 255));
	}
	for (int i = 0; i < roi.pastROI.size(); ++i){
		rectangle(frame, roi.pastROI[i], Scalar(0, 155, 255),2);
	}

	if (displayBool){
		putText(frame, "fps: " + to_string(fps), Point(5, 15), FONT_HERSHEY_PLAIN, 1.2, Scalar(0, 0, 255, 255), 2);
	}
	//Set new ROI
	
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
