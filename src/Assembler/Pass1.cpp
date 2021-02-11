#include "Pass1.h"

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


bool checkIfLiteralExists(std::vector<Literal> LITTAB,std::string literal){
	return std::find_if(LITTAB.begin(),LITTAB.end(),[literal](Literal val){
		return(val.Value == literal);
	}) != LITTAB.end();
}


void addToLITTAB(std::vector<Literal>& LITTAB,std::string literal){

	std::string value = "";
	int length;
	if(literal[1] == 'C'){
		for(int i=3;i<literal.length()-1;i++){
			int ascii = (int)literal[i];
			std::ostringstream out;
			out << std::hex << ascii;
			value += out.str();
		}
		length = literal.length()-4;
	}else if(literal[1] == 'X'){
		value = literal.substr(3,literal.length()-4);
		length = value.length()/2;
	}else{
		value = literal;
	}
	if(!checkIfLiteralExists(LITTAB,value)){
		LITTAB.push_back({
			literal,
			value,
			length,
			-1,
			-1
		});
	}
}

int assignAddrToLITTAB(std::vector<Literal>& LITTAB,int &LOCCTR,std::vector<ParseResult> &ParseArr,int currIndex,int blockNumber){
	int count = 0;
	for(auto &obj : LITTAB){
		if(obj.Address == -1){
			count += 1;
			// std::cout<<"CURRENT INDEX "<<currIndex<<std::endl;
			obj.Address = LOCCTR;
			obj.Block = blockNumber;
			LOCCTR += obj.Length;
			ParseArr.insert(ParseArr.begin() + currIndex,{
				"",
				obj.Name,
				"",
				"",
				"Instruction",
				obj.Block,
				obj.Address
			});
			currIndex += 1;
		}
	}
	return count;
}

void AssignLOCCTR(std::vector<ParseResult>& ParseArr,std::map<int,ProgBlock>& BlockTable,std::vector<Literal>& LITTAB){
	int LOCCTR = 0x0;
	int currentBlockNumber = 0;

	BlockTable[0] = {
		"Default",
		0,
		0,
		0
	};

	for(int i=0;i<ParseArr.size();i++){
		ParseResult &obj = ParseArr[i];
		if(ToUpperCase(obj.mnemonic) == "START"){
			BlockTable[0].StartingAddress = std::stoi(ParseArr[0].operand1,(size_t*)nullptr,16);
			continue;
		}

		obj.location = BlockTable[currentBlockNumber].StartingAddress + LOCCTR;
		obj.block = currentBlockNumber;
		if(obj.type == "Comment"){
			continue;
		}

		if(obj.operand1[0] == '='){
			addToLITTAB(LITTAB,obj.operand1);
		}

		if(obj.mnemonic[0] == '+'){
			LOCCTR += 0x4;
		}else if(obj.type == "Directive"){
			if(ToUpperCase(obj.mnemonic) == "BYTE"){
				if(obj.operand1[0] == 'C'){
					LOCCTR += obj.operand1.length() -3;
				}else if(obj.operand1[0]=='X'){
					LOCCTR += (obj.operand1.length() - 3)/2; 
				}else if(obj.operand1[0] == '=' && obj.operand1[1] == 'C'){
					LOCCTR += obj.operand1.length() -4;
				}else if(obj.operand1[0] == '=' && obj.operand1[1] == 'X'){
					LOCCTR += (obj.operand1.length() - 4)/2; 
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
			}else if(ToUpperCase(obj.mnemonic) == "LTORG" || ToUpperCase(obj.mnemonic) == "END"){
				int skip = assignAddrToLITTAB(LITTAB,LOCCTR,ParseArr,i+1,currentBlockNumber);
				i += skip;
			}else{
				continue;
			}
		}else{
			LOCCTR += GetInstFormat(obj.mnemonic);
		}
	}

	// for(auto obj:ParseArr){
	// 	std::cout<<obj<<std::endl;
	// }

	BlockTable[currentBlockNumber].Length = LOCCTR;


	for(int i=1;i<BlockTable.size();i++){
		BlockTable[i].StartingAddress = BlockTable[i-1].StartingAddress + BlockTable[i-1].Length;
	}

	std::cout<<"BLOCK TABLE\n\n\n";
	for(auto item: BlockTable){
		std::cout<<item.second.BlockName<<"    ";
		std::cout<<item.second.BlockNumber<<"    ";
		std::cout<<std::hex<<item.second.StartingAddress<<std::dec<<"    ";
		std::cout<<std::hex<<item.second.Length<<std::dec<<"    \n\n\n";
		
	}


	// std::cout<<"LITERAL TABLE\n\n";
	// for(auto item: LITTAB){
	// 	std::cout<<item.Name<<"     ";
	// 	std::cout<<item.Value<<"     ";
	// 	std::cout<<std::hex<<item.Address<<std::dec<<"     ";
	// 	std::cout<<std::hex<<item.Length<<std::dec<<"     ";
	// 	std::cout<<item.Block<<"     \n";
	// }
}
