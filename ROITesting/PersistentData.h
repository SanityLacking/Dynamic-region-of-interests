#ifndef PersistentData_H
#define PersistentData_H
#include <vector>
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;
//very simple class that accepts data in and stores it to a file.
class persistentData{
public:
	//constructor
	persistentData();
	//functions
	void store(vector<Rect>objects, double time );
	void storeToFile(vector<Rect>objects, double time, string filename, bool fileFinished = true);
	void storeToFile(string filename, bool fileFinished = true);
	vector<vector<Rect> > currentData;
	vector<double> processTime;
	string outputHeader;
private:
	string outputResults();
};
#endif