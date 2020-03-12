#ifndef TIMER_VINE_H
#define TIMER_VINE_H

#include "SFML\Graphics.hpp"
#include "Time based particles.h"
sf::Texture timerVineTexture;
bool __vine_texture_loaded__ = false;

class Timer_Vine{
public:

	void move(sf::Vector2f velocity){
		for (size_t i = 0; i < numOfSegments; i++){
			vineSegments[i].move(velocity);
			vineFruits[i].move(velocity);
		}
		flowerHead.move(velocity);
		roots.move(velocity);
	}

	void fillInNextFruit(){
		vineFruits[nextFruitToTurn].setTextureRect(fullFruint);
		createParticleBurst();
		nextFruitToTurn -= 1;
	}

	void draw(sf::RenderWindow& window){
		for (size_t i = 0; i < numOfSegments; i++){
			window.draw(vineSegments[i]);
			window.draw(vineFruits[i]);
		}
		window.draw(flowerHead);
		window.draw(roots);
	}

	Timer_Vine(suint numOfTears, sf::Vector2f startPosition):
		numOfSegments(numOfTears),
		nextFruitToTurn(numOfTears-1),
		flowerHead(timerVineTexture, sf::IntRect(30, 0, 13, 11)),
		roots(timerVineTexture, sf::IntRect(29, 12, 14, 9))
	{

		if (!__vine_texture_loaded__){
			__vine_texture_loaded__ = true;
			timerVineTexture.loadFromFile("Art/Timer vine.png");
		}

		createVineSegments(numOfTears, startPosition);
	}

private:
	std::vector<sf::Sprite> vineSegments;
	std::vector<sf::Sprite> vineFruits;

	sf::Sprite flowerHead;
	sf::Sprite roots;

	suint nextFruitToTurn;
	size_t numOfSegments;

	const sf::IntRect segmentRect = sf::IntRect(8, 0, 20, 21);
	const sf::IntRect stoneFruit = sf::IntRect(0, 0, 8, 9);
	const sf::IntRect fullFruint = sf::IntRect(0, 10, 8, 9);

	void createParticleBurst(){
		for (size_t i = 0; i < 6; i++){
			//no point them going to the left of the screen
			tear_collect_mech.angle = rand() % 180;
			new ps::Particle(vineFruits[nextFruitToTurn].getPosition(), tear_collect_vis, tear_collect_mech);
		}
	}

	void createVineSegments(suint& numOfTears,sf::Vector2f& startPosition){
		for (size_t i = 0; i < numOfTears; i++){
			sf::Sprite segment(timerVineTexture, segmentRect);
			segment.setOrigin(10, 10.5);
			segment.setPosition(startPosition - sf::Vector2f(40, 0) + sf::Vector2f(0, 21 * i));
			
			sf::Sprite fruit(timerVineTexture, stoneFruit);
			fruit.setOrigin(4, 0);
			fruit.setPosition(startPosition - sf::Vector2f(49, 0.5) + sf::Vector2f(0, 21 * i));

			if (i % 2 != 0){
				segment.setScale(-1, 1);
				segment.move(15, 0);
				fruit.move(31, 0);
			}


			vineSegments.push_back(segment);
			vineFruits.push_back(fruit);
		}

		flowerHead.setPosition(startPosition - sf::Vector2f(37, 21));
		roots.setPosition(startPosition - sf::Vector2f(39,11) + sf::Vector2f(0, 21 * numOfTears));
	}
};

#endif