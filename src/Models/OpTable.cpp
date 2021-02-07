#include "OpTable.h"

void GetInstFormat(std::string mnemonic){
	std::map<std::string,InstInfo>::const_iterator pos = OPTAB.find(mnemonic);
	std::cout<<"MNEMONIC -> "<<mnemonic<<"\n";
	if (pos == OPTAB.end()) {
		throw std::runtime_error("INVALID INSTRUCTION " + pos->second.mnemonic);
	} else {
		std::cout<<pos->second;
	}
}

void GetInstOpCode(std::string mnemonic){
	std::map<std::string,InstInfo>::const_iterator pos = OPTAB.find(mnemonic);
	std::cout<<"MNEMONIC -> "<<mnemonic<<"\n";
	if (pos == OPTAB.end()) {
		throw std::runtime_error("INVALID INSTRUCTION " + pos->second.mnemonic);
	} else {
		std::cout<<pos->second;
	}
}