// objectDetection.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include <time.h> // to calculate time needed
#include <limits.h> // to get INT_MAX, to protect against overflow

#include <iostream>
#include <future>
#define _GLIBCXX_USE_NANOSLEEP
#include <string>
#include <thread>


using namespace cv;
using namespace std;


const int frameWidth = 320;
const int frameHeight = 240;
Size frameSize;

Point frameP(Point resize);
Rect frameRoi(Rect obj, Point RoiPoint);

double PointDist(Point a, Point b);
vector<Eyes> partnerEyes(vector<Rect>eyes);
vector<Rect> detectEyes(Mat& frame);
vector<Rect> detectEyes(Mat& frame, vector<Rect>& Roi);
void trackEyes(vector<Eyes> currentEyes, vector<Eyes>& pastEyes);
Mat processImage(Mat frame, vector<Eyes>& pastEyes, vector<Rect>& Roi);
void updateRoi(vector<Rect>& Roi, vector<Eyes>,int maxWidth, int maxHeight);
int eyeHeight = 15;
int eyeDistance = 200;

int avgDistance= 0;
int pairsFound = 0;

void detectEyesAsync(Mat frame);
bool asyncReady = true; //when true, the async complete detecteyes can run.
vector<Rect> asyncEyes; //if the async returns results they will be stored here until the next run so that they can be used.




vector<Point> objectsTracked;


cv::String eyes_cascade_name = "haarcascades/haarcascade_eye_tree_eyeglasses.xml";
cv::String face_cascade_name = "haarcascades/haarcascade_frontalface.xml";

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

int displayCamera(VideoCapture& camera){
	/*
	if (!camera.isOpened())  // check if we succeeded
		return -1;
	

	vector<Eyes> pastEyes; //vector that is going to contain the eyes from the previous loop.
	vector<Eyes>& pastEyesRef = pastEyes;
	vector<Eyes> pastE;
	vector<Eyes>& pastERef = pastE;
	vector<Rect> Roi;
	vector<Rect>& RoiRef = Roi;


	// fps counter begin
	time_t start, end;
	int counter = 0;
	double sec;
	double fps;
	// fps counter end


	Mat edges;
	//namedWindow("edges", 1);
	for (;;)
	{
		if (counter == 0){
			time(&start);
		}
		// fps counter end
		
		Mat frame;
		camera >> frame; // get a new frame from camera
		//flip(frame, frame, 0);
		
		frame = processImage(frame, pastEyes, RoiRef);
		cout << "frame end" << endl;

		time(&end);
		counter++;
		sec = difftime(end, start);
		fps = counter / sec;
		if (counter > 30)
			
		// overflow protection
		if (counter == (INT_MAX - 1000))
			counter = 0;
		// fps counter end
		putText(frame, "fps: " + to_string(fps), Point(5, 15), FONT_HERSHEY_PLAIN, 1.2, Scalar(0, 0, 255, 255), 2);
	
		if (frame.empty()) break; // end of video stream
		imshow("this is you, smile! :)", frame);
		if (waitKey(1) == 27) break; // stop capturing by pressing ESC 
		

		
	}
	*/
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}

// convert a point generated used the resized frame to the equivalent position in the original frame.


