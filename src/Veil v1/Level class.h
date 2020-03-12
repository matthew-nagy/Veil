#ifndef LEVEL_CLASS_H
#define LEVEL_CLASS_H true
#include "Special Tiles.h"
#include "Options menu.h"
#include "Globals.h"

//Texturres for game over, game won, and all text respectivly
sf::Texture gameOverTexture;
sf::Texture gameWonTexture;
sf::Texture whiteTextTexture;

class Level_Map{
public:

	//Runs all guardians, the player and timer systems
	void						update(){
		player.update(*map);

		for (size_t i = 0; i < size_guardian; i++){
			guardians[i]->update(*map, player);
		}


		checkAlertSystem();

		checkTearCollision();

		stairs->moveTears();
	}

	//Map, tears, player and guardians
	void						draw(sf::RenderWindow& window){

		map->draw(window);

		for (size_t i = 0; i < decor_size; i++){
			window.draw(decorations[i]);
		}

		for (size_t i = 0; i < size_tear; i++){
			tears[i]->draw(window);
		}

		player.draw(window);

		for (size_t i = 0; i < size_guardian; i++){
			guardians[i]->draw(window);
		}


		player.drawHUD(window);

		if (*gameOverFlag){
			runGameOver(window);
			decorations.push_back(player.getCorpse());
		}
		else if (playerHasWon){
			runGameWon(window);
		}
	}
	
	//Returns the player's global position
	inline Real_Coords& const	getPlayerPosition()const{
		return player.getPosition();
	}

	//Returns true if the game is over
	inline bool					isGameOver()const{
		return *gameOverFlag;
	}

	//Returns true if the player has won the game
	inline bool					hasPlayerWon()const{
		return playerHasWon;
	}

	Level_Map(Tile_Map* tileMap,std::vector<Guardian_Base*> guardians,std::vector<Tear*> tears, Real_Coords playerPosition,bool* gameOverFlag,Real_Coords stairPosition,std::vector<sf::Sprite>& decorations):
		map(tileMap),
		player(playerPosition,this,tears.size(),&Level_Map::flipHostile,&Level_Map::flipSafe),
		guardians(guardians),
		tears(tears),
		size_guardian(guardians.size()),
		size_tear(tears.size()),
		gameOverFlag(gameOverFlag),
		decorations(decorations),
		decor_size(decorations.size())
		{
			stairs = reinterpret_cast<Stair_Tile*>(tileMap->getTile(stairPosition));
		}

	~Level_Map(){
		for (size_t i = 0; i < size_tear; i++){
			delete tears[i];
		}
		for (size_t i = 0; i < size_guardian; i++){
			delete guardians[i];
		}
		delete map;
		delete gameOverFlag;
	}
private:

	Tile_Map*					map;			//Tile map of this level
	Stair_Tile*					stairs;			//Pointer to the stair tile
	Player						player;			//The player

	std::vector<Guardian_Base*> guardians;		//All guardians in this level
	size_t						size_guardian;	//The size used in for loops
	std::vector<Tear*>			tears;			//All tears in this level
	size_t						size_tear;		//The size used in for loops

	std::vector<sf::Sprite>&	decorations;	//Used to store any extranious sprites
	size_t						decor_size;		// (like the players corpse :) )

	bool						modeIsHostile	//What mode is the level in
									= false;
	bool						playerHasWon	//Has the player won yet
									= false;
	bool*						gameOverFlag;	//Is it game over?

	//Used to alter the level state
	void						flipHostile(){
		for (size_t i = 0; i < size_guardian; i++){
			guardians[i]->wake();
		}
		for (size_t i = 0; i < size_tear; i++){
			tears[i]->flip();
		}

		modeIsHostile = true;
	}
	void						flipSafe(){
		if (modeIsHostile){
			for (size_t i = 0; i < size_guardian; i++){
				guardians[i]->reset();
			}

			if (modeIsHostile){
				for (size_t i = 0; i < size_tear; i++){
					tears[i]->flip();
				}
				modeIsHostile = false;
			}
		}
		else{
			if (seekerAroundPlayer != nullptr){
				seekerAroundPlayer->reset();
			}
		}
		seekerAroundPlayer = nullptr;
	}
	
	//deals with guardian alerts
	void						checkAlertSystem(){
		if (alertSystem.isAlert){

			alertSystem.isAlert = false;
			for (size_t i = 0; i < size_guardian; i++){
				guardians[i]->alert(*map);
			}

		}
	}

