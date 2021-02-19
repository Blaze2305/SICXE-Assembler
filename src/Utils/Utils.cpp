#include "Utils.h"
	
// Convert the string to uppercase
std::string ToUpperCase(std::string input){
	// convert the passed instruction mnemonic to uppercase
	std::transform(input.begin(), input.end(),input.begin(), ::toupper);
	return input;
}

// trim from start (in place)
static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

std::vector<std::string> Splitpath(const char * str){
	
	std::set<char> delims{'/'};

	std::vector<std::string> result;
	std::string temp = str;
	char const* pch = temp.c_str();
	char const* start = pch;
	for(; *pch; ++pch){
		if (delims.find(*pch) != delims.end()){
			if (start != pch){
				std::string str(start, pch);
				result.push_back(str);
			}
			else{
				result.push_back("");
			}
			start = pch + 1;
		}
	}
	result.push_back(start);

	return result;
}

bool isOperator(std::string item){
	std::vector<std::string> OPERATORS {"+","-","*","^","/"};
	return std::find(OPERATORS.begin(),OPERATORS.end(),item) != OPERATORS.end();
}


int getPrecedence(std::string c){ 
	if(c == "^"){
		return 3;
	}else if (c == "*" || c == "/"){
		return 2;
	}else if(c == "+" || c == "-"){
		return 1;
	}else{
		return -1;
	}
} 

std::vector<std::string> splitString(std::string exp,char delim){
	std::vector<std::string> split;
	std::string temp = "";
	for(auto item : exp){
		if(item == delim){
			split.push_back(temp);
			temp = "";
		}
		temp += item;
	}

	return split;
}


std::vector<std::string> infixToPostfix(std::string str){ 
	std::stack<std::string> infixStack; 
	std::vector<std::string> exp;
	std::string temp;
	infixStack.push("NULL"); 

	std::string operand = "";
	for(auto item:str){

		if(std::isalnum(item)){
			std::cout<<"ITEM --> "<<item<<std::endl;
			operand += item;
			continue;
		}else{
			if(operand!=""){
				exp.push_back(operand);
				operand = "";
			}
		}
		
		
		if(item == '('){
			infixStack.push("(");

		}else if(item == ')'){
			while(infixStack.top() != "NULL" && infixStack.top() != "("){

				temp = infixStack.top();
				infixStack.pop();
				exp.push_back(temp);

			}
			infixStack.pop();

		}else{

			temp = "";
			temp += item;
			while(infixStack.top() != "NULL" && getPrecedence(temp) <= getPrecedence(infixStack.top())){ 
				std::string c = infixStack.top(); 
				infixStack.pop(); 
				exp.push_back(c); 
			} 
			infixStack.push(temp);
		}
	}

	if(operand != ""){
		exp.push_back(operand);
	}

	// Pop all the remaining elements from the stack 
	while(infixStack.top() != "NULL"){ 
		std::string c = infixStack.top(); 
		infixStack.pop(); 
		exp.push_back(c);
	} 
	std::cout<<exp;
	return exp;

} 

bool is_number(const std::string& s){
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}


int evaluatePostfix(std::vector<std::string> exp){  
	std::stack<std::string> Stack; 
	int i;

	for(auto item :exp){
		if(is_number(item)){
			Stack.push(item);
		}else{
			int val1 = std::stoi(Stack.top());
			Stack.pop();
			int val2 = std::stoi(Stack.top());
			Stack.pop();
            switch (item[0]){  
				case '+': 
					Stack.push(std::to_string(val2 + val1)); 
					break;  
				case '-': 
					Stack.push(std::to_string(val2 - val1)); 
					break;  
				case '*': 
					Stack.push(std::to_string(val2 * val1)); 
					break;  
				case '/': 
					Stack.push(std::to_string(val2/val1)); 
					break;  
            }  
		}
	}
	return std::stoi(Stack.top());
}  


