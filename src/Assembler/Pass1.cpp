#include "Pass1.h"


// Function to get the new block number and location counter values for the new block found
// can be a fresh block address or resuming a previously used block
// 
// params:
// 		oldBlockNumber : <int> The old block number which was being used
// 		LOCCTR : <int> Current value of the location counter
// 		BlockTable : std::map<int,ProgBlock> BlockTable
// 		newBlockName : std::string New block name
// returns a int,int pair
std::pair<int,int> getNewBlockNumberAndLOCCTR(int oldBlockNumber ,int LOCCTR, std::map<int,ProgBlock>& BlockTable, std::string newBlockName){
	
	// assign length to old block
	BlockTable[oldBlockNumber].Length = LOCCTR;
	
	// search for the new block, if its already used it'll be present and then we return its length and block number
	for(auto obj : BlockTable){
		if(obj.second.BlockName == newBlockName){
			return std::pair<int,int>{obj.second.BlockNumber,obj.second.Length};
		}
	}
	// else create a new block entry and store in the block table
	int blockNum = BlockTable.size();
	BlockTable[blockNum] = {
		newBlockName,
		blockNum,
		0,
		0
	};

	return std::pair<int,int>{blockNum,0};
}

// Function to check if the literal exists in the LITTAB
// params:
// 	LITTAB : std::vector<Literal> LITTAB
// 	literalVal : std::string The value of the literal to be found
// returns a boolean
bool checkIfLiteralExists(std::vector<Literal> LITTAB,std::string literalVal){
	return std::find_if(LITTAB.begin(),LITTAB.end(),[literalVal](Literal val){
		return(val.Value == literalVal);
	}) != LITTAB.end();
}


