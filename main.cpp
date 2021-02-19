#include<iostream>

#include "src/IO/IO.h"
#include "src/Parser/Parser.h"
#include "src/Utils/Utils.h"
#include "src/Models/Structs.h"
#include "src/Models/OpTable.h"
#include "src/Assembler/Pass1.h"
#include "src/Assembler/Pass2.h"

std::ifstream inputFile;
std::ofstream outputFile;
std::vector<ParseResult> ParseArray;
std::map<int,ProgBlock> BlockTable;
std::vector<Literal> LITTAB;
std::map<std::string,SymTabRow> SYMTAB;


// This function opens the file for reading and writing data
void openInitialStreams(std::string in, std::string out) {
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

// This function is called in last to close all opened file streams.
void closeFileStreams() {
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
	std::cout<<"----------------------------------------------------------------------------\n";
	std::cout<<argv[1]<<std::endl;
	std::cout<<"----------------------------------------------------------------------------\n";
	
	std::string output = "Output_"+Splitpath(argv[1]).back();

	openInitialStreams(argv[1],output);

	ParseResult result;

	while (!inputFile.eof()) {
		result = readAndParse();
		ParseArray.push_back(result);
	}

	AssignLOCCTR(ParseArray,BlockTable,LITTAB,SYMTAB);
	GenerateObjectProgram(ParseArray,LITTAB,BlockTable,&outputFile);
	closeFileStreams();
	return 0;
}