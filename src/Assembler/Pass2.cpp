#include "Pass2.h"

// Function to get the value of the literal from the LITTAB
// params:
// 		literal : std::string The literal name
// 		LITTAB : std::vector<Literal> Literal Table
// returns the value of the literal
std::pair<std::string,int> getLiteralValue(std::string literal,std::vector<Literal> LITTAB){
	for(auto item : LITTAB){
		if(item.Name == literal){
			return std::pair<std::string,int>(item.Value,item.Value.length()/2);
		}
	}
	throw new std::string("LITERAL DOES NOT EXIST => " + literal);
	
}

int getAbsoluteDisplacement(std::string label,std::map<std::string,SymTabRow> SYMTAB,std::vector<Literal> LITTAB,std::map<int,ProgBlock> BlockTable){
	int TargetAddr;
	std::string temp = "";
	// std::cout<<label<<std::endl;
	if(label[0] == '#'){

		temp = label.substr(1);
		if(is_number(temp)){
			TargetAddr = std::stoi(temp);
			return TargetAddr;
		}

	}else if(label[0] == '@'){
		temp = label.substr(1);
	}else{
		temp = label;
	}
	auto iter = SYMTAB.find(temp);
	if ( iter != SYMTAB.end() ) {
		if(iter->second.Type == 'A'){
			TargetAddr = iter->second.Value;
			return TargetAddr;
		}else{
			int blockNumber = iter->second.BlockNumber;
			TargetAddr = iter->second.Value + BlockTable[blockNumber].StartingAddress;
			return TargetAddr;
		}
	}
	throw std::runtime_error("INVALID SYMBOL USED  " + temp);


}


std::pair<int,int> getDisplacement(int PC, int Base,std::string label,std::vector<Literal> LITTAB,std::map<std::string,SymTabRow> SYMTAB,std::map<int,ProgBlock> BlockTable){

	// -8264 <= x <= 8263 PC rel

	// 0 <= x <= 16533 Base rel	
	enum TYPES {
		NO_REL,
		PC_REL,
		BASE_REL
	};

	int TargetAddr;

	std::string temp = "";
	if(label[0] == '='){
		std::vector<Literal>::iterator position;
		position = std::find_if(LITTAB.begin(),LITTAB.end(),[label](Literal val){
			return(val.Name == label);
		});
		if(position == LITTAB.end()){
			std::string err = "INVALID LITERAL USED  " + label;
			throw std::runtime_error(err);
		}else{
			TargetAddr = position->Address + BlockTable[position->Block].StartingAddress;
		}
	}else if(label[0] == '#'){

		temp = label.substr(1);
		if(is_number(temp)){
			TargetAddr = std::stoi(temp);
			return std::pair<int,int>{NO_REL,TargetAddr};
		}

	}else if(label[0] == '@'){
		temp = label.substr(1);
	}else{
		temp = label;
	}

	if(temp != ""){
		auto iter = SYMTAB.find(temp);
		if ( iter != SYMTAB.end() ) {
			if(iter->second.Type == 'A'){
				TargetAddr = iter->second.Value;
				return std::pair<int,int>{NO_REL,TargetAddr};
			}else{
				int blockNumber = iter->second.BlockNumber;
				TargetAddr = iter->second.Value + BlockTable[blockNumber].StartingAddress;
			}
		} else {

			std::string err = "INVALID SYMBOL USED  " + label;
			throw std::runtime_error(err);
		}
	}

	int disp;
	disp = TargetAddr - PC;

	if(disp >= -8264 && disp <= 8263){
		return std::pair<int,int>{PC_REL,disp};
	}

	disp = TargetAddr - Base;

	if(disp >= 0 && disp <= 16533){
		return std::pair<int,int>{BASE_REL,disp};
	}

	throw "INSTRUCTION NEEDS TO BE FORMAT 4 : ";
}


