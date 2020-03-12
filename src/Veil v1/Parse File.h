#ifndef PARSE_FILE_H
#define PARSE_FILE_H true
#include "Level class.h"
#include "Split File.h"

sf::Vector2f stringToVec(std::string& toParse){
	std::string x = "";
	std::string y = "";

	for (size_t i = 0; i < toParse.size(); i++){
		if (toParse[i] == ','){
			x = y;
			y = "";
		}
		else{
			y += toParse[i];
		}
	}

	return sf::Vector2f(std::atoi(x.c_str()), std::atoi(y.c_str()));
}

Tile_Map* getTileMap(std::vector<std::string> stringMap, sf::Texture& tex, Real_Coords& playerPosition, Real_Coords& stairPosition, int numOfTears){
	
	std::vector<std::vector<Tile*>> tiles;
	for (size_t y = 0; y < stringMap.size(); y++){
		std::vector<Tile*> row;
		for (size_t x = 0; x < stringMap[y].size(); x++){

			switch (stringMap[y][x]){
			case '0':
				row.push_back(new Tile(Real_Coords(x * 60, y * 60), tex, sf::IntRect(0, 0, 60, 60), true));
				break;
			case '1':
				row.push_back(new Tile(Real_Coords(x * 60, y * 60), tex, sf::IntRect(60, 0, 60, 60), false));
				break;
			case '2':
				playerPosition = Real_Coords((x * 60) + 30, (y * 60) + 30);
				//flow into next case
			case '5':
				row.push_back(new Tile(Real_Coords(x * 60, y * 60), tex, sf::IntRect(120, 0, 60, 60), false, tt_invunrable));
				break;
			case '3':
				row.push_back(new Stair_Tile(Real_Coords(x * 60, y * 60),tex,numOfTears));
				stairPosition = Real_Coords((x * 60) + 30, (y * 60) + 30);
				break;
			case '4':
				row.push_back(new Breakable_Tile(Real_Coords(x * 60, y * 60), tex));
			}
		}
		tiles.push_back(row);
	}

	return new Tile_Map(tiles);

}

Guardian_Base* getGuardian(std::string& line,bool* gameOver){
	std::vector<std::string> data;
	std::string current = "";
	for (size_t i = 0; i < line.size(); i++){
		if (line[i] == ' '){
			data.push_back(current);
			current = "";
		}
		else{
			current += line[i];
		}
	}//current will still have the last string
	data.push_back(current);

	//Get the positions. Assume that the formatting is correct because the program which creates them will not allow
	//files to be made without this information
	sf::Vector2f startPosition = stringToVec(data[1]);
	std::vector<sf::Vector2f> patrolPath;
	for (size_t i = 2; i < data.size(); i++){
		patrolPath.push_back(stringToVec(data[i]));
	}

	if (data[0] == "Seeker"){
		return new Seeker(startPosition, patrolPath, gameOver);
	}
	else if (data[0] == "Spectre"){
		return new Spectre(startPosition, patrolPath, gameOver);
	}
	//else
	return new Stalker(startPosition, patrolPath, gameOver);
}

std::vector<Tear*> getTears(std::vector<std::string>& tearStringVector){
	std::vector<Tear*> tears;
	for (size_t i = 0; i < tearStringVector.size(); i++){
		tears.push_back(new Tear(stringToVec(tearStringVector[i])));
	}

	return tears;
}

Level_Map* getLevelMap(std::string fileName, sf::Texture& tex, std::vector<sf::Sprite>& decorations){
	File_Segment fileSegments = getSegmentedFile(fileName);
	bool* gameover = new bool(false);
	Real_Coords playerPosition;
	Real_Coords stairPosition;

	Tile_Map* tileMap = getTileMap(fileSegments.map, tex, playerPosition, stairPosition, fileSegments.tears.size());

	std::vector<Guardian_Base*> guardians;
	for (size_t i = 0; i < fileSegments.guardians.size(); i++){
		guardians.push_back(getGuardian(fileSegments.guardians[i],gameover));
	}

	std::vector<Tear*> tears = getTears(fileSegments.tears);

	return new Level_Map(tileMap, guardians, tears, playerPosition, gameover, stairPosition, decorations);
}

#endif