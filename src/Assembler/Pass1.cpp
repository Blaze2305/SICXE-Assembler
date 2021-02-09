#include "Pass1.h"
#include<sstream>

std::pair<int,int> getNewBlockNumberAndLOCCTR(int oldBlockNumber ,int LOCCTR, std::map<int,ProgBlock>& BlockTable, std::string newBlockName){
	
	BlockTable[oldBlockNumber].Length = LOCCTR;
	
	for(auto obj : BlockTable){
		if(obj.second.BlockName == newBlockName){
			return std::pair<int,int>{obj.second.BlockNumber,obj.second.Length};
		}
	}
	
	int blockNum = BlockTable.size();
	BlockTable[blockNum] = {
		newBlockName,
		blockNum,
		0,
		0
	};

	return std::pair<int,int>{blockNum,0};
}

void AssignLOCCTR(std::vector<ParseResult>& ParseArr,std::map<int,ProgBlock>& BlockTable){
	int LOCCTR = 0x0;
	int currentBlockNumber = 0;

	BlockTable[0] = {
		"Default",
		0,
		0,
		0
	};

	for(auto obj : ParseArr){

		if(ToUpperCase(obj.mnemonic) == "START"){
			LOCCTR = std::stoi(ParseArr[0].operand1,(size_t*)nullptr,16);
			BlockTable[0].StartingAddress = LOCCTR;
			continue;
		}

		obj.location = LOCCTR;
		obj.block = currentBlockNumber;
		std::cout<<obj;
		if(obj.type == "Comment"){
			continue;
		}

		if(obj.mnemonic[0] == '+'){
			LOCCTR += 0x4;
		}else if(obj.type == "Directive"){
			if(ToUpperCase(obj.mnemonic) == "BYTE"){
				if(obj.operand1[0] == 'C'){
					LOCCTR += obj.operand1.length() -3;
				}else if(obj.operand1[0]=='X'){
					LOCCTR += (obj.operand1.length() - 3)/2; 
				}else{
					throw std::runtime_error("WRONG USE OF BYTE => "+obj.label+" "+obj.mnemonic+" "+obj.operand1);
				}
			}else if(ToUpperCase(obj.mnemonic) == "WORD"){
				LOCCTR += 3;
			}else if(ToUpperCase(obj.mnemonic) == "RESB"){
				LOCCTR += std::stoi(obj.operand1);
			}else if (ToUpperCase(obj.mnemonic) == "RESW"){
				LOCCTR += 3 * std::stoi(obj.operand1);
			}else if(ToUpperCase(obj.mnemonic) == "USE"){
				if(obj.operand1 == ""){
					std::pair<int,int> temp = getNewBlockNumberAndLOCCTR(currentBlockNumber,LOCCTR,BlockTable,"Default");
					currentBlockNumber = temp.first;
					LOCCTR = temp.second;
				}else{
					std::pair<int,int> temp = getNewBlockNumberAndLOCCTR(currentBlockNumber,LOCCTR,BlockTable,obj.operand1);
					currentBlockNumber = temp.first;
					LOCCTR = temp.second;
				}
				continue;
			}else{
				continue;
			}
		}else{
			LOCCTR += GetInstFormat(obj.mnemonic);
		}

	}
}
