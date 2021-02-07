#include "Pass1.h"

void AssignLOCCTR(std::vector<ParseResult>& parseArr){
	int LOCCTR = 0;
	if(parseArr[0].mnemonic == "START"){
		LOCCTR = std::stoi(parseArr[0].operand1,(size_t*)nullptr,16);
	}
	std::cout<<"LOCATION => "<<LOCCTR<<std::endl;
	for(auto obj : parseArr){
		if(obj.mnemonic == "START"){}

		obj.location = LOCCTR;
		// std::cout<<obj;
		if(obj.mnemonic[0] == '+'){
			LOCCTR += 0x4;
		}else if(obj.type!="Comment" && obj.type !="Directive"){
			GetInstFormat(obj.mnemonic);
		}


	}
}
