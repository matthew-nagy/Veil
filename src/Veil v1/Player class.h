#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H true
#include "Animation class.h"
#include "Timer HUD.h"
#include "Tears.h"

enum Direction{
	d_up,
	d_down,
	d_left,
	d_right
};

class Seeker;
extern Seeker* seekerAroundPlayer;
sf::Texture playerTexture;

class Player{
public:

	void speedUpCountDown(){
		timer.setSpeed(2);
	}

	inline Real_Coords getPosition()const{
		return sprite.getPosition();
	}

	inline const sf::FloatRect& getCollision()const{
		return sprite.getGlobalBounds();
	}

	inline void resetTimer(){
		timer.reset();
	}

	void update(Tile_Map& map){
		sf::Vector2f velocity(0,0);

		moved = false;
		if (!frozen){
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
				velocity.y -= speed;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
				velocity.y += speed;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
				velocity.x -= speed;
				xDirection = d_left;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
				velocity.x += speed;
				xDirection = d_right;
			}
			sprite.move(velocity);
			if (velocity.x != 0 || velocity.y != 0){
				moved = true;
			}
		}
		
		if (collides(map)){
			dealWithCollision(map, velocity);
		}
		screenShadow.move(velocity);

		runAnimation();

		//If it is a special invunrable tile
		bool inInvunrableTile = map.getTile(sprite.getPosition())->getType() == tt_invunrable;

		if (inSafeZone){
			if (!inInvunrableTile){
				timer.setSpeed(1);
				if (timer.getCount() == 0){
					timer.triggerHostile();
				}
				inSafeZone = false;
			}
		}
		else{
			if (inInvunrableTile){
				inSafeZone = true;
				timer.triggerSafeZone();
				timer.setSpeed(0);
				seekerAroundPlayer = nullptr;
			}
		}
		timer.update(velocity);

	}

	inline void draw(sf::RenderWindow& window){
		window.draw(sprite);
	}

	inline void drawHUD(sf::RenderWindow& window){
		window.draw(screenShadow);
		timer.draw(window);
	}

	sf::Sprite getCorpse(){
		//call the copy constructor
		sf::Sprite newCorpse = sprite;

		newCorpse.setTextureRect(sf::IntRect(14,35, 30, 14));

		return newCorpse;
	}

	//Makes the player stop moving when attacked
	inline void freeze(){
		frozen = true;
		//sets it to the first of the death animation
		sprite.setTextureRect(deathAnimation.getCurrentFrame());
	}

	//runs a game over animation
	void runGameOver(){
		if (deathAnimation.isFrameChange()){
			sprite.setTextureRect(deathAnimation.getCurrentFrame());
		}
	}

	Player(Real_Coords position, Level_Map* levelMap, suint numOfTears,void(Level_Map::*timeoutTrigger)(), void(Level_Map::*resetTrigger)()) :
		timer(0.5, position - sf::Vector2f(170, 170), levelMap,numOfTears,timeoutTrigger, resetTrigger)
	{
		setupAnimation();

		sprite.setTexture(playerTexture);
		sprite.setTextureRect(walkAnimation.getCurrentFrame());
		sprite.setOrigin(8,9);
		sprite.setScale(1.4, 1.4);
		sprite.setPosition(position);

		shadowTexture.loadFromFile("Art/Shadow 2.png");
		screenShadow.setTexture(shadowTexture);
		screenShadow.setPosition(position - sf::Vector2f(300,300));
		screenShadow.setScale(6,6);
		screenShadow.setTextureRect(sf::IntRect(0, 0, 100, 100));
	}
private:
	sf::Sprite sprite;
	float halfWidth = 12;
	float halfHeight = 10;

	sf::Sprite screenShadow;
	sf::Texture shadowTexture;

	TimerHUD timer;
	bool inSafeZone = false;
	bool frozen = false;

	const suint speed = 5;

	Animation walkAnimation;
	Animation deathAnimation;

	Direction xDirection = d_right;
	Direction lastDirection = d_right;
	bool moved = false;

	bool collides(Tile_Map& map){
		Real_Coords position = getPosition();
		if (map.getTile(position - sf::Vector2f(halfHeight, halfWidth))->isCollidable()) return true;
		if (map.getTile(position + sf::Vector2f(halfHeight, halfWidth))->isCollidable()) return true;
		if (map.getTile(position - sf::Vector2f(halfHeight, -halfWidth))->isCollidable()) return true;
		if (map.getTile(position + sf::Vector2f(halfHeight, -halfWidth))->isCollidable()) return true;
		return false;
	}

	void dealWithCollision(Tile_Map& map, sf::Vector2f& velocity){
		sprite.move(-velocity.x, 0);
		bool xCollide = !collides(map);
		sprite.move(velocity.x, -velocity.y);
		bool yCollide = !collides(map);
		sprite.move(0, velocity.y);//move the sprite back to where it was

		//If they were each responisble for eachothers collision, they are still true
		if (!(xCollide || yCollide)){
			xCollide = true;
			yCollide = true;
		}

		float x = 0;
		float y = 0;

		if (xCollide){
			x = -velocity.x / speed;
		}
		//COuld collide both ways
		if (yCollide){
			y = -velocity.y / speed;
		}

		do{
			sprite.move(x, y);
			//Alter velocity as well to make sure it stays in sync with the player
			velocity.x += x;
			velocity.y += y;
		} while (collides(map));

	}

	void runAnimation(){
		if (!moved) return;

		if (walkAnimation.isFrameChange()){
			sprite.setTextureRect(walkAnimation.getCurrentFrame());
		}

		if (lastDirection != xDirection){
			sprite.setScale(xDirection == d_right ? 1.4 : -1.4, 1.4);
		}

		lastDirection = xDirection;
	}

	void setupAnimation(){
		std::vector<sf::IntRect> walkFrames = { sf::IntRect(0, 0, 16, 18), sf::IntRect(17, 0, 16, 18) };
		std::vector<sf::IntRect> deathFrames = { sf::IntRect(0, 0, 16, 18), sf::IntRect(17, 0, 16, 18), sf::IntRect(34, 0, 16, 18),
													sf::IntRect(0,19,20,16),sf::IntRect(21,19,30,14)};

		walkAnimation = Animation(walkFrames, 9);
		deathAnimation = Animation(deathFrames, 6);
	}
};


#endif