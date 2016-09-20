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
	
private:
};

// the derived classes of all the possible types of ROI.
class DynamicRentering : public ROI{
public:
	DynamicRentering(Size s, ObsData& obs) :ROI(s, obs){};
	DynamicRentering(Size s) :ROI(s){};
	int setROI(vector<Rect>objects, double time, Size s);
	int fallback(vector<Rect>&objects, Size s);
};

class StaticRentering : public ROI{
public:
	StaticRentering(Size s, ObsData& obs) :ROI(s, obs){};
	StaticRentering(Size s) :ROI(s){};
	int setROI(vector<Rect>objects, double time, Size s);
	int fallback(vector<Rect>&objects, Size s);
};

class ExpandingBox : public ROI{
public:
	ExpandingBox(Size s, ObsData& obs) :ROI(s, obs){};
	ExpandingBox(Size s) :ROI(s){};
	int setROI(vector<Rect>objects, double time, Size s);
	int fallback(vector<Rect>&objects, Size s);
};

class BlockMatching : public ROI{
public:
	BlockMatching(Size s, ObsData& obs) :ROI(s, obs){};
	BlockMatching(Size s) :ROI(s){};
	int setROI(vector<Rect>objects, double time, Size s);
	int fallback(vector<Rect>&objects, Size s);
};

class KalmanPredictive : public ROI{
public:
	KalmanPredictive(Size s, ObsData& obs) :ROI(s, obs){};
	KalmanPredictive(Size s) :ROI(s){};
	int setROI(vector<Rect>objects, double time, Size s);
	int fallback(vector<Rect>&objects, Size s);
};

class MonteCarloPredictive : public ROI{
public:
	MonteCarloPredictive(Size s, ObsData& obs) :ROI(s, obs){};
	MonteCarloPredictive(Size s) :ROI(s){};
	int setROI(vector<Rect>objects, double time, Size s);
	int fallback(vector<Rect>&objects, Size s);
};


#endif