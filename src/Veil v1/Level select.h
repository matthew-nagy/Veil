#ifndef LEVEL_SELECT_H
#define LEVEL_SELECT_H true

#include "Globals file access.h"

int levelSelect(sf::RenderWindow& window,std::vector<std::string>& levels){
	std::vector<std::string>options;
	if (global::levelOn >= levels.size()) options = levels;
	else{
		for (size_t i = 0; i < global::levelOn; i++){
			options.push_back(levels[i]);
		}
	}



	Options_Menu levelMenu(sf::Vector2f(250, 200), options, whiteTextTexture, 20, 2, sf::Color(0, 255, 255), { guardianTexture, sf::IntRect(14, 29, 17, 21), 17, 8 });

	//prevent immidiate input
	_sleep(300);

	while (window.isOpen() && !levelMenu.hasChoice()){
		sf::Event event;
		while (window.pollEvent(event)){
			switch (event.type){
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		levelMenu.update();
		window.clear();
		levelMenu.draw(window);
		window.display();
	}

	if (levelMenu.hasChoice()){
		//its 0 indexed
		return levelMenu.getChoice()-1;
	}
	return 0;
}

#endif