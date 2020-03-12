#include "Options menu.h"

extern sf::Texture guardianTexture;
extern sf::Texture blank;
extern sf::Texture whiteTextTexture;

enum Menu_Option{
	mo_quitGame,
	mo_play,
	mo_selectLevel
};


Menu_Option mainMenu(sf::RenderWindow& window){
	sf::Texture shadow;
	shadow.loadFromFile("Art/Shadow.png");

	sf::Sprite shadowSprite(shadow, sf::IntRect(0, 0, 100, 100));
	shadowSprite.setPosition(0, 0);
	shadowSprite.setScale(8, 8);

	sf::Sprite guardianSprite(guardianTexture, sf::IntRect(129, 169, 48, 73));
	guardianSprite.setScale(10, 10);
	guardianSprite.setPosition(-510, 60);

	for (size_t i = 0; i < 20; i++){
		guardianSprite.move(25, 0);
		window.clear();
		window.draw(guardianSprite);
		window.draw(shadowSprite);
		window.display();
	}

	sf::CircleShape circle(80,80);
	circle.setOrigin(80, 80);
	circle.setPosition(400, 400);
	circle.setTexture(&blank);
	circle.setFillColor(sf::Color(240,240,240));
	for (size_t i = 0; i < 6; i++){
		window.clear();
		window.draw(guardianSprite);
		window.draw(shadowSprite);
		window.draw(circle);
		window.display();
		circle.setRadius(circle.getRadius() + 100);
		circle.move(-100, -100);
	}

	PointerInfo info{
		guardianTexture,
		sf::IntRect(14,29,17,21),
		17,
		8
	};
	Options_Menu menu(sf::Vector2f(340, 450), std::vector < std::string > {"play game", "level select", "exit game"}, whiteTextTexture, 20, 2, sf::Color(0, 255, 255), info);
	
	sf::Texture veilTitleTex;
	veilTitleTex.loadFromFile("Art/Veil title.png");
	sf::Sprite titleSprite(veilTitleTex, sf::IntRect(0, 0, 30, 7));
	titleSprite.setScale(10, 10);
	titleSprite.setOrigin(15, 3.5);
	titleSprite.setPosition(450, 360);
	
	sf::Sprite tearSprite(guardianTexture, sf::IntRect(11, 26, 25, 29));
	tearSprite.setScale(2, 2);
	tearSprite.setOrigin(12, 15);
	tearSprite.setPosition(495, 305);

	suint alpha = 255;
	for (size_t i = 0; i < 50; i++){
		alpha -= 5;
		circle.setFillColor(sf::Color(240, 240, 240, alpha));
		window.clear();
		window.draw(guardianSprite);
		menu.draw(window);
		window.draw(titleSprite);
		window.draw(tearSprite);
		window.draw(shadowSprite);
		window.draw(circle);
		window.display();
	}

	while (window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event)){
			switch (event.type){
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		menu.update();
		if (menu.hasChoice()){
			switch (menu.getChoice()){
			case 1:
				return mo_play;
			case 2:
				return mo_selectLevel;
			case 3:
				return mo_quitGame;
			}
		}

		window.clear();
		window.draw(guardianSprite);

		menu.draw(window);
		window.draw(titleSprite);
		window.draw(tearSprite);

		window.draw(shadowSprite);

		window.display();
	}

}