#include "Opcode.h"

std::string GenerateOpCode(ObjCode& obj){
	// struct ObjCode{
	// 		int operation;
	// 		int displacement;
	// 		std::bitset<6> flags; // nixbpe flags
	// 		int format; // format 1 <---> 4 // also stores length in case of BYTE,WORD
	// 		int reg1;
	// 		int reg2;
	// 		int location;
	// 		int blockNumber;
	// 		std::string value; // Used for literal pools, BYTE, WORD
	// 		friend std::ostream& operator << (std::ostream& out,const ObjCode &obj);
	// };
	std::ostringstream out;
	std::string opcode = "";
	if(obj.operation != -1){
		switch(obj.format){
			case 1: {
				opcode += std::bitset<8>(obj.operation).to_string();
				break;
			}
			case 2: {
				opcode += std::bitset<8>(obj.operation).to_string();
				opcode += std::bitset<4>(obj.reg1).to_string();
				opcode += std::bitset<4>(obj.reg2).to_string();
				break;
			}
			case 3: {

				opcode += std::bitset<8>(obj.operation).to_string().substr(0,6);
				opcode += obj.flags.to_string();
				opcode += std::bitset<12>(obj.displacement).to_string();
				break;
			}
			case 4: {
				opcode += std::bitset<8>(obj.operation).to_string().substr(0,6);
				opcode += obj.flags.to_string();
				opcode += std::bitset<20>(obj.displacement).to_string();
				break;
			}
		}
		out<<std::setw(obj.format * 2)<<std::setfill('0')<<std::hex<<std::bitset<32>(opcode).to_ulong()<<std::dec;

	}else{
		opcode += obj.value;
		if(obj.format * 2 == obj.value.length()){
			out<<opcode;
		}else{
			out<<std::setw(obj.format*2)<<std::setfill('0')<<opcode;
		}
	}

	
	return out.str();
}