#ifndef UTILS_H_

#define UTILS_H_

#include <string>
#include <algorithm>
#include <cctype>
#include <locale>
#include <vector>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <cctype>
#include "../Models/Structs.h"

std::string ToUpperCase(std::string);

std::vector<std::string> Splitpath(const char * str);


void trim(std::string &s);
std::vector<std::string> infixToPostfix(std::string str);
bool isOperator(std::string item);
bool is_number(const std::string& s);
int evaluatePostfix(std::vector<std::string> exp);

#endif