/*
 *  resultOutputStream.h
 *
 *  Created on: Nov 25, 2009
 *      Author: Derek Boytim
 */

#ifndef _OutputStream
#define _OutputStream

#include <iostream>
#include <fstream>

class OutputStream {
  private:
	ofstream outputStream;
	string outputFile;
	bool isOpen;
  public:
	OutputStream(string);
	~OutputStream();
	bool isOpen();
	void write(int);
	void write(char);
	void wirte(string);
	OutputStream operator<<(int);
	OutputStream operator<<(char);
	OutputStream operator<<(string);
};

OutputStream::OutputStream(string fileName) {
	outputFile = fileName;
	outputStream.open(fileName);
	if(outputStream.is_open()) {
		isOpen = true;
	}
}

bool OutputStream::isOpen() {
	return isOpen;
}

void OutputStream::write(int i) {
	outputStream << i;
}

void OutputStream::write(char c) {
	outputStream << c;
}

void OutputStream::write(string s) {
	outputStream << s;
}

OutputStream OutputStream::operator<<(int i) {
	outputStream << i;
	return this;
}

OutputStream OutputStream::operator<<(char c) {
	outputStream << c;
	return this;
}

OutputStream OutputStream::operator<<(string s) {
	outputStream << s;
	return this;
}

#endif

