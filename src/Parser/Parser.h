#ifndef PARSER_H_

#define PARSER_H_

#include<iostream>
#include<vector>
#include<algorithm>
#include<string>

#include "../Models/Structs.h"
#include "../Utils/Utils.h"



ParseResult parse(std::string line);
bool checkIfAssemblerDirective(std::string mnemonic);

#endif