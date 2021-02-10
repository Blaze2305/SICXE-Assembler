#include "Structs.h"

std::ostream& operator << (std::ostream& out,const ParseResult &obj){
	out<<"\n---------------------------------------PARSE RESULT---------------------------------------\n";
	out<<"Label --> "<<obj.label<<std::endl;
	out<<"Mnemonic --> "<<obj.mnemonic<<std::endl;
	out<<"Op1 --> "<<obj.operand1<<std::endl;
	out<<"Op2 --> "<<obj.operand2<<std::endl;
	out<<"Type --> "<<obj.type<<std::endl;
	out<<"Block --> "<<obj.block<<std::endl;
	out<<"Location --> 0x"<<std::hex<<obj.location<<std::dec<<std::endl;
	out<<"\n------------------------------------------------------------------------------\n";
	return out;
}

std::ostream& operator << (std::ostream& out,const InstInfo &obj){
	out<<"\n---------------------------------------INSTRUCTION INFO---------------------------------------\n";
	out<<"Mnemonic --> "<<obj.mnemonic<<std::endl;
	out<<"Format --> "<<obj.format<<std::endl;
	out<<"OpCode --> "<<std::hex<<obj.opCode<<std::dec<<std::endl;
	out<<"\n------------------------------------------------------------------------------\n";
	return out;
}

std::ostream& operator << (std::ostream& out,const ProgBlock &obj){
	out<<"\n---------------------------------------PROGRAM BLOCK INFO---------------------------------------\n";
	out<<"Block Name --> "<<obj.BlockName<<std::endl;
	out<<"Block Number --> "<<obj.BlockNumber<<std::endl;
	out<<"Block Starting Address --> "<<std::hex<<obj.StartingAddress<<std::dec<<std::endl;
	out<<"Block Length --> "<<std::hex<<obj.Length<<std::dec<<std::endl;
	out<<"\n------------------------------------------------------------------------------\n";
	return out;
}
