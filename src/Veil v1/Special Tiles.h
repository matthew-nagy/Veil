#ifndef SPECIAL_TILES_H
#define SPECIAL_TILES_H true
#include "Player class.h"
#include "Guardian base.h"


ps::Visual_Data stair_tile_visual(
	&guardianTexture,
	sf::IntRect(18,41,2,2),
	sf::Color::White,
	0,
	1);
ps::Mechanical_Data stair_tile_mech(
	1,
	0,
	-1,
	0.95,
	20);

class Stair_Tile:public Tile{
public:
	//This constructor auto sets up some aspects of tile, and also
	//sets up the new private variables such as tears
	Stair_Tile(Real_Coords position, sf::Texture& texture,suint numOfTears) :
		Tile(position, texture, closedRect, false, tt_stair),
		numOfTears(numOfTears)
	{
		setupTears(numOfTears,position);
		tileSprite.setTextureRect(closedRect);
	}

	//Also draws the tear representations
	void				draw(sf::RenderWindow& window)const override{
		Tile::draw(window);

		for (size_t i = 0; i < numOfTears; i++){
			window.draw(tearRepresentations[i]);
		}
	}

	//Rotates the representations around the centre and sometimes
	//shoots particles out
	void				moveTears(){
		particleCounter += 1;
		if (particleCounter == 6){
			particleCounter = 0;
		}
		for (size_t i = 0; i < numOfTears; i++){
			tearRepresentations[i].setPosition(math::rotateAroundPoint(tileSprite.getPosition()+sf::Vector2f(30,30), tearRepresentations[i].getPosition(), 3));
			tearRepresentations[i].rotate(3);

			if (particleCounter == 1){
				stair_tile_mech.angle = math::getAngle(tileSprite.getPosition() + sf::Vector2f(30, 30), tearRepresentations[i].getPosition()) - 60;
				new ps::Particle(tearRepresentations[i].getPosition(), stair_tile_visual, stair_tile_mech);
			}
		}
	}

	//Pops back one of the tear representations
	void				removeTear(){
		numOfTears -= 1;
		tearRepresentations.pop_back();

		if (numOfTears == 0){
			tileSprite.setTextureRect(openRect);
		}
	}
private:
	//Constant texture rects for how the stairs look
	const sf::IntRect	closedRect = sf::IntRect(240, 0, 60, 60);
	const sf::IntRect	openRect   = sf::IntRect(180, 0, 60, 60);

	suint				particleCounter = 0;	//The timer for creating new particles

	std::vector<sf::Sprite> tearRepresentations;//Stores all the rotating sprites, one for each live tear
	size_t				numOfTears;				//The number of tears in the list. Used to optimise loops.

	//Called in the constructor to setup the tears representations
	void				setupTears(suint& tearNumber,sf::Vector2f& position){
		for (size_t i = 0; i < tearNumber; i++){
			sf::Sprite sprite(guardianTexture);
			sprite.setTextureRect(sf::IntRect(23, 109, 5, 5));
			sprite.setOrigin(2.5, 2.5);
			sprite.setPosition(position + sf::Vector2f(30, 30));

			suint angle = 30 * i;
			sprite.rotate(angle);
			sprite.move(math::getMove(28, angle));

			tearRepresentations.push_back(sprite);
		}
	}

};


ps::Visual_Data breakable_tile_visual1(
	&guardianTexture,
	sf::IntRect(35,105,7,8),
	sf::Color::White,
	3,
	2
	);

ps::Visual_Data breakable_tile_visual2(
	&guardianTexture,
	sf::IntRect(32,115, 6,6),
	sf::Color::White,
	5,
	2
	);

ps::Mechanical_Data breakable_tile_mech(
	8,
	0,
	0,
	0.8,
	80
	);

class Breakable_Tile :public Tile{
public:
	void		shatter(){
		type = tt_normal;
		collidable = false;
		coverSprite.setTextureRect(brokenRect);

		for (suint i = 0; i < 20; i++){
			breakable_tile_mech.angle = rand() % 360;
			new ps::Particle(emminatePoint, breakable_tile_visual1, breakable_tile_mech);

			breakable_tile_mech.angle = rand() % 360;
			new ps::Particle(emminatePoint, breakable_tile_visual2, breakable_tile_mech);
		}
	}

	void		draw(sf::RenderWindow& window)const override{
		Tile::draw(window);
		window.draw(coverSprite);
	}

	Breakable_Tile(Real_Coords position, sf::Texture& texture) :
		Tile(position, texture, sf::IntRect(60, 0, 60, 60), true, tt_breakable),
		emminatePoint(position + sf::Vector2f(30, 30))
	{
		coverSprite.setTexture(texture);
		coverSprite.setTextureRect(sf::IntRect(300, 0, 60, 60));
		coverSprite.setPosition(position);
	}

private:
	sf::Sprite	coverSprite;
	sf::IntRect	brokenRect
		= sf::IntRect(360, 0, 60, 60);
	Real_Coords	emminatePoint;
};

inline void	Guardian_Base::breakNextTile(){
	reinterpret_cast<Breakable_Tile*>(currentPath[currentPathPointer])->shatter();
}
#endif