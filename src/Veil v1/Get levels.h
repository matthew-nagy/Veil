#ifndef GET_LEVELS_H
#define GET_LEVELS_H true
#include "Parse File.h"

std::vector<std::string> getLevelNames(){
	std::ifstream file;
	file.open("Data/Level list.dat");

	std::vector<std::string> levels;
	std::string line = "";
	while (std::getline(file, line)){
		levels.push_back(line);
	}


	file.close();

	return levels;
}

#endif