#include "OpTable.h"

int GetInstFormat(std::string mnemonic){
	std::map<std::string,InstInfo>::const_iterator pos = OPTAB.find(mnemonic);
	if (pos == OPTAB.end()) {
		throw std::runtime_error("INVALID INSTRUCTION " + pos->second.mnemonic);
	} else {
		return pos->second.format;
	}
}

int GetInstOpCode(std::string mnemonic){
	std::map<std::string,InstInfo>::const_iterator pos = OPTAB.find(mnemonic);
	if (pos == OPTAB.end()) {
		throw std::runtime_error("INVALID INSTRUCTION " + pos->second.mnemonic);
	} else {
		return pos->second.opCode;
	}
}

int GetRegisterNumber(char registerCode){
	std::map<char,int>::const_iterator pos = REGISTERS.find(registerCode);
	if(pos == REGISTERS.end()){
		throw std::runtime_error("INVALID REGISTER " + pos->second);
	} else {
		return pos->second;
	}
}