std::pair<int,std::string> getByteDirectiveValue(std::string mnemonic){

	std::string value = "";
	int length ;

	if(mnemonic[0] == 'C'){
		// concat hex value of each char 
		for(int i=2;i<mnemonic.length()-1;i++){
			int ascii = (int)mnemonic[i];
			std::ostringstream out;
			out << std::hex << ascii;
			value += out.str();
		}
		length = mnemonic.length()-3;
	}else if(mnemonic[0] == 'X'){
		value = mnemonic.substr(2,mnemonic.length()-3);
		length = value.length()/2;
	}

	return std::pair<int,std::string> {length,value};
}


	
std::string generateTextRecord(std::string startingAddr, std::vector<ObjCode>& ObjectCodes,std::map<int,ProgBlock> BlockTable){
	std::ostringstream textRecord;
	std::ostringstream temp;
	std::string tempObjCode;
	int length = 0;
	int currentBlockNumber = ObjectCodes[0].blockNumber;
	int  ObjectCodeLength = ObjectCodes.size();
	textRecord << "T"<< std::setw(6)<<std::setfill('0')<<startingAddr;	
	for(int i = 0; i < ObjectCodes.size();i++){
		tempObjCode = GenerateOpCode(ObjectCodes[i]);

		// std::cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
		// std::cout<<"ObjecCode ---> "<<ObjectCodes[i]<<std::endl;
		// std::cout<<"CURRENT BLOCK NUMBER ---> "<<currentBlockNumber<<std::endl;
		// std::cout<<"OBJECT CODE BLOCK NUMBER ---> "<<ObjectCodes[i].blockNumber<<std::endl;
		// std::cout<<"TEMP ---> "<<tempObjCode<<std::endl;
		// std::cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";


		if(tempObjCode == "-1"){

			// Handles assmebler directives USE and causes a change in program block
			if(ObjectCodes[i].blockNumber != currentBlockNumber){
				currentBlockNumber = ObjectCodes[i].blockNumber;
			}
			// Handle the RESB and RESW directives
			if(length != 0){
				textRecord <<std::setw(2)<<std::setfill('0')<< std::hex << length << std::dec;
				textRecord << temp.str();
				textRecord << "\n";
				temp.str("");
				length = 0;
			}

			if(ObjectCodes[i+1].format != -1 && i+1 < ObjectCodeLength){
				textRecord << "T"<<std::setw(6)<<std::setfill('0')<< std::hex<<ObjectCodes[i+1].location + BlockTable[ObjectCodes[i+1].blockNumber].StartingAddress<<std::dec;
			}
			continue;
		}else if(tempObjCode != ""){
			length += tempObjCode.length()/2;
			if(length > 30){
				textRecord << std::setw(2)<<std::setfill('0')<<std::hex << length - (tempObjCode.length()/2)<< std::dec;
				textRecord << temp.str();
				textRecord << "\n";
				temp.str("");
				length = tempObjCode.length()/2;
				textRecord << "T"<<std::setw(6)<<std::setfill('0')<< std::hex<<ObjectCodes[i].location + BlockTable[ObjectCodes[i].blockNumber].StartingAddress<<std::dec;
			}else if(ObjectCodes[i].blockNumber != currentBlockNumber){
				if(length != 0 ){
					currentBlockNumber = ObjectCodes[i].blockNumber;
					textRecord <<std::setw(2)<<std::setfill('0')<< std::hex << length - (tempObjCode.length()/2)<< std::dec;
					textRecord << temp.str();
					textRecord << "\n";
					temp.str("");
					length = tempObjCode.length()/2;
				}
				textRecord << "T"<< std::setw(6)<<std::setfill('0')<<std::hex<<ObjectCodes[i].location + BlockTable[ObjectCodes[i].blockNumber].StartingAddress<<std::dec;
			}
			temp << tempObjCode;
		}else {
			continue;
		}

		
	}
	if(length > 0){
		textRecord << std::setw(2)<<std::setfill('0')<<std::hex << length << std::dec;
		textRecord << temp.str();
		textRecord << "\n";
	}
	return textRecord.str();
}




