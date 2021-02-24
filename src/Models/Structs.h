#ifndef STRUCT_H_
#define STRUCT_H_

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <bitset>
#include <stack>


// Structure to hold each instruction and its information
struct InstInfo{
	std::string mnemonic;// The instruction as its written
	int format; //format 1 <----> 4
	int opCode; // its opcode in hex
	
	friend std::ostream& operator << (std::ostream& out,const InstInfo &obj);

};

// Structure to hold the object code of any given instruction

// https://stackoverflow.com/questions/29483123/why-does-stdbitset-expose-bits-in-little-endian-fashion
// https://stackoverflow.com/questions/4975037/why-are-the-bits-of-a-stdbitset-in-reverse-order
// bitset stores the bits in reverse order ie 2 is stored in std::bitset<4> as 0100  NOT 0010. this is consistent with the index order
// ie 2^0 2^1 ... 2^index to calc the value of the bitset.
// Therefore we set the nixbpe flags from 5 -> 0 instead of 0 -> 5
struct ObjCode{
	int operation;
	int displacement;
	std::bitset<6> flags; // nixbpe flags n = 5,i = 4 .... e = 0
	int format; // format 1 <---> 4 // also stores length in case of BYTE,WORD
	int reg1;
	int reg2;
	int location;
	int blockNumber;
	std::string value; // Used for literal pools, BYTE, WORD

	ObjCode();

	friend std::ostream& operator << (std::ostream& out,const ObjCode &obj);

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

struct Literal{
	std::string Name;
	std::string Value;
	int Length;
	int Address;
	int Block;

	friend std::ostream& operator << (std::ostream& out,const Literal& obj);
};


struct SymTabRow {
	std::string SymbolName;
	int BlockNumber;
	char Type;
	int Value;

	friend std::ostream& operator << (std::ostream& out,const SymTabRow& obj);

};


std::ostream& operator<< (std::ostream& out, const std::vector<std::string>& obj);
std::ostream& operator<< (std::ostream& out, const std::stack<std::string>& obj);
std::ostream& operator<< (std::ostream& out, const std::map<std::string,SymTabRow>& obj);


#endif