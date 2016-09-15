
#include "faceDetection.h"
#include "Eyes.h"
#include <iostream>

using namespace std;

FaceDetect::FaceDetect(){
	if (!face_cascade.load(face_cascade_name)){ printf("--(!)Error loading face\n"); };
	if (!eyes_cascade.load(eyes_cascade_name)){ printf("--(!)Error loading eyes\n"); };
}

vector<Rect> FaceDetect::detectFaces(Mat& frame, vector<Rect>& RoiRef){
	vector<Rect> faces;
	bool noRoi = true;
	if (RoiRef.size() > 0){	//if Roi is present use Roi.
		cout << "ROI Present " << RoiRef.size() << endl;
		noRoi = false;
		for (int i = 0; i < RoiRef.size(); i++){
			Mat RoiFrame = frame(RoiRef[i]);
			Mat& RoiFrameRef = RoiFrame;
			vector<Rect> faces2 = detectFaces(RoiFrameRef);
			for (int j = 0; j < faces2.size(); j++){
				faces.push_back(frameRoi(faces2[j], Point(RoiRef[i].x, RoiRef[i].y)));
			}
		}
		cout << "ROI FACES: " << faces.size() << endl;
	}
	else{	// if no Roi is present, no objects were detected in the last frame scan whole image.                                                                             
		cout << "no ROI Present" << endl;
		faces = detectFaces(frame);
	}

	if (faces.size() == 0 && noRoi == false){	// if an Roi was present but no results were found, scan whole image.
		cout << "no faces within ROI" << endl;
		faces = detectFaces(frame);
		/*else if (fallbackMethod == "expandBox"){
			for (int i = 0; i < RoiRef.size(); i++){
				expandRect(RoiRef[i], 50, frame.size());
			}

		}*/
		//either the program expands the boxes or scans the whole img. to know this, I need to know what to do from the ROI.
	}
	cout << "number of faces detected " << faces.size() << endl;

	for (int i = 0; i < faces.size(); i++){
		for (int j = 0; j < faces.size(); j++){
			if ((faces[j].x > faces[i].x && faces[j].x < faces[i].x + faces[i].width) && (faces[j].y > faces[i].y && faces[j].y < faces[i].y + faces[i].height)){ // if j is within i 
				if (faces[j].width + faces[j].height < faces[i].width + faces[i].height){// if J is smaller then I
					//cout << "overlapping faces detected" << endl;
					faces.erase(faces.begin() + j);
				}
			}
		}
	}
	return faces;
}

vector<Rect> FaceDetect::detectFaces(Mat& frame){
	vector<Rect> faces;
	face_cascade.detectMultiScale(frame, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(40, 40), Size(340, 340));
	/*
	for (int i = 0; i < faces.size(); i++){
		for (int j = 0; j < faces.size(); j++){
			if ((faces[j].x > faces[i].x && faces[j].x < faces[i].x + faces[i].width) && (faces[j].y > faces[i].y && faces[j].y < faces[i].y + faces[i].height)){ // if j is within i 
				if (faces[j].width + faces[j].height < faces[i].width + faces[i].height){// if J is smaller then I
					//cout << "overlapping faces detected" << endl;
					faces.erase(faces.begin() + j);
				}
			}
		}
	}
	*/
	return faces;
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
			imshow("ROI", RoiFrame);
			//rectangle(frame, RoiRef[i], Scalar(0, 155, 255), 2);
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

// expand rectangle r by amount s. s is applied to each side. so if 10 is given for s, the rectangle will grow by 20.
void expandRect(Rect& r, int s, Size frameSize){
	r = Rect(
		r.x - s,
		r.y - s,
		min(r.width + s * 2, abs(frameSize.width - r.x)),
		min(r.height + s * 2, abs(frameSize.height - r.y))
		);
}