// Function to add a literal to the LITTAB
// params:
// 		LITTAB : std::vector<Literal> LITTAB
// 		literal : std::string The literal value to be added to the littab
void addToLITTAB(std::vector<Literal>& LITTAB,std::string literal){
	std::string value = "";
	int length;
	// convert the literal to its base hex value
	// different conversions for Char , Hex, Location based literals
	if(literal[1] == 'C'){
		// concat hex value of each char 
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
		std::ostringstream out;
		out<<std::hex<<std::stoi(literal);
		value = out.str();
	}
	// if the literal already exists, no need for a duplicate so we dont create one
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


std::pair<int,char> evalSymbol(std::string symbVal,std::map<std::string,SymTabRow>& SYMTAB){
	std::vector<std::string> expression = infixToPostfix(symbVal);
	std::vector<std::string> exprCopy = expression;
	int value ;
	char type = 'R';
	for(int i=0;i<expression.size();i++){
		if(is_number(expression[i])){
			int exprHexVal = std::stoi(expression[i],(size_t*)nullptr,16);
			expression.at(i) = std::to_string(exprHexVal);
		}else if(!isOperator(expression[i])){
			int value = SYMTAB[expression[i]].Value;
			expression.at(i) = std::to_string(value);
		}
	}

	value = evaluatePostfix(expression);

	for(int i=0;i<exprCopy.size();i++){
		if(is_number(exprCopy[i])){
			exprCopy.at(i) = std::to_string(0);
		}else if(!isOperator(exprCopy[i])){
			exprCopy.at(i) = std::to_string(1);
		}
	}
	int tempValue = evaluatePostfix(exprCopy);
	if(tempValue == 0){
		type = 'A';
	}
	return std::pair<int,char>{value,type};
}


void addToSymbolTable(std::string label, std::string value, int blockNum , std::map<std::string,SymTabRow>& SYMTAB, std::string type){
	int tempValue;
	char valueType;
	// TO DO : evaluate value of symbol
	if(type == "Simple" || type == "Addr"){
		tempValue = std::stoi(value);
		valueType = 'R';
	}else if (type == "Number"){
		tempValue = std::stoi(value);
		valueType = 'A';
	}else{
		std::pair<int,char> symbolValue = evalSymbol(value,SYMTAB);
		tempValue = symbolValue.first;
		valueType = symbolValue.second;
	}
	SymTabRow obj = {
		label,
		blockNum,
		valueType,
		tempValue
	};

	SYMTAB[label] = obj;
}

// Function to assign an address to the Literals in the LITTAB
int assignAddrToLiterals(std::vector<Literal>& LITTAB,int &LOCCTR,std::vector<ParseResult> &ParseArr,int currIndex,int blockNumber){
	int count = 0;
	for(auto &obj : LITTAB){
		if(obj.Address == -1){
			count += 1;
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

// Function to assign locctr values to the instructions, main part of pass 1 , assigns each and every instruction an address
// adds the symbols to the SYMTAB , pushes literals to LITTAB , takes into account the program blocks and assigns proper addresses
// params:
// 		ParseArr : std::vector<ParseResult> List of parsed instructions
// 		BlockTable : std::map<int,ProgBlock> Block Table
// 		LITTAB : std::vector<Literal> Literal Table
// 		SYMTAB : std::map<std::string,SymTabRow> Symbol Table
void AssignLOCCTR(std::vector<ParseResult>& ParseArr,std::map<int,ProgBlock>& BlockTable,std::vector<Literal>& LITTAB,std::map<std::string,SymTabRow>& SYMTAB){
	int LOCCTR = 0x0;
	int currentBlockNumber = 0;

	// adds a default block table
	BlockTable[0] = {
		"Default",
		0,
		0,
		0
	};

	// go through each parsed item in the parseArr list and assign an address
	for(int i=0;i<ParseArr.size();i++){
		ParseResult &obj = ParseArr[i];
		// get the starting address from the START directive
		if(ToUpperCase(obj.mnemonic) == "START"){
			BlockTable[0].StartingAddress = std::stoi(obj.operand1,(size_t*)nullptr,16);
			continue;
		}

		// ignore a comment
		if(obj.type == "Comment"){
			continue;
		}

		// set the locctr value for the instruction
		obj.location = BlockTable[currentBlockNumber].StartingAddress + LOCCTR;
		obj.block = currentBlockNumber;

		


		if(obj.label != "" && obj.mnemonic != "EQU"){
			addToSymbolTable(obj.label,std::to_string(LOCCTR),currentBlockNumber,SYMTAB,"Simple");
		}

		// Handles literals
		if(obj.operand1[0] == '='){
			if(obj.operand1[1] == '*'){
				// Location based literal 
				addToLITTAB(LITTAB,std::to_string(LOCCTR));
			}else{
				addToLITTAB(LITTAB,obj.operand1);
			}
		}

		// Handle Format 4 intructions
		if(obj.mnemonic[0] == '+'){
			LOCCTR += 0x4;
		}else if(obj.type == "Directive"){
			// Handle assembler directives 

			// Handle BYTE directive
			if(ToUpperCase(obj.mnemonic) == "BYTE"){
				// get the number of bytes to be added for each type of BYTE inst
				if(obj.operand1[0] == 'C'){
					// Char bytes
					LOCCTR += obj.operand1.length() -3;
				}else if(obj.operand1[0]=='X'){
					// Hex bytes
					LOCCTR += (obj.operand1.length() - 3)/2; 
				}else if(obj.operand1[0] == '=' && obj.operand1[1] == 'C'){
					// Literal Char bytes
					LOCCTR += obj.operand1.length() -4;
				}else if(obj.operand1[0] == '=' && obj.operand1[1] == 'X'){
					// Literal Hex bytes
					LOCCTR += (obj.operand1.length() - 4)/2; 
				}else{
					// Wrong use of BYTE, go back to school
					throw std::runtime_error("WRONG USE OF BYTE => "+obj.label+" "+obj.mnemonic+" "+obj.operand1);
				}

			// Handle Word, RESB, RESW
			}else if(ToUpperCase(obj.mnemonic) == "WORD"){
				LOCCTR += 3;
			}else if(ToUpperCase(obj.mnemonic) == "RESB"){
				LOCCTR += std::stoi(obj.operand1);
			}else if (ToUpperCase(obj.mnemonic) == "RESW"){
				LOCCTR += 3 * std::stoi(obj.operand1);
			}else if(ToUpperCase(obj.mnemonic) == "USE"){
				// Handles program blocks

				// If nothing infront of USE, then its a default block
				if(obj.operand1 == ""){
					std::pair<int,int> temp = getNewBlockNumberAndLOCCTR(currentBlockNumber,LOCCTR,BlockTable,"Default");
					currentBlockNumber = temp.first;
					LOCCTR = temp.second;
				}else{
					// else its a different block
					std::pair<int,int> temp = getNewBlockNumberAndLOCCTR(currentBlockNumber,LOCCTR,BlockTable,obj.operand1);
					currentBlockNumber = temp.first;
					LOCCTR = temp.second;
				}
				obj.block = currentBlockNumber;
				continue;

			// Handle LTORG and END , both which cause litrals in the LITTAB to be assigned address/value
			}else if(ToUpperCase(obj.mnemonic) == "LTORG" || ToUpperCase(obj.mnemonic) == "END"){
				int skip = assignAddrToLiterals(LITTAB,LOCCTR,ParseArr,i+1,currentBlockNumber);
				// this also pushes some psuedo-parseitems to the ParseArr to make it easier to handler later in pass 2, 
				// so we get the number of items to skip in the list and do so 
				i += skip;

			// handle User defined symbols
			}else if(ToUpperCase(obj.mnemonic) == "EQU"){
				if(obj.operand1 == "*"){
					addToSymbolTable(obj.label,std::to_string(BlockTable[currentBlockNumber].StartingAddress + LOCCTR),currentBlockNumber,SYMTAB,"Addr");
				}else if(is_number(obj.operand1)){
					addToSymbolTable(obj.label,obj.operand1,currentBlockNumber,SYMTAB,"Number");
				}else{
					std::string tempOperand1 = obj.operand1;
					// remove extra spaces in the operand ie allow "a - b" also instead of just "a-b"
					std::string::iterator end_pos = std::remove(tempOperand1.begin(), tempOperand1.end(), ' ');
					tempOperand1.erase(end_pos, tempOperand1.end());
					addToSymbolTable(obj.label,tempOperand1,currentBlockNumber,SYMTAB,"Calc");
				}

			}else{
				continue;
			}
		}else{
			// default get the number of bytes the instruction needs to be assigned
			LOCCTR += GetInstFormat(obj.mnemonic);
		}
	}

	
	// set the length of the ending block
	BlockTable[currentBlockNumber].Length = LOCCTR;

	// calc the starting addr of each block using the first block and each blocks length
	for(int i=1;i<BlockTable.size();i++){
		BlockTable[i].StartingAddress = BlockTable[i-1].StartingAddress + BlockTable[i-1].Length;
	}

	// for(auto item : ParseArr){
	// 	std::cout<<item;
	// }

	// DEBUG :: PRINT THE BLOCK TABLE
	// std::cout<<"BLOCK TABLE\n\n\n";
	// for(auto item: BlockTable){
	// 	std::cout<<item.second.BlockName<<"    ";
	// 	std::cout<<item.second.BlockNumber<<"    ";
	// 	std::cout<<std::hex<<item.second.StartingAddress<<std::dec<<"    ";
	// 	std::cout<<std::hex<<item.second.Length<<std::dec<<"    \n\n\n";
		
	// }

	// DEFAULT :: PRINT THE LITERAL TABLE
	// std::cout<<"LITERAL TABLE\n\n";
	// for(auto item: LITTAB){
	// 	std::cout<<item.Name<<"     ";
	// 	std::cout<<item.Value<<"     ";
	// 	std::cout<<std::hex<<item.Address<<std::dec<<"     ";
	// 	std::cout<<std::hex<<item.Length<<std::dec<<"     ";
	// 	std::cout<<item.Block<<"     \n";
	// }
}
