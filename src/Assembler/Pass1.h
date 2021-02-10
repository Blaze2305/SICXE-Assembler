#ifndef PASS_1_H_

#define PASS_1_H_

#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "../Models/Structs.h"
#include "../Models/OpTable.h"
#include "../Utils/StringUtils.h"
#include "../IO/IO.h"

void AssignLOCCTR(std::vector<ParseResult>&,std::map<int,ProgBlock>&,std::vector<Literal>&);

#endif