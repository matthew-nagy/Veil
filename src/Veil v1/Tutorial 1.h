#ifndef TUTORIAL_1
#define TUTORIAL_1 true

#include "Level class.h"

Level_Map* getTutorial1LevelMap(sf::Texture& tex, std::vector<sf::Sprite>& decorations){
	std::vector<std::vector<suint>> map{
			 //0  1  2  3  4  5  6  7  8
		/*0*/{ 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		/*1*/{ 1, 1, 1, 1, 1, 0, 0, 0, 1 },
		/*2*/{ 1, 1, 1, 1, 1, 0, 0, 0, 1 },
		/*3*/{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
		/*4*/{ 1, 0, 4, 0, 0, 0, 3, 0, 1 },
		/*5*/{ 1, 0, 0, 0, 0, 0, 0, 0, 1 },
		/*6*/{ 1, 1, 1, 1, 1, 0, 0, 0, 1 },
		/*7*/{ 1, 1, 1, 1, 1, 0, 0, 0, 1 },
		/*8*/{ 1, 1, 1, 1, 1, 0, 0, 0, 1 },
		/*9*/{ 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};

	std::vector<std::vector<Tile*>> tiles;

	Real_Coords stairPosition;
	Real_Coords startPosition;
	for (size_t y = 0; y < map.size(); y++){
		std::vector<Tile*> row;
		for (size_t x = 0; x < map[y].size(); x++){
			switch (map[y][x]){
			case 0:
				row.push_back(new Tile(Real_Coords(x * 60, y * 60), tex, sf::IntRect(60, 0, 60, 60), false));
				break;
			case 1:
				row.push_back(new Tile(Real_Coords(x * 60, y * 60), tex, sf::IntRect(0, 0, 60, 60), true));
				break;
			case 2:
				row.push_back(new Breakable_Tile(Real_Coords(x * 60, y * 60), tex));
				break;
			case 3:
				row.push_back(new Stair_Tile(Real_Coords(x * 60, y * 60), tex, 3));
				stairPosition = Real_Coords((x * 60) + 30, (y * 60) + 30);
				break;
			case 4:
				row.push_back(new Tile(Real_Coords(x * 60, y * 60), tex, sf::IntRect(120, 0, 60, 60), false, tt_invunrable));
				startPosition = Real_Coords((x * 60) + 30, (y * 60) + 30);
				break;
			}
		}
		tiles.push_back(row);
	}

	Tile_Map* tileMap = new Tile_Map(tiles);

	Tear* t1 = new Tear(Real_Coords((4 * 60) + 30, (4 * 60) + 30));
	Tear* t2 = new Tear(Real_Coords((6 * 60) + 30, (2 * 60) + 30));
	Tear* t3 = new Tear(Real_Coords((6 * 60) + 30, (7 * 60) + 30));

	std::vector<Tear*> tears{ t1, t2, t3 };

	return new Level_Map(tileMap, std::vector < Guardian_Base* > {}, tears, startPosition, new bool(false), stairPosition, decorations);

}


#endif