Mat processImage(Mat frame, vector<Eyes>& pastEyesRef, vector<Rect>& RoiRef){
	//process the image.
	frameSize = frame.size();
	Mat processImg = frame;
	
	//resize(frame, processImg, Size(frameWidth, frameHeight),0,0,1);

	//cvtColor(processImg, processImg, CV_BGR2GRAY);
	//equalizeHist(processImg, processImg);
	Mat& processImgRef = processImg;
	std::vector<Rect> eyes;

	eyes = detectEyes(processImgRef, RoiRef);
	/*
	for (int i = 0; i < RoiRef.size(); i++){	//draw current ROi.
		rectangle(frame, Point(RoiRef[i].x, RoiRef[i].y), Point(RoiRef[i].x + RoiRef[i].width, RoiRef[i].y + RoiRef[i].height), Scalar(0, 0, 0), 1, 8, 0);
		namedWindow("roi", 1);
		imshow("roi", frame(RoiRef[i]));
	}
	*/


	vector<Eyes> eyesVect = partnerEyes(eyes);
	cout << "eyes vector size : " << eyesVect.size() << endl;
	updateRoi(RoiRef, eyesVect, frameWidth, frameHeight);

	for (int i = 0; i < RoiRef.size(); i++){
		rectangle(frame, frameP(Point(RoiRef[i].x, RoiRef[i].y)), frameP(Point(RoiRef[i].x + RoiRef[i].width, RoiRef[i].y + RoiRef[i].height)), Scalar(0,0,0), 1, 8, 0);
	}

	String eyeLoc;
	
	for (int i = 0; i < eyesVect.size(); i++){
		//display the previous 
		//cout << pastEyesRef.size() << endl;
		trackEyes(eyesVect, pastEyesRef);
		for (int z = 0; z < pastEyesRef.size(); z++){
			rectangle(frame, frameP(Point(pastEyesRef[i].rect.x, pastEyesRef[i].rect.y)), frameP(Point(pastEyesRef[i].rect.x + pastEyesRef[i].rect.width, pastEyesRef[i].rect.y + pastEyesRef[i].rect.height)), pastEyesRef[i].colour, 1, 8, 0);
		};
		
		rectangle(frame, frameP(Point(eyesVect[i].rect.x, eyesVect[i].rect.y)), frameP(Point(eyesVect[i].rect.x + eyesVect[i].rect.width, eyesVect[i].rect.y + eyesVect[i].rect.height)), eyesVect[i].colour, 1, 8, 0);
		if (eyesVect[i].partnerId >=0)
			line(frame, frameP(eyesVect[i].center()), frameP(eyesVect[eyesVect[i].partnerId].center()), eyesVect[i].colour, 1);
		
		putText(frame, "" + to_string(eyesVect[i].rect.width) + "/" + to_string(eyesVect[i].rect.height), frameP(Point(eyesVect[i].rect.x, eyesVect[i].rect.y + eyesVect[i].rect.height + 15)), FONT_HERSHEY_PLAIN, 1.2, Scalar(0, 0, 255, 255), 2);
	}
	
	//update pastEyes



	pastEyesRef.clear();
//	cout << pastEyesRef.size() << endl;



	for (int i = 0; i < eyesVect.size(); i++){
		eyesVect[i].colour = Scalar(255, 0, 0);
		//rectOverlaps(eyesVect[i], pastEyesRef);
		pastEyesRef.push_back(eyesVect[i]);
	}

	return frame;
}

// convert point generated for resized frame to the correct position for the original frame.
Point frameP(Point resize){

	return Point(resize.x * (frameSize.width / frameWidth), resize.y * (frameSize.height / frameHeight));

}
// convert Rect generated for Roi frame to the correct position for the original frame.
Rect frameRoi(Rect obj, Point roiPoint){
	return Rect(obj.x + roiPoint.x, obj.y+ roiPoint.y, obj.width, obj.height);

}

vector<Rect>detectEyes(Mat& frame, vector<Rect>& RoiRef){
	
	// for each ROI run through detect eyes method.
	vector<Rect> eyes;


	//begin async call
	/*
	if (asyncReady == true){
		cout << "async returns: " << asyncEyes.size() << endl;
		for (int j = 0; j < asyncEyes.size(); j++){
			eyes.push_back(asyncEyes[j]);
		}
		asyncEyes.clear();
		asyncReady = false;
		thread(detectEyesAsync, frame);
	}
	*/

	//end async call


	bool noRoi = true;

	if (RoiRef.size() > 0){	//if Roi is present use Roi.
		cout << "ROI Present " << RoiRef.size()<< endl;
		noRoi = false;
		for (int i = 0; i < RoiRef.size(); i++){
		//	cout << "ROISize" << RoiRef[i]<<endl;
			Mat RoiFrame = frame(RoiRef[i]);
			Mat& RoiFrameRef = RoiFrame;
			vector<Rect> eyes2 = detectEyes(RoiFrameRef);
			for (int j = 0; j < eyes2.size(); j++){
				eyes.push_back(frameRoi(eyes2[j], Point(RoiRef[i].x, RoiRef[i].y)));
			}
			//eyes.insert(eyes.end(), eyes2.begin(), eyes2.end()); //insert all of eyes2 into the return value.
			
		}
	}else{	// if no Roi is present, scan whole image.
		cout << "no ROI Present" << endl;
		eyes = detectEyes(frame);

	}
	
	if (eyes.size() == 0 && noRoi == false){	// if an Roi was used but no results were found, scan whole image.
		cout << "no Eyes within ROI" << endl;
		eyes = detectEyes(frame);
	}
	cout <<"number of eyes detected " << eyes.size() << endl;

	/*
	for (int i = 0; i < eyes.size(); i++){
		cout << eyes[i] << endl;
	}
	*/
	//eyes = detectEyes(frame);

	for (int i = 0; i < eyes.size(); i++){
		for (int j = 0; j < eyes.size(); j++){
			if ((eyes[j].x > eyes[i].x && eyes[j].x < eyes[i].x + eyes[i].width) && (eyes[j].y > eyes[i].y && eyes[j].y < eyes[i].y + eyes[i].height)){ // if j is within i 
				if (eyes[j].width + eyes[j].height < eyes[i].width + eyes[i].height){// if J is smaller then I
					//cout << "overlapping eyes detected" << endl;
					eyes.erase(eyes.begin() + j);
				}
			}
		}
	}

	return eyes;
}

