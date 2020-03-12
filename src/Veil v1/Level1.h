#ifndef LEVEL_1
#define LEVEL_1 true

#include "Level class.h"

Level_Map* getLevel1(sf::Texture& tex,std::vector<sf::Sprite>& decorations){
	std::vector<std::vector<short>> map = {
		//		0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39
		/* 0*/{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		/* 1*/{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		/* 2*/{ 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 3, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1 },
		/* 3*/{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1 },
		/* 4*/{ 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1 },
		/* 5*/{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1 },
		/* 6*/{ 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 2, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1 },
		/* 7*/{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
		/* 8*/{ 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
		/* 9*/{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
		/*10*/{ 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1 },
		/*11*/{ 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1 },
		/*12*/{ 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1 },
		/*13*/{ 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1 },
		/*14*/{ 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
		/*15*/{ 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1 },
		/*16*/{ 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1 },
		/*17*/{ 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
		/*18*/{ 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1 },
		/*19*/{ 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		/*20*/{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },

	};
	Real_Coords stairTilePosition;

	std::vector<std::vector<Tile*>> tiles;
	for (size_t y = 0; y < map.size(); y++){
		std::vector<Tile*> row;
		for (size_t x = 0; x < map[0].size(); x++){
			if (map[y][x] == 0){
				row.push_back(new Tile(sf::Vector2f(x * 60, y * 60), tex, sf::IntRect(60, 0, 60, 60), false));
			}
			else if (map[y][x] == 1){
				row.push_back(new Tile(sf::Vector2f(x * 60, y * 60), tex, sf::IntRect(0, 0, 60, 60), true));
			}
			else if (map[y][x] == 2){
				row.push_back(new Tile(sf::Vector2f(x * 60, y * 60), tex, sf::IntRect(120, 0, 60, 60), false, tt_invunrable));
			}
			else if (map[y][x] == 3){
				row.push_back(new Stair_Tile(sf::Vector2f(x * 60, y * 60), tex, 4));
				stairTilePosition = sf::Vector2f((x * 60) + 1, (y * 60) + 1);
			}
		}
		tiles.push_back(row);
	}


	Tile_Map* tileMap = new Tile_Map(tiles);

	bool* gameOverFlag = new bool(false);

	Tear* t1 = new Tear(Real_Coords(270, 150));
	Tear* t2 = new Tear(Real_Coords((31 * 60) + 30, (3 * 60) + 30));
	Tear* t3 = new Tear(Real_Coords((24 * 60) + 30, (16 * 60) + 30));
	Tear* t4 = new Tear(Real_Coords((20 * 60) + 30, (8.5 * 60) + 30));
	std::vector<Tear*> liveTears{ t1, t2, t3, t4 };

	Spectre* sp1 = new Spectre(Real_Coords((20 * 60) + 30, (11 * 60) + 30), std::vector < Real_Coords > {
		Real_Coords(20 * 60, 7.3 * 60), Real_Coords(20 * 60, 19.3 * 60)
	}, gameOverFlag);

	Spectre* sp2 = new Spectre(Real_Coords((4 * 60) + 30, (4 * 60) + 30), std::vector < Real_Coords > {
		Real_Coords(7.2 * 60, 7.2 * 60), Real_Coords(1.2 * 60, 7.2 * 60), Real_Coords(1.2 * 60, 3.2 * 60), Real_Coords(7.2 * 60, 3.2 * 60)
	}, gameOverFlag);

	Spectre* sp3 = new Spectre(Real_Coords((35 * 60) + 30, (11 * 60) + 30), std::vector < Real_Coords > {
		Real_Coords(35.2 * 60, 11.2 * 60), Real_Coords(23.2 * 60, 19.2 * 60)
	}, gameOverFlag);

	Spectre* sp4 = new Spectre(Real_Coords((30 * 60) + 30, (5 * 60)), std::vector < Real_Coords > {
		Real_Coords((12 * 60.2) + 30, (3.2 * 60) + 30), Real_Coords((32 * 60) + 30, 67)
	}, gameOverFlag);

	Seeker* se1 = new Seeker(Real_Coords((38 * 60) + 30, (2 * 60) + 30), std::vector < Real_Coords > {
		Real_Coords(38.2 * 60, 2.2 * 60), Real_Coords(29.2 * 60, 14 * 60)
	}, gameOverFlag);
	Seeker* se2 = new Seeker(Real_Coords((12 * 60) + 30, (11 * 60) + 30), std::vector < Real_Coords > {
		Real_Coords(12.2 * 60, 11.2 * 60), Real_Coords(15.2 * 60, 11.2 * 60), Real_Coords(15.2 * 60, 14.2 * 60), Real_Coords(12.2 * 60, 14.2 * 60)
	}, gameOverFlag);
	Seeker* se3 = new Seeker(Real_Coords((4 * 60) + 30, (1 * 60) + 30), std::vector < Real_Coords > {
		Real_Coords(4.2 * 60, 9.2 * 60), Real_Coords(4.2 * 60, 1.2 * 60)
	}, gameOverFlag);
	Seeker* se4 = new Seeker(Real_Coords((14 * 60) + 30, (19 * 60) + 30), std::vector < Real_Coords > {
		Real_Coords(14.2 * 60, 19.2 * 60), Real_Coords(25.2 * 60, 19.2 * 60), Real_Coords(25.2 * 60, 16.2 * 60), Real_Coords(22.2 * 60, 16.2 * 60)
	}, gameOverFlag);

	std::vector<Guardian_Base*> guardians{ sp1, sp2, sp3, sp4, se1, se2, se3, se4 };

	Level_Map* level_map = new Level_Map(tileMap, guardians, liveTears, Real_Coords((20 * 60) + 30, (6 * 60) + 30), gameOverFlag, stairTilePosition, decorations);
	return level_map;
}


#endif