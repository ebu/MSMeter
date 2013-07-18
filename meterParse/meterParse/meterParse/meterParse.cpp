// meterParse.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

int parse(string name);
int parseARead(ifstream &file, vector<double> array);


int _tmain(int argc, _TCHAR* argv[])
{
	string filename = "results.csv";
	ifstream file (filename);
	vector<double> array;
	parseARead(file, array);
	parseARead(file, array);
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
int parseARead(ifstream &file, vector<double> array) {
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

			//cout << "line *" << line << "*" << endl;

			double value;
			stringstream ss(line);
			ss.get(); ss.get(); ss.get();ss.get();ss.get();
			while (ss >> value){
				//do something with value
				cout << value << endl;
				array.push_back(value);
				if (ss.peek() == ':') ss.ignore();
			}

			if (line.at(4) != ':'){
				file.seekg(position, ios_base::beg);
				break;
			}

		}

	} else {
		cout << "Unable to open file" << endl;
		return 1;
	}

}