vector<Rect> detectEyes(Mat& frame){
	vector<Rect> eyes;
	//-- Detect eyes
	eyes_cascade.detectMultiScale(frame, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(1, 1), Size(40, 40));

	// if two eye results overlap, pick the result that is larger.
	// notes: you only have to delete J here, as this will loop through the entire array.
	// this will not have an issue with comparing to itself as it uses < not <= to compare.
		for (int i = 0; i < eyes.size(); i++){
			for (int j = 0; j < eyes.size(); j++){
				if ((eyes[j].x > eyes[i].x && eyes[j].x < eyes[i].x + eyes[i].width) && (eyes[j].y > eyes[i].y && eyes[j].y < eyes[i].y + eyes[i].height)){ // if j is within i 
					if (eyes[j].width + eyes[j].height < eyes[i].width + eyes[i].height){// if J is smaller then I
						//cout << "overlapping eyes detected" << endl;
						eyes.erase(eyes.begin() + j);
					}
				}
			}
		}
	

		// find the matching eye, if any.
		// for speed, later this should be intergrated into the other loop, however while testing this is sufficent to stay as a second loop.
		// need to save the partnering of the eyes somewhere in the class, to do this, perhaps I should make a new class that extends Rect
		// that includes a partner iterator tag.
	//cout << eyes.size()<< endl;
	return eyes;
}


// run detectEyes on the frame and when it returns add it to the global async variable. this will then be added to the next frame as eyes.
void detectEyesAsync(Mat frame){
	vector<Rect> objects;
	objects = detectEyes(frame);

	for (int j = 0; j < objects.size(); j++){
		asyncEyes.push_back(objects[j]);
	}
	asyncReady = true;
}
// partner the eyes that have been detected
vector<Eyes> partnerEyes(vector<Rect>eyes){
	std::vector<Eyes> eyesVect;

	/*
	Define what characteristics a pair of eyes has:
	they are roughly the same size.
	the distance between the two centers is a certain ratio of of the size of the eye.
	
	*/
		int partnerId =-1;//-1 instead of null, i think null does werid things in c++
		double diagonal, diagonal2;
		Scalar colour;
		for (int i = 0; i < eyes.size(); i++){
			Point centerPoint;
			for (int j = 0; j < eyes.size(); j++){
				if ((abs(eyes[j].y - eyes[i].y) <= eyeHeight) && (j != i) && abs((eyes[j].x + eyes[j].height / 2) - (eyes[i].x + eyes[i].height / 2))<=eyeDistance){//exclude matching against itself.
					//compare the distance between the centers of each eye 
					//&& (abs(eyes[j].x - eyes[i].x) <= eyeDistance)
				//	double ratio = (abs((eyes[j].x + eyes[j].height / 2) - (eyes[i].x + eyes[i].height / 2))) / (abs((eyes[i].height * eyes[i].width) + (eyes[j].height * eyes[j].width)) / 2);
					diagonal = sqrt(pow(eyes[i].height, 2) + pow(eyes[i].width, 2));
					diagonal2 = sqrt(pow(eyes[j].height, 2) + pow(eyes[j].width, 2));
					double avgDiag = (diagonal + diagonal2) / 2;
					double distance = abs(((eyes[j].x + eyes[j].height / 2) - (eyes[i].x + eyes[i].height / 2)));
					double ratio = avgDiag / distance;
					//cout << "avg size of pair :" << avgDiag << " " << "distance between pair: " << distance << " " <<"Ratio: " << ratio << endl;

					//track the eyes and their movement from the last frame.

					//average area of the eyes compared to the distance between the two centers
					//cout << "distance between eyes" << abs(((eyes[j].x + eyes[j].height / 2) - (eyes[i].x + eyes[i].height / 2))) << "average size: " << abs(((eyes[i].height * eyes[i].width) / 2) + ((eyes[j].height * eyes[j].width) / 2) / 2) << " :Ratio to size : " << ratio << endl;

					partnerId = j;
					colour = Scalar(100,100,100);//will need to randomize this.
				}
			}
			Eyes e = Eyes(eyes[i]);
			if (partnerId >= 0){// partner was found.
				e.partnerId = partnerId;
				e.colour = colour;


				//add center point as
				objectsTracked.push_back(Point());
			}
			eyesVect.push_back(e);
		}	

	return eyesVect;

}

