#ifndef STRING_UTILS_H_

#define STRING_UTILS_H_

#include <string>
#include <algorithm>
#include <cctype>
#include <locale>
#include <vector>
#include <iostream>
#include <set>

std::string ToUpperCase(std::string);

std::vector<std::string> Splitpath(const char * str);


void trim(std::string &s);


#endif