void GenerateObjectProgram(std::vector<ParseResult>& ParseArr,std::vector<Literal>& LITTAB,std::map<int,ProgBlock>& BlockTable,std::map<std::string,SymTabRow>& SYMTAB,std::ofstream* outfile){
	std::string progName ;
	std::string startingAddr ;
	std::ostringstream headerRecord;
	std::ostringstream modificationRecord;
	
	std::vector<ParseResult>::iterator position;

	position = std::find_if(ParseArr.begin(),ParseArr.end(),[](ParseResult val){
		return(ToUpperCase(val.mnemonic) == "START");
	});

	if(position == ParseArr.end()){
		std::cout<<"NO START DIRECTIVE FOUND. PROGRAM NAME IS ASSUMED AS 'PROGA'. STARTING ADDRESS IS 0x0";
		progName = "PROGA";
		startingAddr = "0";
	}else{
		progName = position->label;
		startingAddr = position->operand1;
	}


	if(progName.length() >6){
		progName.resize(6);
	}

	headerRecord<<"H"<<progName;
	headerRecord<<std::setw(6)<<std::setfill('0')<<startingAddr;

	
	int progLength = BlockTable[BlockTable.size() - 1].StartingAddress + BlockTable[BlockTable.size() - 1].Length ; 
	headerRecord<<std::setw(6)<<std::setfill('0')<<std::hex<<progLength<<std::dec<<std::endl;

	WriteLine(outfile,headerRecord.str());

	std::vector<ObjCode> ObjectCodes;
	bool baseRegSet = false;
	int BASE = 0;
	int PCIndex;
	int PC;
	
	for(int i = 0 ;i < ParseArr.size();i++){
		ObjCode obj {};

		
		ParseResult &parseItem = ParseArr[i];

		PCIndex = i != ParseArr.size() ? i+1 : ParseArr.size();
		PC = ParseArr[PCIndex].location;

		if(parseItem.type == "Comment"){
			continue;
		}

		obj.location = parseItem.location;
		obj.blockNumber = parseItem.block;

		if(parseItem.type == "Directive"){
			if(ToUpperCase(parseItem.mnemonic) == "BASE"){
				baseRegSet = true;
				continue;
			}else if(ToUpperCase(parseItem.mnemonic) == "BYTE"){
				std::pair<int,std::string> byteLenVal =  getByteDirectiveValue(parseItem.operand1);
				obj.format = byteLenVal.first;
				obj.value = byteLenVal.second;
			
			}else if(ToUpperCase(parseItem.mnemonic) == "WORD"){
				std::string wordVal = parseItem.operand1;
				obj.format = 3;
				obj.value = wordVal;
			}else if(ToUpperCase(parseItem.mnemonic) == "RESB" ||ToUpperCase(parseItem.mnemonic) == "RESW" || ToUpperCase(parseItem.mnemonic) == "USE" ){

			}else{
				continue;
			}

			// std::ostringstream out;
			// out<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
			// out<<parseItem;
			// out<<obj;
			// out<<GenerateOpCode(obj)<<std::endl;
			// out<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
			// WriteLine(outfile,out.str());
			
			ObjectCodes.push_back(obj);
			continue;
		}
		
		if(ToUpperCase(parseItem.mnemonic).find("LDB") != std::string::npos){
			if(!baseRegSet){
				throw std::runtime_error("BASE DIRECTIVE MISSING");
			}
			
			std::pair<int,int> tempPair ;
			try{
				tempPair = getDisplacement(PC,BASE,parseItem.operand1,LITTAB,SYMTAB,BlockTable);
			}catch(const char * err){
				throw std::runtime_error(err + parseItem.label + "   " + parseItem.mnemonic + "  " + parseItem.operand1 + " " + parseItem.operand2);
			}

			BASE = tempPair.second;
		}

		if(parseItem.mnemonic[0] == '='){
			std::pair<std::string,int> literalVals =  getLiteralValue(parseItem.mnemonic,LITTAB);
			obj.value = literalVals.first;
			obj.format = literalVals.second;
			// std::cout<<obj;
			ObjectCodes.push_back(obj);
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
				obj.flags[5] = 0;
				obj.flags[4] = 1;
			}else if(parseItem.operand1[0] == '@'){
				obj.flags[5] = 1;
				obj.flags[4] = 0;
			}else{
				obj.flags[5] = 1;
				obj.flags[4] = 1;
			}

			if(parseItem.operand1 == "X" || parseItem.operand2 == "X"){
				obj.flags[3] = 1;
			}

			if(parseItem.mnemonic[0] == '+'){
				obj.flags[2] = 0; 
				obj.flags[1] = 0; 
				obj.flags[0] = 1;
				obj.displacement = getAbsoluteDisplacement(parseItem.operand1,SYMTAB,LITTAB,BlockTable);
				modificationRecord << "M" << std::setw(6) << std::setfill('0')<< std::hex << obj.location + 1 << std::dec << "05\n";
			}else{
				// calc displacement for format 3 instructions

				// exception to this statement, as RSUB is a format 3/4 inst but it doesnt have an argument
				// thus parseItem.operand1 is empty and throws and error in getDisplacement
				// spent 2 hours finding this, figured it out. am proud.
				if(ToUpperCase(parseItem.mnemonic) == "RSUB"){
					obj.flags[2] = 0; 
					obj.flags[1] = 0; 
					obj.flags[0] = 0;
					obj.displacement = 0;
				}else{
					std::pair<int,int> disp = getDisplacement(PC,BASE,parseItem.operand1,LITTAB,SYMTAB,BlockTable); 
					switch(disp.first){
						case 0:{
								obj.flags[2] = 0; 
								obj.flags[1] = 0; 
								obj.flags[0] = 0;
								break;
						}
						case 1:{
								obj.flags[2] = 0; 
								obj.flags[1] = 1; 
								obj.flags[0] = 0;
								break;
						}
						case 2:{
								obj.flags[2] = 1; 
								obj.flags[1] = 0; 
								obj.flags[0] = 0;
								break;
						}
					}

					obj.displacement = disp.second;
				}
			}
		}
		// std::ostringstream out;
		// out<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
		// out<<parseItem;
		// out<<obj;
		// out<<"INDEX --> "<<ObjectCodes.size() + 1 <<std::endl;
		// out<<GenerateOpCode(obj)<<std::endl;
		// out<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
		// WriteLine(outfile,out.str());

		ObjectCodes.push_back(obj);
	}

	std::ostringstream textRecord ;
	textRecord << generateTextRecord(startingAddr,ObjectCodes,BlockTable);
	WriteLine(outfile,textRecord.str());

	WriteLine(outfile,modificationRecord.str());

	std::ostringstream endRecord ;
	endRecord << "E" << std::setw(6) << std::setfill('0') << startingAddr;
	WriteLine(outfile,endRecord.str());

}