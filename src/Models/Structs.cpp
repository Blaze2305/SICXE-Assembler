#include "Structs.h"


ObjCode::ObjCode(){
	operation = -1 ;
	displacement = -1 ; 
	format = -1; 
	reg1 = -1;
	reg2 = -1;
	location = -1;
	blockNumber = -1; 
	value = ""; 
}


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
	out<<"Operation Code --> "<<std::hex<<obj.operation<<std::dec<<std::endl;
	// out<<"Operation Code --> "<<obj.operation<<std::endl;
	out<<"Format --> "<<obj.format<<std::endl;
	out<<"Displacement --> "<<std::hex<<obj.displacement<<std::dec<<std::endl;
	out<<"Register 1 --> "<<obj.reg1<<std::endl;
	out<<"Register 2 --> "<<obj.reg2<<std::endl;
	out<<"Value --> "<<obj.value<<std::endl;
	out<<"Location --> "<<std::hex<<obj.location<<std::dec<<std::endl;
	out<<"Block Number --> "<<obj.blockNumber<<std::endl;
	out<<"Flags"<<std::endl;
	out<<"n --> "<<obj.flags[5]<<std::endl;
	out<<"i --> "<<obj.flags[4]<<std::endl;
	out<<"x --> "<<obj.flags[3]<<std::endl;
	out<<"b --> "<<obj.flags[2]<<std::endl;
	out<<"p --> "<<obj.flags[1]<<std::endl;
	out<<"e --> "<<obj.flags[0]<<std::endl;
	out<<"\n------------------------------------------------------------------------------\n";
	return out;
}


std::ostream& operator<< (std::ostream& out, const std::vector<std::string>& obj){
	out<<"\n---------------------------------------VECTOR INFO---------------------------------------\n";
	for(auto item :obj){
		out<<item<<std::endl;
	}
	out<<"\n------------------------------------------------------------------------------\n";
	return out;
}

std::ostream& operator<< (std::ostream& out, const std::stack<std::string>& obj){
	out<<"\n---------------------------------------STACK INFO---------------------------------------\n";
	int length = obj.size();
	std::stack<std::string> copyObj = obj;
	for(int i=0;i<copyObj.size();i++){
		out<<length - copyObj.size() + 1<<" -----> " << copyObj.top()<<std::endl;
		copyObj.pop();
	}
	out<<"\n------------------------------------------------------------------------------\n";
	return out;
}

std::ostream& operator<< (std::ostream& out, const SymTabRow& obj){
	out<<"\n------------------------------------------------------------------------------\n";
	out<<"Symbol Name --> "<<obj.SymbolName<<std::endl;
	out<<"Block Number --> "<<obj.BlockNumber<<std::endl;
	out<<"Type --> "<<obj.Type<<std::endl;
	out<<"Value --> "<<std::hex<<obj.Value<<std::dec<<std::endl;
	out<<"\n------------------------------------------------------------------------------\n";
	return out;
}



std::ostream& operator<< (std::ostream& out, const std::map<std::string,SymTabRow>& obj){
	out<<"\n--------------------------------------- SYMBOL TABLE ---------------------------------------\n";
	for (auto const& x : obj){
		out << x.first  // string (key)
				<< ':' 
				<< x.second // string's value 
				<< std::endl;
	}
	out<<"\n------------------------------------------------------------------------------\n";
	return out;
}

std::ostream& operator << (std::ostream& out,const Literal& obj){
	out<<"\n------------------------------------------------------------------------------\n";
	out<<"Literal Name --> "<<obj.Name<<std::endl;
	out<<"Literal Value --> "<<obj.Value<<std::endl;
	out<<"Literla Length --> "<<obj.Length<<std::endl;
	out<<"Block number --> "<<obj.Block<<std::endl;
	out<<"Literal Address --> "<<std::hex<<obj.Address<<std::dec<<std::endl;
	out<<"\n------------------------------------------------------------------------------\n";
	return out;
}

