#include "Current Levels.h"
#include "Start menu.h"
#include "Level select.h"
#include "Parse file.h"


int main(){
	setupGlobals();

	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(800, 800), "Veil v1");
	window.setFramerateLimit(30);
	particleTexture.loadFromFile("Art/Particles.png");
	guardianTexture.loadFromFile("Art/Guardians.png");
	
	gameOverTexture.loadFromFile("Art/game over.png");
	gameWonTexture.loadFromFile("Art/descending.png");
	whiteTextTexture.loadFromFile("Art/Big text.png");

	playerTexture.loadFromFile("Art/Player.png");

	//playLevel(getLevelMap, "Data/temporary_test.veil", window);

	std::vector<std::string> levelNames = getLevelNames();

	int currentLevel;
	global::returnToMenu = true;

	while (window.isOpen()){
		if (global::returnToMenu){
			window.setView(window.getDefaultView());

			Menu_Option option = mainMenu(window);
			switch (option){
			case mo_selectLevel:
				currentLevel = levelSelect(window, levelNames);
				break;
			case mo_play:
				currentLevel = 0;
				break;
			case mo_quitGame:
				window.close();
				return 0;
			}
			global::returnToMenu = false;
		}
		else{
			currentLevel += 1;
			//level on is 1 indexed, so >=
			if (currentLevel >= global::levelOn){
				global::levelOn += 1;
				saveGlobals();
			}
			if (currentLevel == levelNames.size()){
				window.close();
				return 0;
			}
		}

		playLevel(getLevelMap, "Data/"+levelNames[currentLevel]+".veil", window);

	}

	
	return 0;
}