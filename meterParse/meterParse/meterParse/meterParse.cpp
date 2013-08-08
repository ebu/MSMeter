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

int parseFileGiveToStacks (ifstream &file);

class ClientStack {
public:
	ClientStack();
	void addLine(string line);
	bool writeToFile(double time);
	//parameters
	int id;
	int blockSize;
	int frameSize;
	int mode;
	int currentTime;
	string filename;
};

//output file stream
ofstream output;


int _tmain(int argc, _TCHAR* argv[])
{
	/*
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
	*/

	//output file preparation
	
	output.open ("output.csv");

	string filename = "results2.csv";
	ifstream file (filename);
	parseFileGiveToStacks(file);



	return 0;
}

//reads line by line, "sends" the lines to the right instance of client
int parseFileGiveToStacks (ifstream &file) {
	vector<ClientStack> clients;
	if (file.is_open()){
		string line;


		while (file.good()){
			getline(file, line);

			//3rd digit gives us the client id
			stringstream ss(line);
			ss.get(); ss.get();
			int clientID;
			ss >> clientID;

			//check if client has a stack already
			bool found = false;
			ClientStack* temp = NULL;
			for(vector<ClientStack>::iterator it = clients.begin(); it != clients.end(); ++it) {
				cout << "Searching : " << (*it).id << endl;
				if ((*it).id == clientID){
					found = true;
					temp = &(*it);
					break;
				}

			}
			if (!found){
				//create new instance of ClientStack
				cout << "creating instance for id : " << clientID << endl;
				ClientStack *tempClientStack = new ClientStack();
				tempClientStack->id = clientID;
				cout << "created instance, id is : " << (*tempClientStack).id << endl;
				clients.push_back(*tempClientStack);
				temp = tempClientStack;
				cout << "trying to access id from pointer : " << (*temp).id << endl;
			}

			//give the string line to the clientStack instance
			
			(*temp).addLine(line);

		}


	} else {
		cout << "unable to open file" << endl;
		return 1;
	}
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


ClientStack::ClientStack() {
	//nothing initialised in constructor
}

void ClientStack::addLine(string line) {
	cout << "Message from clientstack with id " << id << " : {" << line << "}" << endl;
	stringstream ss(line);
	ss.get(); ss.get(); ss.get(); ss.get(); //"removing" first 4 irrelevant chars

	//adding a line
	//case 1: it is the main (initialising line)
	if (line.at(4) != ':'){
		cout << "init line for stack " << id << endl;

		int t1, t2;
		int length;
		double rate, latency;

		t1 = line.find(',');
		t2 = line.find(',', t1 + 1);
		this->mode = atoi((line.substr(t1 + 1, t2 - t1 - 1)).c_str());
		cout << "mode " << this->mode << endl;
		
		t1 = t2;
		t2 = line.find(',', t1 + 1);
		length = atol((line.substr(t1 + 1, t2 - t1 - 1)).c_str());
		cout << "length : " << length << endl;

		//the second time i'm doing length reading, dirty hack, need to investigate
		//but it works
		t1 = t2;
		t2 = line.find(',', t1 + 1);
		length = atol((line.substr(t1 + 1, t2 - t1 - 1)).c_str());
		cout << "length : " << length << endl;

		t1 = t2;
		t2 = line.find(',', t1 + 1);
		blockSize = atoi((line.substr(t1 + 1, t2 - t1 - 1)).c_str());
		cout << "blocksize " << blockSize << endl;

		t1 = t2;
		t2 = line.find(',', t1 + 1);						//This should be the last ','
		rate = atof((line.substr(t1 + 1, t2 - t1 - 1)).c_str());
		cout << "rate " << rate << endl;

		latency = atof((line.substr(t2 + 1)).c_str());
		cout << "latency " << latency << endl;


	//case 2: is the NTP line
	} else if(line.at(5) == 't'){
		//cout << "NTP line for stack " << id << endl;
		ss.get(); ss.get();
		string ntpMicroSec;
		ss >> ntpMicroSec;
		//cout << "NTP value : " << ntpMicroSec << endl;

	//case 3: it is the delta times "arrays"
	} else {
		cout << "delta times arrays for stack " << id << endl;
		double value;
		vector<double> dTimeArray;
		ss.get();
		while (ss >> value){
			(dTimeArray).push_back(value);
			if (ss.peek() == ':') ss.ignore();
			writeToFile(value);
		}
	}
}

bool ClientStack::writeToFile(double time) {

	output << id << "," << mode << "," << blockSize << "," << time << endl;
	return true;
}
