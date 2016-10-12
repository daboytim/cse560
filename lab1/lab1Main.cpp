/*
 * lab1Main.cpp
 *
 *  Created on: Jan , 11 2010
 *      Author: The Dougars
 */
#include <iostream>
#include <fstream>

using namespace std;

void putUsage();
bool parsePrimaryFile(int&, ifstream&, ofstream&, ofstream&);
void parseAuxFile(int&, ifstream&);
void openResultFile(ifstream&);
void openReportFile(ifstream&);
int toInt(string);

int main(int argc, char* argv[]) {
	if(argc != 5) {
		cout <<"Error: command line." << argc;
		putUsage();
		return 1;
	}
	
	ifstream auxiliaryIn (argv[1]);
	ifstream primaryIn (argv[2]);
	ofstream resultOut (argv[3]);
	ofstream reportOut (argv[4]);
	int numLines;
			   
	parseAuxFile(numLines, auxiliaryIn);
	
	if(!parsePrimaryFile(numLines, primaryIn, resultOut, reportOut)) {
		cout <<"Error:";
		return 1;
	}
	
	return 0;
}

void putUsage() {
	cout << "\nUsage: %lab1Main 'auxiliaryInputFile' 'primaryInputFile' 'resultOutputFile' 'reportOutputFile'";
}

void parseAuxFile(int& numLines, ifstream& auxIn) {
	numLines=0;
	if(auxIn.is_open()) {
		string line;
		auxIn >> line;
		numLines = toInt(line);
	}else {
		cout <<"Error: auxiliary file could not be read.";
	}
}

bool parsePrimaryFile(int& numLines,
					  ifstream& primaryIn,
					  ofstream& resultOut,
					  ofstream& reportOut) {
	
	
	if(numLines == -1) {
		cout <<"Error: auxiliary file did not contain a positive integer.";
		return false;
	}else if(numLines == 0) {
		cout <<"Error: numLines == 0";
		return false;
	}
	
	for(int i=0; i<numLines; i++) {
		char byte=0;
		int outputBytes=0;
		while(!primaryIn.eof() && byte != '\n' && outputBytes < 80) {
			primaryIn.get(byte);
			if(byte == 9 || (byte >= 32 && byte <= 126) ) {
				resultOut << byte;
				outputBytes++;
			}
		}
		if(outputBytes == 80) {
			while(!primaryIn.eof()) {
					primaryIn.ignore('\n');
			}
		}
		resultOut << '\n';
		if(primaryIn.eof()) {
			break;
		}
	}
	
	return true;
}

int toInt(string s) {
	int val=0;
	for(int i=0; i<s.length(); i++) {
		if(s[i] < 48 || s[i] > 57) {
			return -1;
		}
		val *= 10;
		val += ((int)s[i])-48;
	}
	return val;
}

