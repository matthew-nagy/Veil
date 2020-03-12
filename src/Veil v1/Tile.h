#ifndef TILE_H
#define TILE_H true

#ifndef SFML_GRAPHICS_HPP
#include "SFML\Graphics.hpp"
#endif

#include "Particles.h"

//Create new ways of typing sf::Vector2f, and sf::Vector2i to clear confusion on what their contents should be later
typedef sf::Vector2f Real_Coords;	//Actual window based coordinates
typedef sf::Vector2i Virtual_Coords;//Coordinates based on a tilemap
sf::Texture blank;					//For whenever a sprite needs to be clear

enum Tile_Type{
	tt_normal,
	tt_invunrable,
	tt_stair,
	tt_breakable
};

class Tile{
public:

	//Checks if the tile is to be collided with or not
	inline bool						isCollidable()const{
		return collidable;
	}

	//Returns the center and dimensions of the tile sprite
	inline const Real_Coords&		getCenter()const{
		return center;
	}
	inline const sf::Vector2f&		getSize()const{
		return dimensions;
	}

	//Used to interface with mapPosition
	inline void						setMapPosition(Virtual_Coords coordinates){
		mapPosition = coordinates;
	}
	inline const Virtual_Coords&	getMapPosition()const{
		return mapPosition;
	}

	Tile(Real_Coords position, sf::Texture& texture, sf::IntRect rect, bool collidable, Tile_Type type=tt_normal) :
		collidable(collidable),
		dimensions(rect.width,rect.height),
		tileSprite(texture),
		type(type)
	{

		tileSprite.setTextureRect(rect);
		tileSprite.setPosition(position);

		center = Real_Coords(position + sf::Vector2f(rect.width / 2, rect.height / 2));
	}

	//Draws the tile sprite to screen. Overloads may draw other things as well
	virtual void					draw(sf::RenderWindow& window)const{
		window.draw(tileSprite);
	}

	inline Tile_Type				getType()const{
		return type;
	}
protected:
	sf::Sprite						tileSprite;			//The main tile
	
	bool							collidable;			//Is the tile solid?

	Real_Coords						center;				//The window based center of the tile
	sf::Vector2f					dimensions;			//The dimensions of the tile
	Virtual_Coords					mapPosition;		//The map based coordinates in the Tile_Map container
	Tile_Type						type;				//Anything special about the tile
};


//These tiles are expanded on later in Special Tiles.h
class Stair_Tile;
class Breakable_Tile;

#endif