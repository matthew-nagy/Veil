#ifndef SPLIT_FILE_H
#define SPLIT_FILE_H true
#include <fstream>
#include <string>
#include <vector>

//Set up structs to easily pass and return values
struct File_Segment{
	std::vector<std::string> map;
	std::vector<std::string> guardians;
	std::vector<std::string> tears;
};


//Splits a .veil file into map, guardians and tears
File_Segment getSegmentedFile(std::string fileName){
	std::ifstream file;
	file.open(fileName);

	std::vector<std::string> currentSegment;
	std::string line;

	//Minor optimisation
	std::vector<std::string> segments[2];
	size_t arrayIndex = 0;
	
	//Get split them up into groups of lines
	while (std::getline(file, line)){
		if (line == "#"){
			segments[arrayIndex] = currentSegment;
			arrayIndex++;
			currentSegment.clear();
		}
		else{
			currentSegment.push_back(line);
		}
	}

	//currentSegment has the last segment, which is the tears. The others are in the array
	File_Segment toReturn;
	toReturn.map = segments[0];
	toReturn.guardians = segments[1];
	toReturn.tears = currentSegment;

	file.close();

	return toReturn;
}


#endif