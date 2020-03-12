#ifndef GLOBALS_FILE_ACCESS_H
#define GLOBALS_FILE_ACCESS_H true

#include "Globals.h"
#include <vector>
#include <string>
#include <fstream>

const std::string fileName = "Data/Data.dat";

void setupGlobals(){
	std::ifstream file;
	file.open(fileName);
	std::vector<std::string> lines;
	std::string line;

	while (std::getline(file, line)){
		lines.push_back(line);
	}

	global::levelOn = std::atoi(lines[0].c_str());

	file.close();
}

void saveGlobals(){
	std::ofstream file;
	file.open(fileName);

	file << std::to_string(global::levelOn)+"\n";

	file.close();
}

#endif