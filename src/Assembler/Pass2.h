#ifndef PASS_2_H_

#define PASS_2_H_

#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "../Models/Structs.h"
#include "../Models/OpTable.h"
#include "../Parser/Parser.h"
#include "../Utils/Utils.h"
#include "../IO/IO.h"


void GenerateObjectProgram(std::vector<ParseResult>& ,std::vector<Literal>& ,std::map<int,ProgBlock>& ,std::map<std::string,SymTabRow>&,std::ofstream*);

#endif