#ifndef ANIMATION_CLASS_H
#define ANIMATION_CLASS_H true


typedef short unsigned int suint;
#include "SFML\Graphics.hpp"

class Animation{
public:

	//Incriments the count by 1, then returns true if its time for the next frame
	bool				isFrameChange(){
		currentTick += 1;
		if (currentTick == ticksPerFrame){
			currentFrame += 1;
			if (currentFrame == size){
				currentFrame = 0;
			}
			currentTick = 0;

			return true;
		}
		return false;
	}
	const sf::IntRect&	getCurrentFrame()const{
		return frames[currentFrame];
	}

	//Used in the event animation needs to be sped up. Walking VS running for example
	inline const suint& getTicksPerFrame(){
		return ticksPerFrame;
	}
	inline void			alterFrameRate(short ticks){
		if (ticks*-1 > ticksPerFrame) ticksPerFrame = 1;
		else ticksPerFrame += ticks;
	}

	//Empty so that it can be defined in class
	Animation(){}

	//Frames loaded from frames, ticks per frame is loaded from speed
	Animation(std::vector<sf::IntRect> frames, suint speed) :
		ticksPerFrame(speed),
		frames(frames),
		size(frames.size())
	{

	}

private:

	suint				currentFrame=0;	//The index of the current frame
	suint				currentTick=0;	//The current internal clock tick
	suint				ticksPerFrame;	//The number of internal ticks before the next animation

	std::vector
		<sf::IntRect>	frames;			//All of the frames of animation
	suint				size;			//The number of frames of animation

};

#endif