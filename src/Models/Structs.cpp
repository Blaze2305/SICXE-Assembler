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

std::ostream& operator << (std::ostream& out,const ObjCode &obj){
	out<<"\n---------------------------------------OBJECT CODE INFO---------------------------------------\n";
	out<<"Operation Code --> "<<obj.operation<<std::endl;
	out<<"Format --> "<<obj.format<<std::endl;
	out<<"Displacement --> "<<std::hex<<obj.displacement<<std::dec<<std::endl;
	out<<"Register 1 --> "<<obj.reg1<<std::endl;
	out<<"Register 2 --> "<<obj.reg2<<std::endl;
	out<<"Value --> "<<obj.value<<std::endl;
	out<<"Flags"<<std::endl;
	out<<"n --> "<<obj.flags[0]<<std::endl;
	out<<"i --> "<<obj.flags[1]<<std::endl;
	out<<"x --> "<<obj.flags[2]<<std::endl;
	out<<"b --> "<<obj.flags[3]<<std::endl;
	out<<"p --> "<<obj.flags[4]<<std::endl;
	out<<"e --> "<<obj.flags[5]<<std::endl;
	out<<"\n------------------------------------------------------------------------------\n";
	return out;
}

