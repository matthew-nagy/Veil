#ifndef TUTORIAL_3
#define TUTORIAL_3 true
#include "Level class.h"

Level_Map* getTutorial3(sf::Texture& tex, std::vector<sf::Sprite>& decorations){

	bool* gameover = new bool(false);
	std::vector<std::vector<suint>> map{
			//  0  1  2  3  4  5  6  7  8  9
		/* 0*/{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		/* 1*/{ 1, 1, 3, 1, 1, 0, 0, 0, 1, 1 },
		/* 2*/{ 1, 1, 0, 1, 1, 1, 1, 0, 1, 1 },
		/* 3*/{ 1, 0, 0, 1, 1, 1, 1, 0, 1, 1 },
		/* 4*/{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		/* 5*/{ 1, 0, 0, 1, 1, 1, 1, 0, 0, 1 },
		/* 6*/{ 1, 0, 1, 1, 1, 1, 1, 0, 0, 1 },
		/* 7*/{ 1, 0, 2, 0, 2, 0, 0, 0, 0, 1 },
		/* 8*/{ 1, 4, 1, 1, 1, 0, 0, 0, 0, 1 },
		/* 9*/{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	};
	std::vector<Tear*> tears{
		new Tear(Real_Coords((3 * 60) + 30, (4 * 60) + 30)),
		new Tear(Real_Coords((5 * 60) + 30, (1 * 60) + 30)),
		new Tear(Real_Coords((5 * 60) + 30, (8 * 60) + 30))
	};
	std::vector<Guardian_Base*> guardians{
		new Stalker(Real_Coords(3.5 * 60, 7.5 * 60), std::vector < Real_Coords > {Real_Coords(3.2 * 60, 7.2 * 60), Real_Coords(1.2 * 60, 5.2 * 60), Real_Coords(7.2 * 60, 5.2 * 60)}, gameover),
		new Seeker(Real_Coords((6 * 60) + 30, (4 * 60) + 30), std::vector < Real_Coords > {Real_Coords((6 * 60) + 30, (4 * 60) + 30), Real_Coords(7.2 * 60, 6.2 * 60), Real_Coords(6.2 * 60, 7.2 * 60)}, gameover)
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

	return new Level_Map(tileMap, guardians, tears, startPosition, gameover, stairPosition, decorations);
}

#endif