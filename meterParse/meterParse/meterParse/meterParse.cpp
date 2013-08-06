// meterParse.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

int parse(string name);
int parseARead(ifstream &file, vector<double>& array);
int process(vector<double>& dTime, int blockSize, int window);
int processTimeDependent(vector<double>& times, int blockSize, int window);
void printVector(vector<double>& array);
void deltaTimeToReferencedTime(vector<double>& array);
double timeToSpeed(double time, int data);


int _tmain(int argc, _TCHAR* argv[])
{
	string filename = "results.csv";
	ifstream file (filename);
	vector<double> array;
	parseARead(file, array);
	process(array, 2, 20);
	printVector(array);
	array.clear();
	parseARead(file, array);
	printVector(array);
	//array.clear();
	//deltaTimeToReferencedTime(array);
	//processTimeDependent(array, 2, 3);

	return 0;
}

int readAndPrint(string name) {

	string line;
	ifstream file (name);
	if (file.is_open()){
		while ( file.good() ) {
			getline (file,line);
			cout << line << endl;
		}
		file.close();
	} else {
		cout << "Unable to open file"; 
		return 1;
	}
	return 0;
}

//function takes the ifstream and reads from where it last stopped
//stops when is the end of a "block" of data
int parseARead(ifstream &file, vector<double>& array) {
	string line;
	if (file.is_open()){

		//first line contains the infos about the read
		getline(file, line);
		cout << "First line *" << line << "*" << endl;

		
		//reading the rest
		while ( file.good() ){
			//memorizing position
			streampos position = file.tellg();
			getline(file, line);

			if (line.at(4) != ':'){
				file.seekg(position, ios_base::beg);
				break;
			}


			double value;
			stringstream ss(line);
			ss.get(); ss.get(); ss.get();ss.get();ss.get();
			while (ss >> value){
				(array).push_back(value);
				if (ss.peek() == ':') ss.ignore();
			}


			

		}

	} else {
		cout << "Unable to open file" << endl;
		return 1;
	}

}

int process(vector<double>& dTime, int blockSize, int window) {

	//note: the values in dTime are in [ms] and the blockSize is in KB
	
	if (window <= dTime.size()) {

		int size = (dTime).size();
		cout << "size: " << size << endl;

		//average of sliding window
		vector<double> slidingAverages;
		for (int i = 0; i <= dTime.size()-window; i++) {
			double accumulator = 0;
			for (int j = i; j < (i+window) ; j++){
				accumulator += dTime.at(j);
			}
			slidingAverages.push_back(accumulator/window);
		}
		sort(slidingAverages.begin(), slidingAverages.end());
		for(int i = 0; i < slidingAverages.size(); i++) {
			cout << "time in ms: " << slidingAverages.at(i) << ", and speed : " << timeToSpeed(slidingAverages.at(i),blockSize*1024) << endl;
		}

		//simple average value
		double sum = 0;
		for (int j = 0; j < (dTime).size(); j++) {
			sum += (dTime).at(j);
		}
		cout << "average : " << sum/(double)(dTime).size() << endl;
		return 0;
	} else {
		//DEBUG
		cout << "window size too big" << endl;
		return 1;
	}
}

//scroll through time... could be inefficient. needs an preprocessed array
//in referenced time
int processTimeDependent(vector<double>& times, int blockSize, int window) {
	//note: the value in times are in ms, blockSize is in KB, window is in ms
	double timeStep = 0.001;

	if (!times.empty()){

		for (double currentTime = 0.0; currentTime < (times.back() - window); currentTime += timeStep) {
			double endTime = currentTime + window;
			int startingBlock;
			int endingBlock;
			
			if (endTime > times.back()) break;
			int i;
			//checking on which block we start
			for (i = 0; i < times.size(); i++){
				if (currentTime <= times.at(i)) {
					startingBlock = i+1;
					break;
				}
			}
			//checking on which block we end
			for (i; i < times.size(); i++){
				if (currentTime < times.at(i)) {
					endingBlock = i-1;
				}
			}
			//calculating the time to transmit frame
			double time = times.at(endingBlock) - times.at(startingBlock);
			cout << "ms : " << currentTime << " - " << time << endl;
		}
	
		return 0;
	} else {
		cout << "processTimeDependent: empty vector" << endl;
		return 1;
	}
}

//Will transform an array with delta times like 0.23 0.32 0.32
// to an array of times connected to a reference => 0.23 0.55 0.87
void deltaTimeToReferencedTime(vector<double>& array) {
	printVector(array);
	double accumulator = 0;
	for (int i = 0; i < array.size(); i++) {
		accumulator += array.at(i);
		array.at(i) = accumulator;
	}
	printVector(array);
}

void printVector(vector<double>& array) {
	cout << "Double vector of size : " << array.size() << endl;
	for (int i = 0; i < array.size(); i++) {
		cout << array.at(i) << endl;
	}
}

//receives the time in ms
//and quantity of data in bytes
//and gives the speed in MB/s
double timeToSpeed(double time, int data) {
	return ((data*1000) / (time*1024*1024));
}

class ClientStack {

public:

	void addLine(string line){

	}

	bool writeToFile(){

	}



};

