#ifndef GUARDIAN_PARTICLES_H
#define GUARDIAN_PARTICLES_H true
#include "Particles.h"
extern sf::Texture particleTexture;

/*Visual_Data(sf::Texture* texture, sf::IntRect rect, sf::Color colour, short unsigned fade, float spin) :
texture(texture),
rect(rect),
colour(colour),
fade(fade),
spin(spin)
{}*/

/*short unsigned	fadeoutDelay;	//The time before it starts to fade
		float			spinFade;		//How quickly the rotation stops

		bool			colourAlter;	//If this is set to false, the next 2 are ignored
		sf::Color		transitionColour;//The colour it will transition to
		short unsigned	transitionTime;	//How many frames for the transition to be complete
		short unsigned transitionDelay; //How long before the transition begins
*/

/*Mechanical_Data(double speed, double angle, double curve, double speedFadeOut, short lifespan) :
speed(speed),
angle(angle),
curve(curve),
speedFadeOut(speedFadeOut),
lifespan(lifespan)
{}*/
ps::Visual_Data smoke_vis[2]{
	{&particleTexture, sf::IntRect(6, 7, 3, 3), sf::Color(175, 0, 0, 210), 10, 2},
	{ &particleTexture, sf::IntRect(6, 7, 3, 3), sf::Color(87, 0, 127, 210), 10, 2 }
};
ps::SP_Visual_Data smoke_sp[3]{
	{20, 1, true, sf::Color(0, 0, 0), 25, 5},
	{ 20, 1, true, sf::Color(53, 0, 0), 25, 5 },
	{ 20, 1, true, sf::Color(59, 0, 86), 25, 5 }
};
ps::Mechanical_Data smoke_mech(
	5,
	0,
	0,
	0.97,
	40);

ps::Visual_Data mist_vis(
	&particleTexture,
	sf::IntRect(5, 11, 8, 5),
	sf::Color(47, 0, 44, 150),
	4,
	0);
ps::Mechanical_Data mist_mech(
	2,
	0,
	0,
	1,
	35);

#endif