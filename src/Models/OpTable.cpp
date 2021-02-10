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