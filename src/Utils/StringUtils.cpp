#include "StringUtils.h"

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
		if (delims.find(*pch) != delims.end())
		{
		if (start != pch)
		{
			std::string str(start, pch);
			result.push_back(str);
		}
		else
		{
			result.push_back("");
		}
		start = pch + 1;
		}
	}
	result.push_back(start);

	return result;
}