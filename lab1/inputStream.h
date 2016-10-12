/*
 *  inputStream.h
 *
 *  Created on: Nov 25, 2009
 *      Author: Derek Boytim
 */

#ifndef _InputStream
#define _InputStream

#include <iostream>
#include <fstream>

using namespace std;

class InputStream {
  private:
	const ifstream *inputStream;
	string inputFile;
	bool open;
  public:
	InputStream(string fileName);
	~InputStream();
	bool isOpen();
	int readInt();
	char readChar();
	string readSstring();
};

InputStream::InputStream(string fileName) {
	inputFile = fileName;
	inputStream = new ifstream (fileName);
	if(inputStream.is_open()) {
		isOpen = true;
	}
}

bool InputStream::isOpen() {
	return isOpen;
}

int InputStream::readInt() {
	int i;
	inputStream >> i;
	return i;
}

char InputStream::readChar() {
	char c;
	inputStream >> c;
	return c;
}

string InputStream::readString() {
	string s;
	inputStream >> s;
	return s;
}

#endif
