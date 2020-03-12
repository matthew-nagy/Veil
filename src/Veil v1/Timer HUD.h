#ifndef TIMER_HUD_H
#define TIMER_HUD_H true

#include "Timer Vine.h"
typedef short unsigned suint;
#include <iostream>

//used for function pointers
extern class Level_Map;

//The texture for the HUD icons
sf::Texture HUD_texture;
bool HUD_loaded = false;


class TimerHUD{
public:

	//Scale determines the size of the sprites, startPosition is the topLeft
	TimerHUD(float scale, sf::Vector2f startPosition, Level_Map* currentLevelMap, suint numOfTears, void(Level_Map::*timeoutTrigger)(), void(Level_Map::*resetTrigger)()) :
		background(HUD_texture),
		dropCover(HUD_texture),
		timeoutTrigger(timeoutTrigger),
		resetTrigger(resetTrigger),
		currentLevelMap(currentLevelMap),
		vine(numOfTears, startPosition)
	{
		if (!HUD_loaded) HUD_texture.loadFromFile("Art/Timer.png");


		background.setTextureRect(sf::IntRect(0,0,133,189));
		dropCover.setTextureRect(sf::IntRect(135,0,126,178));

		background.setScale(scale, scale);
		dropCover.setScale(scale, scale);

		background.setPosition(startPosition);
		dropCover.setPosition(startPosition + sf::Vector2f(4 * scale, 6.5*scale));

		//For some reason you have to do it in two goes like this
		speed = 178;
		speed /= maxTick;
	}

	//Decreases the timer, moves and resizes the sprites
	void		update(sf::Vector2f move){
		background.move(move);
		dropCover.move(move);

		vine.move(move);

		if (tick == 0) return;
		if (tick > currentCountRate){
			tick -= currentCountRate;

			sf::IntRect r = dropCover.getTextureRect();
			rectHeight += speed*currentCountRate;
			r.height = rectHeight;
			dropCover.setTextureRect(r);
		}
		else{
			dropCover.setTextureRect(sf::IntRect(135, 0, 126, 178));
			triggerHostile();
			tick = 0;
		}
	}

	//Draws all of the sprites onto the screen
	void		draw(sf::RenderWindow& window){
		window.draw(background);
		window.draw(dropCover);
		vine.draw(window);
	}

	//Changes the speed of the timer
	inline void setSpeed(suint newSpeed){
		currentCountRate = newSpeed;
	}

	//Resets the timer and sprites, as a tear has been collected
	void		reset(){
		currentCountRate = 1;
		tick = maxTick;

		rectHeight = 0;
		dropCover.setTextureRect(sf::IntRect(135, 0, 126, 0));

		vine.fillInNextFruit();
	}

	inline void triggerSafeZone(){
		(currentLevelMap->*resetTrigger)();
	}

	inline void triggerHostile(){
		(currentLevelMap->*timeoutTrigger)();
		dangerParticles();
	}

	suint getCount(){
		return tick;
	}
private:
	sf::Sprite	background;		//The background sprite
	sf::Sprite	dropCover;		//A cover to show time running out
	double		speed;			//The speed the rectHeight increases
	float		rectHeight = 0;	//The height of the dropCover

	suint		tick = 0;			//Current timer tick
	suint		currentCountRate = 1;	//The speed the timer decreases at
	const suint maxTick = 600;	//The original large timer value

	Level_Map*	currentLevelMap;

	Timer_Vine vine;

	void dangerParticles(){
		for (size_t i = 0; i < 50; i++){
			time_runout_mech.angle = rand() % 360;
			sf::Vector2f start = background.getPosition() + sf::Vector2f(30, 50);
			start.x += (rand() % 41) - 20;
			start.y += (rand() % 41) - 20;

			new ps::Particle(start, time_runout_vis, time_runout_mech);
		}
	}

	void		(Level_Map::*timeoutTrigger)();	//A pointer to a void function to be
	//called when the timer runs out
	void		(Level_Map::*resetTrigger)();		//A pointer to be called when the player
	//touches a safe zone. Tear collection is
	//dealt with by the level object
};

#endif