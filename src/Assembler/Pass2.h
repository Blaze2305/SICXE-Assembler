#ifndef PASS_2_H_

#define PASS_2_H_

#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "../Models/Structs.h"
#include "../Models/OpTable.h"
#include "../Utils/StringUtils.h"
#include "../IO/IO.h"


void GenerateObjectProgram(std::vector<ParseResult>& ,std::vector<Literal>& ,std::map<int,ProgBlock>& ,std::ofstream*);

#endif