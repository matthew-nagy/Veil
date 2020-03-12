#ifndef TEXT_H
#define TEXT_H 0

#ifndef SFML_GRAPHICS_API
#include "SFML\Graphics.hpp"
#endif
#include <iostream>


void SOLVE_TEXT_STRING_ERROR(){
	std::cout << "Text string error: enetered string is too long\n";
	char a;
	std::cin >> a;
}

class Text_String{
public:
	//Some getter functions to access private functions
	inline sf::Vector2f		getSize()const{
		return sf::Vector2f((characterSize + 1)*textLength, characterSize);
	}
	inline sf::Vector2f		getPosition()const{
		return position;
	}

	//Sets the top left position of the text
	void					setPosition(sf::Vector2f position){
		for (size_t i = 0; i < textLength; i++){
			sprites[i]->setPosition(position.x + ((characterSize + 1)*i), position.y);
		}
	}

	//Draws all the sprites to the screen
	void					draw(sf::RenderWindow& window)const{
		for (size_t i = 0; i < textLength; i++){
			window.draw(*sprites[i]);
		}
	}

	//Moves the text string
	void					move(float x, float y){
		for (size_t i = 0; i < textLength; i++){
			sprites[i]->move(x, y);
		}
		position.x += x;
		position.y += y;
	}

	//Changes the displayed text
	void					changeText(std::string newString){
		reset();
		setup(newString);
	}

	//Changes the colour of all the sprites. I also refuse to spell it American.
	void					changeColour(sf::Color colour){
		for (size_t i = 0; i < textLength; i++){
			sprites[i]->setColor(colour);
		}
	}

	void speak(){
		sf::IntRect rect1 = sprites[4]->getTextureRect();
		sf::IntRect rect2 = sprites[5]->getTextureRect();
		std::cout << "rects are :" << std::endl << rect1.left << "," << rect1.top << "," << rect1.width << "," << rect1.height << std::endl;
		std::cout << rect2.left << "," << rect2.top << "," << rect2.width << "," << rect2.height << "\n";
	}

	//Sets up the basic textstring
	Text_String(std::string text, sf::Vector2f position, int characterSize, sf::Texture& texture,int characterSpacing=1) :
		position(position),
		assignedTexture(texture),
		characterSize(characterSize),
		characterSpacing(characterSpacing)
	{
		setup(text);
	}

	//Just calls reset
	~Text_String(){
		reset();
	}
private:
	sf::Sprite*				sprites[60];		//Sets a max size, much easier to memory manage
	size_t					textLength;			//The length of the string in characters
	const short unsigned	characterSize;		//The size in pixels of each character
	sf::Vector2f			position;			//The top left position of the text
	sf::Texture&			assignedTexture;	//Stores the texture the sprites are assigned to
	const short unsigned	characterSpacing;	//The space between the character cells in the font

	//Deletes all the sprites
	void					reset(){
		for (size_t i = 0; i < textLength; i++){
			delete sprites[i];
		}
		textLength = 0;
	}

	//Creates and sets up sprites determined on the text string entered
	void					setup(std::string& text){
		if (text.size()>60){
			text = "error, size greater than 60";
			SOLVE_TEXT_STRING_ERROR();
		}
		for (size_t i = 0; i < text.size(); i++){
			sf::Sprite* new_sprite = new sf::Sprite(assignedTexture);
			new_sprite->setPosition(position.x + ((characterSize + 1)*i), position.y);
			new_sprite->setTextureRect(getCharRect(text[i]));
			sprites[i] = new_sprite;
		}

		textLength = text.size();
	}

	//Gets the order of a character as according to White text.png
	sf::IntRect				getCharRect(char character){
		if (character >= 97 && character <= 122){
			return sf::IntRect(((character - 97)*characterSize) + ((character - 97)*characterSpacing), 0, characterSize, characterSize);
		}
		//else
		if (character >= 48 && character <= 57){
			return sf::IntRect(((character - 22)*characterSize) + ((character - 22)*characterSpacing), 0, characterSize, characterSize);
		}
		//else
		switch (character){
		case ',':
			return sf::IntRect((38 * characterSize) + 38*characterSpacing, 0, characterSize, characterSize);
		case '!':
			return sf::IntRect((39 * characterSize) + 39*characterSpacing, 0, characterSize, characterSize);
		case '.':
			return sf::IntRect((37 * characterSize) + 37*characterSpacing, 0, characterSize, characterSize);
		}

		return sf::IntRect(0, 0, 0, 0);
	}
};

#endif