#ifndef ASTAR_FUNCTIONS_H
#define ASTAR_FUNCTIONS_H true

#include "A star classes.h"

void	Tile_Map::push_node(Tile_Node* toPush,
	Binary_List<Tile_Node_Wrapper>& openList, Binary_List<Tile_Node_Wrapper>& closedList, std::vector<Tile_Node*>& allTileNodes,
	Tile_Node*& endNode, Tile* targetTile, sf::Vector2f targetPosition,
	std::vector<std::vector<bool>>& addTable,bool canBreakBarriers){

	toPush->inOpen = false;
	closedList.push_back(toPush);
	sf::Vector2f currentOrds = sf::Vector2f(toPush->x, toPush->y);
	Tile* toAddNext[4]{
		map[currentOrds.y - 1][currentOrds.x],
			map[currentOrds.y + 1][currentOrds.x],
			map[currentOrds.y][currentOrds.x - 1],
			map[currentOrds.y][currentOrds.x + 1]
	};

	//Generate a new node for each tile
	for (suint i = 0; i < 4; i++){

		bool breakable = toAddNext[i]->getType() == tt_breakable;
		
		if (breakable && !canBreakBarriers) continue;
	
		//At this point they can break walls, so if it can't be traversed but it is breakable, add it anyway
		if ((toAddNext[i]->isCollidable() || toAddNext[i]->getType() == Tile_Type::tt_invunrable) && !breakable){
			continue;
		}

		Tile_Node* node = new Tile_Node(toAddNext[i], toPush, math::pythagerous(toAddNext[i]->getCenter(), targetPosition), toPush->distanceTravelled + breakable ? toPush->innerTile->getSize().x * 4 : toPush->innerTile->getSize().x);
		allTileNodes.push_back(node);
		const Virtual_Coords mapPos = node->innerTile->getMapPosition();

		if (toAddNext[i] == targetTile){
			endNode = node;
			return;
		}

		if (addTable[mapPos.y][mapPos.x] == true){
			if (closedList.contains(node)){
				size_t index = closedList.getIndex(node);
				if (closedList[index].target->distanceTravelled > node->distanceTravelled){
					closedList[index].target->previousNode = toPush;//We don't want to mess up the traceback
				}
			}
		}
		else{
			node->inOpen = true;
			openList.push_back(node);
			addTable[mapPos.y][mapPos.x] = true;
		}
	}

}


std::vector<Tile*>		Tile_Map::tracebackNode(Tile_Node* endNode){

	std::vector<Tile*> trace;
	Tile_Node* tracer = endNode;

	while (tracer->previousNode != nullptr){
		trace.push_back(tracer->previousNode->innerTile);
		tracer = tracer->previousNode;

	}

	//remove the node the entity started on, no need to retrace steps
	trace.pop_back();
	std::reverse(trace.begin(), trace.end());
	//make sure it makes it to the target
	trace.push_back(endNode->innerTile);
	return trace;
}

std::vector<Tile*>		Tile_Map::getPath(Real_Coords start, Real_Coords target, bool canBreakBarriers){
	std::vector<Tile_Node*> allNodesUsed;
	Binary_List<Tile_Node_Wrapper> openList;
	Binary_List<Tile_Node_Wrapper> closedList;

	std::vector<std::vector<bool>> addTable;
	for (size_t y = 0; y < map.size(); y++){
		std::vector<bool> row;
		row.reserve(map[y].size());
		for (size_t x = 0; x < map[y].size(); x++){
			row.push_back(false);
		}
		addTable.push_back(row);
	}


	Tile* endTile = getTile(target);
	Tile_Node* endNode = nullptr;
	if (endTile->isCollidable() || endTile->getType() == Tile_Type::tt_invunrable){
		if (!(canBreakBarriers && endTile->getType() == tt_breakable)){
			std::cout << "End tile is collision tile\n";
			return std::vector < Tile* > {};
		}
	}


	Tile_Node* firstNode = new Tile_Node(getTile(start), nullptr, math::pythagerous(start, target), 0);
	const Virtual_Coords mapPos = firstNode->innerTile->getMapPosition();
	addTable[mapPos.y][mapPos.x] = true;
	allNodesUsed.push_back(firstNode);
	openList.push_back(Tile_Node_Wrapper(firstNode));


	while (endNode == nullptr && openList.size() > 0){

		Tile_Node* toPush = openList.pop_front().target;
		push_node(toPush,
			openList, closedList, allNodesUsed,
			endNode, endTile, target,
			addTable, canBreakBarriers);

	}

	if (endNode == nullptr){
		for (size_t i = 0; i < allNodesUsed.size(); i++){
			delete allNodesUsed[i];
		}
		return std::vector < Tile* > {};
	}


	std::vector<Tile*> toret = tracebackNode(endNode);
	for (size_t i = 0; i < allNodesUsed.size(); i++){
		delete allNodesUsed[i];
	}
	allNodesUsed.clear();

	return toret;
}