	//check for tear collision, or for stair collision
	void						checkTearCollision(){
		for (size_t i = 0; i < size_tear; i++){
			if (tears[i]->collides(player.getCollision())){
				flipSafe();
				player.resetTimer();

				//delete and remove the tear, and then reduce the incrementor to prevent it from
				//going out of range
				delete tears[i];
				size_tear -= 1;
				tears.erase(tears.begin() + i);
				stairs->removeTear();
				i -= 1;
			}
		}

		if (size_tear == 0){
			if (map->getTile(player.getPosition())->getType() == tt_stair){
				playerHasWon = true;
				if (seekerAroundPlayer != nullptr){
					seekerAroundPlayer->reset();
					seekerAroundPlayer = nullptr;
				}
			}
		}
	}

	//fades out the screen
	void						gameEndedCover(sf::RenderWindow& window){
		sf::Sprite cover(blank);
		sf::Vector2f viewCenter = window.getView().getCenter();
		sf::Vector2f viewSize = window.getView().getSize();
		cover.setPosition(viewCenter - sf::Vector2f(viewSize.x / 2, viewSize.y / 2));
		cover.setTextureRect(sf::IntRect(0, 0, viewSize.x, viewSize.y));
		cover.setColor(sf::Color(0, 0, 0, 15));

		sf::Image finalScreen = window.capture();
		sf::Texture finalScreenTex;
		finalScreenTex.loadFromImage(finalScreen);
		sf::Sprite screen(finalScreenTex);
		screen.setOrigin(finalScreenTex.getSize().x/2,finalScreenTex.getSize().y/2);
	//	screen.setScale(0.5, 0.5);
		screen.setPosition(viewCenter);
		screen.setTextureRect(sf::IntRect(0, 0, finalScreenTex.getSize().x, finalScreenTex.getSize().y));

		//consecutive draws darkens the screen
		for (size_t i = 15; i < 255; i += 7){
			cover.setColor(sf::Color(0, 0, 0, i));
			window.clear();
			window.draw(screen);
			window.draw(cover);
			window.display();
		}
	}

	//Fades out and shows a message for effect. Called when the game is won
	void						runGameWon(sf::RenderWindow&window){
		//Make sure that the level ends
		*gameOverFlag = true;
		gameEndedCover(window);

		sf::View view(player.getPosition(), sf::Vector2f(700, 700));
		window.setView(view);

		sf::Sprite descendingSprite(gameWonTexture);
		descendingSprite.setTextureRect(sf::IntRect(0, 0, 132, 12));
		descendingSprite.setPosition(player.getPosition() - sf::Vector2f(198, 190));
		descendingSprite.setScale(3, 3);

		window.clear();
		player.draw(window);
		window.draw(descendingSprite);
		window.display();

		sf::Clock clock;
		while (clock.getElapsedTime().asSeconds() < 2){

		}

		sf::Sprite cover(blank);
		cover.setTextureRect(sf::IntRect(0, 0, 700, 700));
		cover.setPosition(player.getPosition() - sf::Vector2f(350, 350));
		cover.setColor(sf::Color(0, 0, 0, 20));

		for (size_t i = 20; i < 255; i += 10){
			cover.setColor(sf::Color(0, 0, 0, i));
			window.clear();
			player.draw(window);
			window.draw(descendingSprite);
			window.draw(cover);
			window.display();
		}

	}

	//Runs a game over in the event the game is indeed over
	void						runGameOver(sf::RenderWindow& window){
		gameEndedCover(window);

		for (size_t i = 0; i < 29; i++){
			player.runGameOver();
			window.clear();
			player.draw(window);
			window.display();
		}

		sf::View view(player.getPosition(), sf::Vector2f(700, 700));
		window.setView(view);

		sf::Sprite gameOverSprite(gameOverTexture);
		gameOverSprite.setTextureRect(sf::IntRect(0, 0, 60, 32));
		gameOverSprite.setPosition(player.getPosition()-sf::Vector2f(90,190));
		gameOverSprite.setScale(3, 3);

		Options_Menu menu(
			player.getPosition() - sf::Vector2f(80,-70),
			std::vector<std::string>{"continue","return to menu"},
			whiteTextTexture,
			20,
			4,
			sf::Color::Red,
			PointerInfo{guardianTexture,sf::IntRect(12,68,21,20),21,3}
		);

		bool _continue_ = false;
		bool keepGoing;
		while (window.isOpen() && ! _continue_){
			sf::Event event;
			while (window.pollEvent(event)){
				switch (event.type){
				case sf::Event::Closed:
					window.close();
					break;
				}
			}
			menu.update();

			window.clear();

			player.draw(window);
			window.draw(gameOverSprite);

			menu.draw(window);

			window.display();

			if (menu.hasChoice()){
				keepGoing = menu.getChoice() == 1;
				_continue_ = true;
			}
		}

		if (!keepGoing){
			global::returnToMenu = true;
			return;
		}
	}
};

#endif