#include<iostream>


#include "src/IO/IO.h"
#include "src/Parser/Parser.h"
#include "src/Utils/StringUtils.h"
#include "src/Models/Structs.h"
#include "src/Models/OpTable.h"

std::ifstream inputFile;
std::ofstream outputFile;

void openInitialStreams(std::string in, std::string out) {
	// This function opens the file for reading and writing data
	inputFile.open(in.c_str());
	std::remove(out.c_str());
	outputFile.open(out.c_str(), std::ios::app);
}

ParseResult readAndParse() {
	std::string before = ReadLine(&inputFile);
	trim(before);
	ParseResult result = parse(before);
	return result;
}

void finish() {
	// This function is called in last to close all opened file streams.
	outputFile.close();
	inputFile.close();
}

int main(int argc, char *argv[]){
	// Check if the file is passed as an argument or not
	// if not then error out
 	if(argc != 2){
		std::cout<<"\nFile Missing , Invalid number of arguments\n";
		return 1;
	}

	openInitialStreams(argv[1],"listFile.txt");

	ParseResult result;

	while (!inputFile.eof()) {
		result = readAndParse();
	}
	std::cout<<result;
	finish();
	return 1;
}