void Tile_Map::__debugGetPath(Real_Coords start, Real_Coords target, sf::RenderWindow& window){

	std::vector<Tile_Node*> allNodesUsed;
	Binary_List<Tile_Node_Wrapper> openList;
	Binary_List<Tile_Node_Wrapper> closedList;

	std::vector<std::vector<bool>> addTable;
	for (size_t y = 0; y < map.size(); y++){
		std::vector<bool> row;
		row.reserve(map[y].size());
		for (size_t x = 0; x < map[y].size(); x++){
			row.push_back(false);
		}
		addTable.push_back(row);
	}

	Tile* endTile = getTile(target);
	Tile_Node* endNode = nullptr;
	if (endTile->isCollidable()){
		return;
	}


	Tile_Node* firstNode = new Tile_Node(getTile(start), nullptr, math::pythagerous(start, target), 0);
	const Virtual_Coords mapPos = firstNode->innerTile->getMapPosition();
	addTable[mapPos.y][mapPos.x] = true;
	allNodesUsed.push_back(firstNode);
	openList.push_back(Tile_Node_Wrapper(firstNode));

	sf::Texture blank;

	while (endNode == nullptr && openList.size() > 0 && window.isOpen()){

		Tile_Node* toPush = openList.pop_front().target;
		push_node(toPush,
			openList, closedList, allNodesUsed,
			endNode, endTile, target,
			addTable,false);

		window.clear();

		draw(window);

		for (size_t i = 0; i < closedList.size(); i++){
			sf::Sprite sprite(blank);
			sprite.setTextureRect(sf::IntRect(0, 0, 7, 7));
			sprite.setColor(sf::Color(255,0,0,150));
			sprite.setPosition(closedList[i].target->innerTile->getCenter() - sf::Vector2f(3.5, 3.5));
			window.draw(sprite);
		}

		for (size_t i = 0; i < openList.size(); i++){
			sf::Sprite sprite(blank);
			sprite.setTextureRect(sf::IntRect(0, 0, 7, 7));
			sprite.setColor(sf::Color(0,255,0,150));
			sprite.setPosition(openList[i].target->innerTile->getCenter() - sf::Vector2f(3.5, 3.5));
			window.draw(sprite);
		}

		sf::CircleShape startShape(3);
		startShape.setPosition(start - sf::Vector2f(3, 3));
		startShape.setFillColor(sf::Color::Red);
		window.draw(startShape);

		sf::CircleShape targetShape(3);
		targetShape.setPosition(target - sf::Vector2f(3, 3));
		targetShape.setFillColor(sf::Color::Blue);
		window.draw(targetShape);

		window.display();

		while (!sf::Keyboard::isKeyPressed(sf::Keyboard::K)){
			sf::Event event;
			while (window.pollEvent(event)){
				switch (event.type){
				case sf::Event::Closed:
					window.close();
					break;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)){
				for (size_t i = 0; i < openList.size(); i++){
					std::cout << openList[i].target->overallDistanceValue << "\n";
				}
				std::cout << "\n\n~~~~~~~\n\n";
				while (sf::Keyboard::isKeyPressed(sf::Keyboard::P)){}
			}
		}
		while (sf::Keyboard::isKeyPressed(sf::Keyboard::K)){}
	}

	if (endNode == nullptr){
		std::cout << "Ran out of open lst\n";
		for (size_t i = 0; i < allNodesUsed.size(); i++){
			delete allNodesUsed[i];
		}
	}

	std::vector<Tile*> toret = tracebackNode(endNode);

	for (size_t i = 0; i < allNodesUsed.size(); i++){
		delete allNodesUsed[i];
	}
	allNodesUsed.clear();

}

#endif