#include "OpTable.h"

int GetInstFormat(std::string mnemonic){
	std::map<std::string,InstInfo>::const_iterator pos = OPTAB.find(mnemonic);
	if (pos == OPTAB.end()) {
		throw std::runtime_error("INVALID INSTRUCTION " + mnemonic + " CANNOT FETCH FORMAT;");
	} else {
		return pos->second.format;
	}
}

int GetInstOpCode(std::string mnemonic){
	if(mnemonic[0] == '+'){
		mnemonic = mnemonic.substr(1);
	}
	std::map<std::string,InstInfo>::const_iterator pos = OPTAB.find(mnemonic);
	if (pos == OPTAB.end()) {
		throw std::runtime_error("INVALID INSTRUCTION " + mnemonic + " CANNOT FETCH OPCODE;");
	} else {
		return pos->second.opCode;
	}
}

int GetRegisterNumber(char registerCode){
	std::map<char,int>::const_iterator pos = REGISTERS.find(registerCode);
	if(pos == REGISTERS.end()){
		throw std::runtime_error("INVALID REGISTER " + std::to_string(pos->second) + " CANNOT FETCH REGISTER CODE;");
	} else {
		return pos->second;
	}
}