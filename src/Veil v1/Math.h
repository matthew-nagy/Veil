#ifndef MATH_H
#define MATH_H true
#include <math.h>

#ifndef _SFML_GRAPHICS_HPP
#include "SFML\Graphics.hpp"
#endif


#define PI 3.14159265
typedef short unsigned int suint;
typedef short int sint;

namespace math{

	sf::Vector2f getMove(float hypotinuse, double angle){
		//use triganometry to find the new offsets
		double x = sin(angle*PI / 180)*hypotinuse;

		//due to inverted y axis, all y needs to be inverted
		double y = (cos(angle*PI / 180)*hypotinuse)  *-1;

		return sf::Vector2f(x, y);
	}

	double getAngle(sf::Vector2f start, sf::Vector2f target){
		double rotation = (atan2(target.y - start.y, target.x - start.x)) * 180 / PI;

		rotation += 90;

		if (rotation < 0){
			rotation = 360 + rotation;
		}

		return rotation;
	}

	double getAngle(float x, float y, float x2, float y2){
		double rotation = (atan2(y2 - y, x2 - x)) * 180 / PI;

		rotation += 90;

		if (rotation < 0){
			rotation = 360 + rotation;
		}

		return rotation;
	}

	double getAngle(float x, float y){
		double rotation = (atan2(y, x)) * 180 / PI;

		rotation += 90;

		if (rotation < 0){
			rotation = 360 + rotation;
		}
		return rotation;
	}

	inline double pythagerous(float x, float y){
		return sqrt((x*x) + (y*y));
	}

	double pythagerous(sf::Vector2f a, sf::Vector2f b){
		double x = a.x - b.x;
		double y = a.y - b.y;
		return sqrt((x*x) + (y*y));
	}

	double getDifference(double a, double b){
		double toret = a - b;

		if (toret < 0){
			return toret*-1;
		}
		return toret;
	}

	sf::Vector2f rotateAroundPoint(sf::Vector2f origin, sf::Vector2f currentPosition, double anticlockwiseRotation){
		sf::Vector2f fromOrigin = currentPosition - origin;

		anticlockwiseRotation *= PI / 180;

		sf::Vector2f toret;
		toret.x = fromOrigin.x*cos(anticlockwiseRotation) - fromOrigin.y*sin(anticlockwiseRotation);
		toret.y = fromOrigin.x*sin(anticlockwiseRotation) + fromOrigin.y*cos(anticlockwiseRotation);

		return origin + toret;
	}
}

#endif