//track eyes from the previous eye positions to the current positions
void trackEyes(vector<Eyes> currentEyes, vector<Eyes>& pastEyes){
	//compare current eyes to previous eyes. 
	// compare centerpoint movement
	// compare width and height as well?
	// as I am only dealing with a few eyes per image, i don't need to use a sort or bubble sort. this would work faster,
	// but when dealing with approx maybe 2-6 eyes at most per image, the benefit drops.
	
	// define center distance movement
	int dist = 10;

	for (int i = 0; i < currentEyes.size(); i++){
		for (int j = 0; j < pastEyes.size(); j++){
			if (abs(PointDist(currentEyes[i].center(), pastEyes[j].center())) <= dist){
				pastEyes[j].colour = Scalar(102, 255, 102);
				//cout << "past eye linked" << endl;
			}
		}

	}



}

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	//if (!face_cascade.load(face_cascade_name)){ printf("--(!)Error loading face\n"); };
	if (!eyes_cascade.load(eyes_cascade_name)){ printf("--(!)Error loading eyes\n"); };


	//Mat image = getInput();
	VideoCapture camera(0);
	if (!camera.isOpened()){  // check if we succeeded
		cout << "camera not opened" << endl;
		//return -1;
	}
	displayCamera(camera);

	cout << "Program End";
	getchar();

	return 0;

	*/

	VideoCapture cap;
	// open the default camera, use something different from 0 otherwise;
	// Check VideoCapture documentation.
	if (!cap.open(0))
		return 0;
	for (;;)
	{
		Mat frame;
		cap >> frame;
		if (frame.empty()) break; // end of video stream
		imshow("this is you, smile! :)", frame);
		if (waitKey(1) == 27) break; // stop capturing by pressing ESC 
	}
	// the camera will be closed automatically upon exit
	// cap.close();
	return 0;
}

// simple equation to detemine the distance between two 2d points.
double PointDist(Point a, Point b){
		return sqrt((a.x - b.x)^2 + (a.y - b.y)^2);
}


// when eyes are found, store their locations in the Roi
void updateRoi(vector<Rect>& Roi, vector<Eyes> eyes , int maxWidth, int maxHeight){
	// take the eyes found and make an Rect around them, based on how big the eyes are is how big the ROi should be.
	Roi.clear();
	int expandSize = 10;
	for (int i = 0; i < eyes.size(); i++){
		
		Rect r = Rect(
			max(eyes[i].rect.x - expandSize, 0),
			max(eyes[i].rect.y - expandSize, 0),
			min((eyes[i].rect.width + expandSize * 2),	abs(maxWidth - eyes[i].rect.x)),
			min(eyes[i].rect.height + expandSize * 2,  abs(maxHeight - eyes[i].rect.y)));
		//cout << "width: " << min((eyes[i].rect.width + expandSize * 2), maxWidth) << endl;
		//cout <<"stored ROI "<< r << endl;
		Roi.push_back(r);
	}

	// if i have overlapping ROi i should probably combine them.

}

bool rectOverlaps(Rect a, vector<Rect> r ){
	for (int i = 0; i < r.size(); i++){
		if (a.x + a.width >= r[i].x && a.y + a.height >= r[i].y){
	//		return Rect(a.x, a.y, abs(a.x - r[i].x) - r[i].width, abs(a.y - r[i].y) - r[i].height);
			return true;
		}
		else if (r[i].x + r[i].width >= a.x && r[i].y + r[i].height >= a.y){
		//	return Rect(r[i].x, r[i].y, abs(r[i].x - a.x) - a.width, abs(r[i].y - a.y) - a.height);
			return true;
		}
	}
	
}
// check to see if two Rects overlap each other, if they do, 
// TODO: Improve the code for this check. there should be a way of making the else if unessecary.
Rect rectOverlaps(Rect a, Rect b){
	if (a.x + a.width >= b.x && a.y + a.height >= b.y){
		return Rect(a.x, a.y, abs(a.x - b.x) - b.width, abs(a.y - b.y) - b.height);
	}
	else if (b.x + b.width >= a.x && b.y + b.height >= a.y){
		return Rect(b.x, b.y, abs(b.x - a.x) - a.width, abs(b.y - a.y) - a.height);
	}
}	