#ifndef STRUCT_H_
#define STRUCT_H_

#include<map>
#include<string>
#include<iostream>
#include<vector>
#include<bitset>


// Structure to hold each instruction and its information
struct InstInfo{
	std::string mnemonic;// The instruction as its written
	int format; //format 1 <----> 4
	int opCode; // its opcode in hex
	
	friend std::ostream& operator << (std::ostream& out,const InstInfo &obj);

};

// Structure to hold the object code of any given instruction
struct ObjCode{
	int operation;
	int displacement;
	std::bitset<6> flags; // nixbpe flags
	int format; // format 1 <---> 4
	int reg1;
	int reg2;
};


// Structure to hold the result of each parsed line
struct ParseResult{
	std::string label;
	std::string mnemonic;
	std::string operand1;
	std::string operand2;
	std::string type;
	int block;
	int location;
	
	friend std::ostream& operator << (std::ostream& out,const ParseResult &obj);
};

struct ProgBlock{
	std::string BlockName;
	int BlockNumber;
	int StartingAddress;
	int Length;

	friend std::ostream& operator << (std::ostream& out,const ProgBlock &obj);
};


#endif