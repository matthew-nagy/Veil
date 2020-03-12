#include <thread>
#include "Level class.h"

sf::Texture particleTexture;
namespace pt{
	using namespace ps;
	Visual_Data vis_typeA(
		&particleTexture,
		sf::IntRect(0, 0, 6, 6),
		sf::Color(90, 90, 90, 230),
		3,
		5.0
		);

	Mechanical_Data mech_typeA(
		7,
		0,
		0,
		0.9,
		40
		);

	Visual_Data vis_typeB(
		&particleTexture,
		sf::IntRect(0, 0, 6, 6),
		sf::Color(255, 0, 0, 200),
		3,
		20.0
		);
	SP_Visual_Data typeB_SP(
		5,
		0.95,
		true,
		sf::Color(0, 0, 0, 200),
		15,
		5
		);
	Mechanical_Data mech_typeB(
		7,
		0,
		0,
		0.9,
		40
		);
	/*
	short unsigned	fadeoutDelay;	//The time before it starts to fade
	float			spinFade;		//How quickly the rotation stops

	bool			colourAlter;	//If this is set to false, the next 2 are ignored
	sf::Color		transitionColour;//The colour it will transition to
	short unsigned	transitionTime;	//How many frames for the transition to be complete*/

	void makeParticleA(sf::View view){
		sf::Vector2f viewSize = view.getSize();
		sf::Vector2f viewTopLeft = view.getCenter() - sf::Vector2f(viewSize.x / 2, viewSize.y / 2);

		sf::Vector2f startPos = viewTopLeft + sf::Vector2f(rand() % int(viewSize.x), rand() % int(viewSize.y));
		new Particle(startPos, vis_typeA, mech_typeA);
	}

	void makeParticleB(sf::View view){
		sf::Vector2f viewSize = view.getSize();
		sf::Vector2f viewTopLeft = view.getCenter() - sf::Vector2f(viewSize.x / 2, viewSize.y / 2);

		sf::Vector2f startPos = viewTopLeft + sf::Vector2f(rand() % int(viewSize.x), rand() % int(viewSize.y));
		new Particle(startPos, vis_typeB, typeB_SP, mech_typeB);
	}
}

#include "Get levels.h"

void playLevel(Level_Map* (*getLevel)(std::string, sf::Texture&, std::vector<sf::Sprite>&), std::string fileName,sf::RenderWindow& window){
	sf::Texture tex;
	tex.loadFromFile("Art/bigger tex.png");
	tex.setRepeated(true);

	sf::View view;
	view.setSize(500, 500);
	//view.setSize(1400, 1400);
	view.setCenter(125, 125);
	std::vector<sf::Sprite> playerCorpses;
	Level_Map* level_map = (*getLevel)(fileName,tex, playerCorpses);

	bool _continue = true;
	while (window.isOpen() && _continue){
		sf::Event event;
		while (window.pollEvent(event)){
			switch (event.type){
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		//called in a thread to save processing time
		std::thread particleThread(ps::updateParticles);

		level_map->update();

		view.setCenter(level_map->getPlayerPosition());
		window.setView(view);

		particleThread.join();

		pt::makeParticleA(view);
		pt::makeParticleA(view);
		ps::addNewParticles();

		window.clear();

		level_map->draw(window);

		ps::drawParticles(window);

		window.display();
		if (level_map->isGameOver()){
			if (!level_map->hasPlayerWon()){
				delete level_map;
				ps::deleteAllParticles();

				if (global::returnToMenu){
					return;
				}

				level_map = level_map = (*getLevel)(fileName,tex, playerCorpses);
				seekerAroundPlayer = nullptr;
			}
			else{
				_continue = false;
				delete level_map;
				ps::deleteAllParticles();
			}
		}
	}
}