// ROITesting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "ROIs.h"
#include "measurementSuite.h"
#include "faceDetection.h"
#include <time.h> // to calculate time needed
#include <chrono>
#include <limits.h> // to get INT_MAX, to protect against overflow
#include <fstream>

#include <iomanip>
#include <ctime>
#include <sstream>

using namespace cv;
using namespace std;

bool displayBool =  true;
bool storeBool = true;
bool fileFinished = false;

//depending on which method is run, the output string here is changed.
//eg: dynamic_recentering, static_control, dynamic_kalman.
string outputString = "dynamic_recentering";

string outputHeader = "frame, ROI found, ROI location, ROI size, frame process time \n";
int displayCamera(VideoCapture& camera);
void processImage(Mat& frame, ROI& roi, MeasureTool mTool, FaceDetect faceDetect);
int storeResults(MeasureTool mTool, ROI roi);
Point frameP(Point resize);


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
	MeasureTool mTool;
	cout << "frame size start: "<< frame.size << endl;
	ROI roi(frame.size());
	FaceDetect faceDetect;

	for (;;){
		frame.release();
		camera >> frame;
		if (frame.empty()){
			fileFinished = true;
			break; //the file has finished or the web camera has stopped sending frames.
		}
		mTool.updateStats(roi.pastROI);

		mTool.start();//fps counter start
		processImage(frame, roi, mTool,faceDetect);
		mTool.end();// fps counter end
		
		if (displayBool){
			putText(frame, "fps: " + to_string(mTool.getFPS()), Point(5, 15), FONT_HERSHEY_PLAIN, 1.2, Scalar(0, 0, 255, 255), 2);
			imshow("output", frame);
			if (waitKey(1) == 27) {
				break;
			}
		}
	}
	storeResults(mTool, roi);

	return 0;
}

//do the preliminary processing in this function, this function calls specific processing functions as well.
void processImage(Mat& frame, ROI& roi, MeasureTool mTool, FaceDetect faceDetect){	
	Size frameSize = frame.size();
	Mat processImg = frame;
	vector<Rect> objects;
	
	//preprocess Blur, color correct, etc	

	//call Detection Method
	objects = faceDetect.detectFaces(processImg, roi.pastROI);
	// Draw Detections

	for (int i = 0; i < objects.size(); ++i){
		rectangle(frame, objects[i], Scalar(255, 255, 255));
	}
	for (int i = 0; i < roi.pastROI.size(); ++i){
		rectangle(frame, roi.pastROI[i], Scalar(0, 155, 255),2);
	}

	//Set new ROI
	roi.setROI(objects,frame.size());
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

// after input has finished, finialize results and store them in an excel document for further comparasion. 
int storeResults(MeasureTool mTool, ROI roi){
	ofstream myfile;
	struct tm timeinfo;
	time_t now;
	now = time(NULL);
	auto t = std::time(nullptr);
	localtime_s(&timeinfo, &now);
	std::ostringstream oss;
	oss << std::put_time(&timeinfo, "%Y%m%d%H%M");
	auto str = oss.str();

	if (!fileFinished)
		outputString += "_unfinished";
	string output_name = ".\\output\\"+outputString+ "_" +str+".csv";
	myfile.open(output_name);
	myfile << outputHeader;
	myfile << mTool.outputResults();
	myfile.close();
	return 0;

}