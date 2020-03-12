#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H true

#include "Text.h"

struct PointerInfo{
	sf::Texture& tex;
	sf::IntRect rect;
	suint width;
	suint widthFromOptions;
};

class Options_Menu{
public:

	inline bool hasChoice(){
		return optionChosen;
	}
	//1-x, not 0 indexed
	inline size_t getChoice(){
		return choiceNumber;
	}

	void draw(sf::RenderWindow& window){
		window.draw(pointer);
		for (size_t i = 0; i < numOfOptions; i++){
			options[i]->draw(window);
		}
	}

	void update(){
		if (inputCooldown > 0){
			inputCooldown -= 1;
			return;
		}
		//else...
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
			choiceNumber -= 1;
			if (choiceNumber == 0){
				choiceNumber = numOfOptions;
			}
			pointer.setPosition(startPosition + sf::Vector2f(-totalXDisplace, (charSize + spacing)*(choiceNumber - 1)));
			inputCooldown = 5;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
			choiceNumber += 1;
			if (choiceNumber > numOfOptions){
				choiceNumber = 1;
			}
			pointer.setPosition(startPosition + sf::Vector2f(-totalXDisplace, (charSize + spacing)*(choiceNumber - 1)));
			inputCooldown = 5;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
			optionChosen = true;
		}
	}

	Options_Menu(sf::Vector2f position,std::vector<std::string> optionNames, sf::Texture& font, suint charSize,suint spacing, sf::Color optionColour, PointerInfo pointerInfo):
		numOfOptions(optionNames.size()),
		charSize(charSize),
		spacing(spacing),
		pointer(pointerInfo.tex),
		startPosition(position),
		totalXDisplace(pointerInfo.width+pointerInfo.widthFromOptions)
	{
		for (size_t i = 0; i < numOfOptions; i++){
			options.push_back(new Text_String(optionNames[i], sf::Vector2f(position.x, position.y + ((charSize + spacing)*i)), charSize, font,2));
			options[i]->changeColour(optionColour);
		}

		pointer.setTextureRect(pointerInfo.rect);
		pointer.setPosition(position - sf::Vector2f(totalXDisplace, 0));
	}

	~Options_Menu(){
		for (size_t i = 0; i < numOfOptions; i++){
			delete options[i];
		}
	}
private:
	sf::Sprite					pointer;
	bool						optionChosen = false;
	std::vector<Text_String*>	options;
	size_t						numOfOptions;
	size_t						choiceNumber = 1;

	suint						charSize;
	suint						spacing;
	suint						totalXDisplace;
	sf::Vector2f				startPosition;

	suint						inputCooldown = 0;
};


#endif