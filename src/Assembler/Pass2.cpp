#include "Pass2.h"

int getDisplacement(int PC, std::string mnemonic){

}


void GenerateObjectProgram(std::vector<ParseResult>& ParseArr,std::vector<Literal>& LITTAB,std::map<int,ProgBlock>& BlockTable,std::ofstream* outfile){
	std::string progName = ParseArr[0].label;
	std::string startingAddr = ParseArr[0].operand1;
	std::ostringstream headerRecord;
	if(progName.length() >6){
		progName.resize(6);
	}
	headerRecord<<progName;
	headerRecord<<std::setw(6)<<std::setfill('0')<<startingAddr;
	int progLength = BlockTable[BlockTable.size() - 1].StartingAddress + BlockTable[BlockTable.size() - 1].Length ; 
	headerRecord<<std::setw(6)<<std::setfill('0')<<std::hex<<progLength<<std::dec<<std::endl;
	WriteLine(outfile,headerRecord.str());

	std::ostringstream textRecords;

	std::vector<ObjCode> ObjectCodes;

	for(auto parseItem : ParseArr){
		ObjCode obj;
		obj.operation = GetInstOpCode(parseItem.mnemonic);
		obj.format = parseItem.mnemonic[0] == '+' ? 4 : GetInstFormat(parseItem.mnemonic);
		if(obj.format == 1){
			continue;
		}else if(obj.format == 2){
			obj.reg1 = GetRegisterNumber(parseItem.operand1[0]);
			obj.reg2 = GetRegisterNumber(parseItem.operand2[0]);
		}else if(obj.format == 3 || obj.format == 4){
			if(parseItem.operand1[0] == '#'){
				obj.flags[0] = 0;
				obj.flags[1] = 1;
			}else if(parseItem.operand1[0] == '@'){
				obj.flags[0] = 1;
				obj.flags[1] = 0;
			}else{
				obj.flags[0] = 1;
				obj.flags[1] = 1;
			}

			if(parseItem.operand1 == "X" || parseItem.operand2 == "X"){
				obj.flags[2] = 1;
			}

			if(parseItem.operand1[0] == '+'){
				obj.flags[3] = 0; 
				obj.flags[4] = 0; 
				obj.flags[5] = 1;
			}else{
				
			}

		}
		
	}

}