#include "PersistentData.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h> // to calculate time needed
#include <chrono>
#include <fstream>

#include <iomanip>

using namespace std;

persistentData::persistentData(){
	outputHeader = "frame, ROI found, ROI location, ROI size, frame process time \n";
}
// add vector of rects to be stored to the local memory. until storetofile is called they will not be saved to a file.
void persistentData::store(vector<Rect>objects, double time){
	cout << "store: "<<objects.size() << "saved so far: "<< currentData.size()<< endl;

	currentData.push_back(objects);
	processTime.push_back(time);
}


void persistentData::storeToFile(vector<Rect>objects, double time, string filename, bool fileFinished){
	currentData.push_back(objects);
	processTime.push_back(time);
	storeToFile(filename);
}


void persistentData::storeToFile(string filename, bool fileFinished){
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
		filename += "_unfinished";
	string output_name = ".\\output\\" + filename + "_" + str + ".csv";
	cout << output_name << endl;
	cout << currentData.size() << endl;
	myfile.open(output_name);
	myfile << outputHeader;
	myfile << outputResults();
	myfile.close();
}

string persistentData::outputResults(){
	string output = "";
	for (int i = 0; i < currentData.size(); i++){
		output += "" + to_string(i) + "," + to_string(currentData[i].size()) + ",";
		if (currentData[i].size() > 0){
			string loc;
			//when decoding this part, delimit by a space " ", then split each one by "_". 
			//this will give you the (x,y) and (width,height) for each of the ROI found in that frame.
			for (int j = 0; j < currentData[i].size(); j++){
				loc += to_string(currentData[i][j].x) + "_" + to_string(currentData[i][j].y) + " ";
			}
			loc += ",";
			for (int j = 0; j < currentData[i].size(); j++){
				loc += to_string(currentData[i][j].width) + "_" + to_string(currentData[i][j].height) + " ";
			}
			output += loc;
		}
		output += "," + to_string(processTime[i]);
		output += "\n";
		cout << output << endl;
	}
	return output;

}