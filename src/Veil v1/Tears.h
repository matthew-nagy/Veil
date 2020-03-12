#ifndef TEARS_H
#define TEARS_H true

//Used to get the guardian_texture later
extern sf::Texture guardianTexture;
//Will be ignored, but for clearer reading
#include "Tile map.h"

class Tear{
public:

	bool collides(const sf::FloatRect& collision){
		return sprite.getGlobalBounds().intersects(collision);
	}

	void draw(sf::RenderWindow& window){
		runFloat();
		window.draw(dropShadow);
		window.draw(sprite);
	}

	void flip(){
		if (hostile){
			sprite.setTextureRect(calmRect);
		}
		else{
			sprite.setTextureRect(hostileRect);
		}
		hostile = !hostile;
	}

	Tear(Real_Coords position) :
		sprite(guardianTexture),
		dropShadow(4),
		basePosition(position)
	{
		sprite.setTextureRect(calmRect);
		//make sure that its floats over that point
		sprite.setOrigin(9.5, 13);
		sprite.setPosition(position);

		dropShadow.setTexture(&blank);
		dropShadow.setFillColor(sf::Color(0, 0, 0, 150));
		dropShadow.setPosition(position + sf::Vector2f(-3.5,7.5));

	}
private:
	sf::Sprite	sprite;				//The sprite
	sf::CircleShape		dropShadow;	//A shadow to give it some weight
	Real_Coords	basePosition;		//The position the sprite starts at

	bool		hostile = false;	//Keeps track of the current game mode

	//Texture rects for the sprite depending on the game mode
	const sf::IntRect	calmRect = sf::IntRect(13,26,19,26);
	const sf::IntRect	hostileRect = sf::IntRect(13,65,19,26);


	const double radianConverter = PI / 180;
	suint angle = 0;

	//Floats the tear up and down on a wave
	void		runFloat(){
		angle += 15;
		if (angle == 360) angle = 0;

		sprite.setPosition(basePosition.x, basePosition.y + sin(angle*radianConverter));
	}
};

#endif