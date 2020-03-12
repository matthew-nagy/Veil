#ifndef ASTAR_CLASSES_H
#define ASTAR_CLASSES_H true

#include "Tile map.h"

//A holder for tiles to deal with distance and tracing back the best path
class Tile_Node{
public:
	Tile* innerTile;//The tile this node is containing
	Tile_Node* previousNode;//The tileNode this came from
	suint x, y;//The in-map coordinates of the inner tile

	bool inOpen = false;

	double heuristic;//Predicted distance from the target
	double distanceTravelled;//How far has this nodetravelled from the start
	double overallDistanceValue;//The value used in the open list

	//This is used to get around the fact that binary trees return a const reference, so that the previous tile can be altered
	inline void setPreviousTile(Tile_Node* newNode){
		previousNode = newNode;
	}

	Tile_Node(Tile* innerTile, Tile_Node* previousNode, double heuristic, double distanceTravelled) :
		innerTile(innerTile),
		previousNode(previousNode),
		x(innerTile->getMapPosition().x),
		y(innerTile->getMapPosition().y),
		heuristic(heuristic),
		distanceTravelled(distanceTravelled),
		overallDistanceValue(heuristic + distanceTravelled)
	{
	}

	bool operator==(Tile_Node& other){
		return inOpen ?
			overallDistanceValue == other.overallDistanceValue :
			innerTile == other.innerTile;
	}
	bool operator>=(Tile_Node& other){
		return inOpen ?
			overallDistanceValue >= other.overallDistanceValue :
			innerTile >= other.innerTile;
	}
	bool operator>(Tile_Node& other){
		return inOpen ?
			overallDistanceValue > other.overallDistanceValue :
		innerTile > other.innerTile;
	}
	bool operator<(Tile_Node& other){
		return inOpen ?
			overallDistanceValue < other.overallDistanceValue :
			innerTile < other.innerTile;
	}

	bool operator==(Tile_Node* other){
		return inOpen ?
			overallDistanceValue == other->overallDistanceValue :
			innerTile == other->innerTile;
	}
	bool operator>=(Tile_Node* other){
		return inOpen ?
			overallDistanceValue >= other->overallDistanceValue :
			innerTile >= other->innerTile;
	}
	bool operator>(Tile_Node* other){
		return inOpen ?
			overallDistanceValue > other->overallDistanceValue :
		innerTile > other->innerTile;
	}
	bool operator<(Tile_Node* other){
		return inOpen ?
			overallDistanceValue < other->overallDistanceValue :
			innerTile < other->innerTile;
	}
};

//All Tile_Nodes will be pointers, so for the binary tree to work properly you need another wrapper
class Tile_Node_Wrapper{
public:
	Tile_Node* target;

	Tile_Node_Wrapper(Tile_Node* target) :
		target(target)
	{

	}

	bool operator==(Tile_Node_Wrapper& other){
		return target->inOpen ?
			target->overallDistanceValue == other.target->overallDistanceValue :
			target->innerTile == other.target->innerTile;
	}
	bool operator>=(Tile_Node_Wrapper& other){
		return target->inOpen ?
			target->overallDistanceValue >= other.target->overallDistanceValue :
			target->innerTile >= other.target->innerTile;
	}
	bool operator>(Tile_Node_Wrapper& other){
		return target->inOpen ?
			target->overallDistanceValue > other.target->overallDistanceValue :
		target->innerTile > other.target->innerTile;
	}
	bool operator<(Tile_Node_Wrapper& other){
		return target->inOpen ?
			target->overallDistanceValue < other.target->overallDistanceValue :
			target->innerTile < other.target->innerTile;
	}
};

#endif