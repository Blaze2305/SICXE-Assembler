#include<iostream>

#include "src/IO/IO.h"
#include "src/Parser/Parser.h"
#include "src/Utils/StringUtils.h"
#include "src/Models/Structs.h"
#include "src/Models/OpTable.h"
#include "src/Assembler/Pass1.h"

std::ifstream inputFile;
std::ofstream outputFile;
std::vector<ParseResult> parseArray;

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

void closeFileStreams() {
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

	std::string output = "Output_"+Splitpath(argv[1]).back();

	openInitialStreams(argv[1],output);

	ParseResult result;

	while (!inputFile.eof()) {
		result = readAndParse();
		parseArray.push_back(result);
	}

	AssignLOCCTR(parseArray);
	closeFileStreams();
	return 0;
}