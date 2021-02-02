#include "IO.h"

std::string ReadLine(std::ifstream  *file) {
	std::string line = "";
	std::getline(*file, line);
	return line;
}