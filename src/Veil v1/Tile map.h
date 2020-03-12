#ifndef TILE_MAP_H
#define TILE_MAP_H true

//Inclusion safety is handled in file
#include "Tile.h"
#include "Binary list.h"
#include "Math.h"

//A way of using Tiles in the Binary Trees for A*. Defined later.
class Tile_Node;
class Tile_Node_Wrapper;


/*Given that this map will be needed as arguments for most future subroutines, it needs to
  be decared now. However, as these future subroutines are used to give it most of its 
  functionality like line of sight and pathfinding, this class has quite a few declarations
  which will be defined properly later in different files.*/


class Tile_Map{
public:
	//Ways of interacting with the stored tiles
	inline Tile*			getTile(Real_Coords position)const{
		return map[position.y / tileSize.y][position.x / tileSize.x];
	}
	inline Virtual_Coords	convertToMapCoords(Real_Coords position){
		return Virtual_Coords(position.x / tileSize.x, position.y / tileSize.y);
	}
	inline Real_Coords		convertToRealCoords(Virtual_Coords position){
		return Real_Coords(position.x*tileSize.x, position.y*tileSize.y);
	}
	inline bool				collides(Real_Coords position)const{
		return getTile(position)->isCollidable();
	}
	//Will get a path using A*. Defined later.
	std::vector<Tile*>		getPath(Real_Coords start, Real_Coords target, bool canBreakBarriers = false);


	Tile_Map(std::vector<std::vector<Tile*>> map) :
		map(map),
		mapSize(map[0].size(), map.size()),
		tileSize(map[0][0]->getSize())
	{

		for (size_t y = 0; y < mapSize.y; y++){
			for (size_t x = 0; x < mapSize.x; x++){
				map[y][x]->setMapPosition(Virtual_Coords(x, y));
			}
		}

	}


	//Draws all the tiles to the window
	void					draw(sf::RenderWindow& window)const{
		for (size_t y = 0; y < mapSize.y; y++){
			for (size_t x = 0; x < mapSize.x; x++){
				map[y][x]->draw(window);
			}
		}
	}
	void __debugDraw(Real_Coords playerPosition, sf::RenderWindow& window, Real_Coords viewTopLeft, Real_Coords viewBottomRight);

	void __debugGetPath(Real_Coords start, Real_Coords target, sf::RenderWindow& window);

private:

	std::vector <
		std::vector < Tile* >
	>						map;		//The tile map itself
	const sf::Vector2f		tileSize;	//Dimensions of the tile
	const sf::Vector2f		mapSize;	//Size of the map
	
	//Used in the pathfinding later to add nodes to the closed list. Defined later.
	void					push_node(Tile_Node* toPush,
		Binary_List<Tile_Node_Wrapper>& openList, Binary_List<Tile_Node_Wrapper>& closedList, std::vector<Tile_Node*>& allTileNodes,
		Tile_Node*& endNode, Tile* targetTile, sf::Vector2f targetPosition,
		std::vector<std::vector<bool>>& addTable, bool canBreakBarriers);

	//Traces back the final node to generate a path. Defined later.
	std::vector<Tile*>		tracebackNode(Tile_Node* endNode);
};


//Link all the other files that are to do with Tile map functionallity
//Given the macros, there is no risk of including a file twice
#include "A star functions.h"

#endif