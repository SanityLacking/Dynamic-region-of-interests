// ROITesting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//#include "measurementSuite.h"
#include "Eyes.h"
#include "ROIs.h"


#include <time.h> // to calculate time needed
#include <limits.h> // to get INT_MAX, to protect against overflow


// fps counter begin
time_t tstart, tend;
int counter = 0;
double sec;
double fps;
// fps counter end

using namespace cv;
using namespace std;

int displayCamera(VideoCapture& camera);
void processImage(Mat& frame, vector<Rect>& RoiRef);

int _tmain(int argc, _TCHAR* argv[])
{
	VideoCapture camera;
	if (!camera.open(0))
		return 0;
	displayCamera(camera);
	
	return 0;
}



int displayCamera(VideoCapture& camera){

	Mat frame;
	vector<Rect> rec;
	//MeasureTool mTool;
	
	for (;;){
		if (counter == 0){
			time(&tstart);
		}
		// fps counter end

		camera >> frame;
	
		//mTool.start();
		processImage(frame, rec);
		//mTool.end();

		time(&tend);
		counter++;
		sec = difftime(tend, tstart);
		fps = counter / sec;
		if (counter > 30)

			// overflow protection
			if (counter == (INT_MAX - 1000))
				counter = 0;
		// fps counter end
		putText(frame, "fps: " + to_string(fps), Point(5, 15), FONT_HERSHEY_PLAIN, 1.2, Scalar(0, 0, 255, 255), 2);

	//	putText(frame, "fps: " + to_string(mTool.getFPS()), Point(5, 15), FONT_HERSHEY_PLAIN, 1.2, Scalar(0, 0, 255, 255), 2);
		imshow("output", frame);
		if (waitKey(1) == 27) break;
	}
	
	
	return 0;
}

//do the preliminary processing in this function, this function calls specific processing functions as well.
void processImage(Mat& frame, vector<Rect>& RoiRef){
	
	Size frameSize = frame.size();
	Mat processImg = frame;

	//preprocess Blur, color correct, etc


	//call Detection Method


	//Set ROI



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

