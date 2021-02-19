#include "Pass2.h"

// Function to get the value of the literal from the LITTAB
// params:
// 		literal : std::string The literal name
// 		LITTAB : std::vector<Literal> Literal Table
// returns the value of the literal
std::string getLiteralValue(std::string literal,std::vector<Literal> LITTAB){
	for(auto item : LITTAB){
		if(item.Name == literal){
			return item.Value;
		}
	}
	throw new std::string("LITERAL DOES NOT EXIST => " + literal);
	
}

// int getDisplacement(int PC, std::string mnemonic,std::vector<ParseResult> ParseArr){
	// To do : calc displacement , would be easier if symtab was implemented so do that first
// }


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

	std::vector<ObjCode> ObjectCodes;

	for(auto parseItem : ParseArr){
		ObjCode obj;
		if(parseItem.type == "Comment"){
			continue;
		}

		if(parseItem.type == "Directive"){
			continue;
		}

		if(parseItem.mnemonic[0] == '='){
			obj.value = getLiteralValue(parseItem.mnemonic,LITTAB);
			continue;
		}

		obj.operation = GetInstOpCode(parseItem.mnemonic);
		obj.format = parseItem.mnemonic[0] == '+' ? 4 : GetInstFormat(parseItem.mnemonic);




		if(obj.format == 1){
			continue;
		}else if(obj.format == 2){
			obj.reg1 = GetRegisterNumber(parseItem.operand1[0]);
			obj.reg2 = parseItem.operand2 != ""?GetRegisterNumber(parseItem.operand2[0]):0;
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
		
		ObjectCodes.push_back(obj);
	}

	// for(auto item:ObjectCodes){
	// 	std::cout<<item;
	// }

}