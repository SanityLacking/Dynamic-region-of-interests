#ifndef ROIS_H
#define ROIS_H
#include "ObservableData.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;
//base class
class ROI{
public:
	//vars
	Size frameSize;
	int expansion = 10;
	vector<Rect> currentROI;
	vector<Rect> pastROI;
	ObsData& obsData;
	ObsData DefaultObsData; //if no reference to an Obs Data, the class will use this one.
	vector<Rect> outputROI;
	//constructor
	ROI(Size s);
	ROI(Size s, ObsData& obs);
	//functions
	vector<Rect> getROI();
	void expandRect(Rect& r, int s, Size frameSize);
	virtual int setROI(vector<Rect>objects, double time, Size s) = 0;
	virtual int fallback(vector<Rect>&objects, Size s) = 0;
	string ROItype = "unset";
private:
};

// the derived classes of all the possible types of ROI.
class Control : public ROI{
public:
	Control(Size s, ObsData& obs) :ROI(s, obs){};
	int setROI(vector<Rect>objects, double time, Size s);
	int fallback(vector<Rect>&objects, Size s);
	string ROItype = "Control";
};


class DynamicRentering : public ROI{
public:
	DynamicRentering(Size s, ObsData& obs) :ROI(s, obs){};
	DynamicRentering(Size s) :ROI(s){};
	int setROI(vector<Rect>objects, double time, Size s);
	int fallback(vector<Rect>&objects, Size s);
	string ROItype = "DynamicRentering";
};

class Static : public ROI{
public:
	Static(Size s, ObsData& obs) :ROI(s, obs){};
	Static(Size s) :ROI(s){};
	int setROI(vector<Rect>objects, double time, Size s);
	int fallback(vector<Rect>&objects, Size s);
	string ROItype = "Static";
};

class StaticRentering : public ROI{
public:
	StaticRentering(Size s, ObsData& obs) :ROI(s, obs){};
	StaticRentering(Size s) :ROI(s){};
	int setROI(vector<Rect>objects, double time, Size s);
	int fallback(vector<Rect>&objects, Size s);
	string ROItype = "StaticRentering";
};

class StaticExpanding : public ROI{
public:
	StaticExpanding(Size s, ObsData& obs) :ROI(s, obs){};
	StaticExpanding(Size s) :ROI(s){};
	int expansionAmount = 50;
	int setROI(vector<Rect>objects, double time, Size s);
	int fallback(vector<Rect>&objects, Size s);
	string ROItype = "StaticExpanding";
};

class ExpandingBox : public ROI{
public:
	ExpandingBox(Size s, ObsData& obs) :ROI(s, obs){};
	ExpandingBox(Size s) :ROI(s){};
	int setROI(vector<Rect>objects, double time, Size s);
	int fallback(vector<Rect>&objects, Size s);
	string ROItype = "ExpandingBox";
};

class BlockMatching : public ROI{
public:
	BlockMatching(Size s, ObsData& obs) :ROI(s, obs){};
	BlockMatching(Size s) :ROI(s){};
	int setROI(vector<Rect>objects, double time, Size s);
	int fallback(vector<Rect>&objects, Size s);
	string ROItype = "BlockMatching";
};

class KalmanPredictive : public ROI{
public:
	KalmanPredictive(Size s, ObsData& obs) :ROI(s, obs){};
	KalmanPredictive(Size s) :ROI(s){};
	int setROI(vector<Rect>objects, double time, Size s);
	int fallback(vector<Rect>&objects, Size s);
	//variables:
	Point previousPrediction;
	double pastP;
	double kalmanGain = 0.5;
	double errMeasurement = 0.5;
	double Q, R = 0; //constants
	string ROItype = "KalmanPredictive";
};

class MonteCarloPredictive : public ROI{
public:
	MonteCarloPredictive(Size s, ObsData& obs) :ROI(s, obs){};
	MonteCarloPredictive(Size s) :ROI(s){};
	int setROI(vector<Rect>objects, double time, Size s);
	int fallback(vector<Rect>&objects, Size s);
	string ROItype = "MonteCarloPredictive";
};

#endif