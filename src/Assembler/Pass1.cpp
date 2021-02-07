#include "Pass1.h"
#include<sstream>


void AssignLOCCTR(std::vector<ParseResult>& parseArr){
	int LOCCTR = 0x0;

	for(auto obj : parseArr){
		if(ToUpperCase(obj.mnemonic) == "START"){
			LOCCTR = std::stoi(parseArr[0].operand1,(size_t*)nullptr,16);
			continue;
		}

		obj.location = LOCCTR;

		if(obj.type == "Comment"){
			continue;
		}

		if(obj.mnemonic[0] == '+'){
			LOCCTR += 0x4;
		}else if(obj.type == "Directive"){
			if(obj.mnemonic == "BYTE"){
				if(obj.operand1[0] == 'C'){
					LOCCTR += obj.operand1.length() -3;
				}else if(obj.operand1[0]=='X'){
					LOCCTR += (obj.operand1.length() - 3)/2; 
				}else{
					throw std::runtime_error("WRONG USE OF BYTE => "+obj.label+" "+obj.mnemonic+" "+obj.operand1);
				}
			}else if(obj.mnemonic == "WORD"){
				LOCCTR += 3;
			}else if(obj.mnemonic == "RESB"){
				LOCCTR += std::stoi(obj.operand1);
			}else if (obj.mnemonic == "RESW"){
				LOCCTR += 3 * std::stoi(obj.operand1);
			}else{
				continue;
			}
		}else{
			LOCCTR += GetInstFormat(obj.mnemonic);
		}

	}
}
