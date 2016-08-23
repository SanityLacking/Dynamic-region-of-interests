
#include "faceDetection.h"
#include "Eyes.h"
#include <iostream>

using namespace std;

FaceDetect::FaceDetect(){
	if (!eyes_cascade.load(eyes_cascade_name)){ printf("--(!)Error loading eyes\n"); };
}
vector<Rect> FaceDetect::detectFaces(Mat input, vector<Rect>pastROI){
	vector<Rect> ROI;
	vector<Rect> eyes = detectEyes(input, pastROI);
	
	return eyes;
}

vector<Rect> FaceDetect::detectEyes(Mat& frame, vector<Rect>& RoiRef){
	vector<Rect> eyes;
	bool noRoi = true;
	if (RoiRef.size() > 0){	//if Roi is present use Roi.
		cout << "ROI Present " << RoiRef.size() << endl;
		noRoi = false;
		for (int i = 0; i < RoiRef.size(); i++){
			//	cout << "ROISize" << RoiRef[i]<<endl;
			Mat RoiFrame = frame(RoiRef[i]);
			Mat& RoiFrameRef = RoiFrame;
			rectangle(frame, RoiRef[i], Scalar(0, 155, 255), 2);
			vector<Rect> eyes2 = detectEyes(RoiFrameRef);
			for (int j = 0; j < eyes2.size(); j++){
				eyes.push_back(frameRoi(eyes2[j], Point(RoiRef[i].x, RoiRef[i].y)));
			}
			//eyes.insert(eyes.end(), eyes2.begin(), eyes2.end()); //insert all of eyes2 into the return value.
		}
	}
	else{	// if no Roi is present, scan whole image.
		cout << "no ROI Present" << endl;
		eyes = detectEyes(frame);
	}

	if (eyes.size() == 0 && noRoi == false){	// if an Roi was used but no results were found, scan whole image.
		cout << "no Eyes within ROI" << endl;
		eyes = detectEyes(frame);
	}
	cout << "number of eyes detected " << eyes.size() << endl;

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

vector<Rect> FaceDetect::detectEyes(Mat& frame){
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

// convert Rect generated for Roi frame to the correct position for the original frame.
Rect FaceDetect::frameRoi(Rect obj, Point roiPoint){
	return Rect(obj.x + roiPoint.x, obj.y + roiPoint.y, obj.width, obj.height);

}