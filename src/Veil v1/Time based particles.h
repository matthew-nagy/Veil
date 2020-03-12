#ifndef TIME_BASED_PARTICLES_H
#define TIME_BASED_PARTICLES_H true

#include "Particles.h"

extern sf::Texture particleTexture;
ps::Visual_Data tear_collect_vis(
	&particleTexture,
	sf::IntRect(0, 7, 5,7),
	sf::Color(0, 255, 255, 150),
	2,
	2.0
	);

ps::Mechanical_Data tear_collect_mech(
	7,
	0,
	0,
	1,
	20
	);


ps::Visual_Data time_runout_vis(
	&particleTexture,
	sf::IntRect(7, 0, 7, 6),
	sf::Color(255, 0, 0, 200),
	5,
	0);
ps::Mechanical_Data time_runout_mech(
	3,
	0,
	0,
	1,
	39);

#endif