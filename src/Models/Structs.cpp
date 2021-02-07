#include "Structs.h"

std::ostream& operator << (std::ostream& out,const ParseResult &obj){
	out<<"\n---------------------------------------PARSE RESULT---------------------------------------\n";
	out<<"Label --> "<<obj.label<<std::endl;
	out<<"Mnemonic --> "<<obj.mnemonic<<std::endl;
	out<<"Op1 --> "<<obj.operand1<<std::endl;
	out<<"Op2 --> "<<obj.operand2<<std::endl;
	out<<"Type --> "<<obj.type<<std::endl;
	out<<"Location --> 0x"<<std::hex<<obj.location<<std::dec<<std::endl;
	out<<"\n------------------------------------------------------------------------------\n";
	return out;
}

std::ostream& operator << (std::ostream& out,const InstInfo &obj){
	out<<"\n---------------------------------------INSTRUCTION INFO---------------------------------------\n";
	out<<"Mnemonic --> "<<obj.mnemonic<<std::endl;
	out<<"Format --> "<<obj.format<<std::endl;
	out<<"OpCode --> "<<obj.opCode<<std::endl;
	out<<"\n------------------------------------------------------------------------------\n";
	return out;
}
