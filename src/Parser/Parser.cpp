#include "Parser.h"

// Check if the instruction given doesnt have any operands
bool checkIfNeedNoOperand(std::string inst){
	// params:
	// 	inst : <std::string> The mnemonic of the instruction as its written
	// 
	// return:
	// 	 <bool> : True if inst doesnt have operand, False if it does

	// List of all the SIC/XE instructions without operands
	std::vector<std::string> noOperandInsts {"RSUB","NOBASE","HIO","FIX","NORM","TIO","SIO","FLOAT","START","CSECT"};
	// Convert to uppercase for matching
	std::string temp = ToUpperCase(inst);
	// use std::find to find the index of the inst in the array, if it inst there it will be equal to noOperandInsts.end()
	// So we compare that and check if it has no operand or not
	return std::find(noOperandInsts.begin(),noOperandInsts.end(),temp) != noOperandInsts.end();
}

ParseResult parse(std::string line) {
	//This function read the line supplied as argument and split it into
	// fundamental elements of an instruction like lable, mnemonic, operand1 and
	//operand2 if they exists.
	unsigned int i;
	// Create an empty parse result structure
	ParseResult result = {"","","",""};

	std::string s[5] = { "", "", "", "", "" };
	//Initialising the array with empty string which will be replaced as string
	//is parsed
	i = 0;
	for (unsigned int j = 0; j < 5 && i < line.length(); j++) {
		// Skiping all blank spaces and tabs until it encounter characters
		// So that freely formated SIC/XE instruction can also be converted by
		// assembler
		for (; i < line.length() && (line[i] == ' ' || line[i] == '\t'); i++);


		for (; i < line.length() && (line[i] != ' ' && line[i] != '\t'); i++){
			//When some character is encountered words are being added in the
			// respective location in s[]
			s[j] += line[i];
			
		}

		
		// Runs during the first iteration, j==0
		if(j==0){
			// convert the string to uppercase
			std::string temp = ToUpperCase(s[0]);
			if(temp.compare("EXTREF")==0 || temp.compare("EXTDEF") == 0){
				result.mnemonic = s[0];
				for(int k=i; k<line.length(); k++)
					//if(line[k]!=' ' && line[k]!='\t')
						result.operand1 += line[k];
				return result;
		}
		
		
		// Runs during the second iteration , j==1
		}else if(j==1){
			std::string temp = ToUpperCase(s[1]);
			if(temp.compare("EXTREF")==0 || temp.compare("EXTDEF") == 0){
				result.label = s[0];
				result.mnemonic = s[1];
				for(int k=i; k<line.length(); k++)
					//if(line[k]!=' ' && line[k]!='\t')
						result.operand1 += line[k];
				return result;
			}
		}

	}
	// Skips all the spaces and tabs at the end of the line
	for (; i < line.length() && (line[i] == ' ' || line[i] == '\t'); i++);

	// Set "?" to indicate a comment line, so checking if the parsed line was a comment here
	if(s[0] != "" && s[0][0] == '?'){        // comment line
		for (i=0; i < line.length() && (line[i] == ' ' || line[i] == '\t'); i++);
		for (; i < line.length(); i++){
			result.mnemonic += line[i];
		}
		result.type = "Comment";
		return result;
	}


	result.type = "Instruction";
	// Used to store whatever is found after the mnemonic
	std::string temp_string;
	// Empty parse, return empty result
	if (s[0] == ""){	
		
		return result;
	}
	// only one item in the parse result is set to the mnemonic and returned
	// could just be a label
	else if (s[1] == ""){
		
		result.mnemonic = s[0];
	}
	// if parse result has only two items , then we check if the second entry in the parse is a valid inst
	// which requires no operand
	else if (s[2] == "") {
		

		// if the element doesnt require an operand then we set that to the mnemonic of the operand
		if (checkIfNeedNoOperand(s[1])) {
			result.label = s[0];
			result.mnemonic = s[1];
		} else {
			// else it has to be the the inst and a single operand 
			result.mnemonic = s[0];
			temp_string = s[1];
		}
	} else if (s[3] == "") {
		if (s[1][s[1].length() - 1] == ',' || s[2][0] == ',') {
			result.mnemonic = s[0];
			temp_string = s[1] + s[2];
		} else {
			result.label = s[0];
			result.mnemonic = s[1];
			temp_string = s[2];
		}
	} else if (s[4] == "") {
		

		if (s[2].compare(",") == 0) {
			result.mnemonic = s[0];
			temp_string = s[1] + s[2] + s[3];
		} else {
			result.label = s[0];
			result.mnemonic = s[1];
			temp_string = s[2] + s[3];
		}
	} else {
		if(s[3].compare(",") != 0)
			throw new std::string("Too Many Arguments");
						// line containging too many arguments

		result.label = s[0];
		result.mnemonic = s[1];
		temp_string = s[2] + s[3] + s[4];
	}


	// Parse the temp string to split into however many operands there are
	std::string temp_op1 = "";
	std::string temp_op2 = "";
	// copy untill we reach ","
	for (i = 0; i < temp_string.length() && temp_string[i] != ','; i++){
		temp_op1 += temp_string[i];
	}
	if(i == temp_string.length()-1 && temp_string[i] == ','){
		temp_op2 = ",";
	}
	else
		for (i++; i < temp_string.length(); i++){
			temp_op2 += temp_string[i];
		}
	// set the opernds
	result.operand1 = temp_op1;
	result.operand2 = temp_op2;
	return result;
}



