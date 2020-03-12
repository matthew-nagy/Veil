#ifndef PARTICLES_H
#define PARTICLES_H true

#include "SFML\Graphics.hpp"
#include "Math.h"
#include <stdlib.h>
#include <time.h> 

//Particle system
namespace ps{
	//Defined later
	struct Mechanical_Data;
	class Particle;

	//Data about how the particle will look
	struct Visual_Data{
		sf::Texture*	texture;	//The texture to get the rect from
		sf::IntRect		rect;		//The textureRect of the particle
		sf::Color		colour;		//Colour of the particle (input is white)
		short unsigned	fade;		//Fade per frame to the opacity
		float			spin;		//The rotation per frame

		Visual_Data(sf::Texture* texture, sf::IntRect rect, sf::Color colour, short unsigned fade, float spin) :
			texture(texture),
			rect(rect),
			colour(colour),
			fade(fade),
			spin(spin)
		{}

		//set to nullptr so it doesn't have to be initilised
		//The data of the trail it sends behind it
		Visual_Data*	trail = nullptr;
		Mechanical_Data* mech = nullptr;
	};

	//More data about how it will look, that not every particle may need
	struct SP_Visual_Data{
		short unsigned	fadeoutDelay;	//The time before it starts to fade
		float			spinFade;		//How quickly the rotation stops

		bool			colourAlter;	//If this is set to false, the next 2 are ignored
		sf::Color		transitionColour;//The colour it will transition to
		short unsigned	transitionTime;	//How many frames for the transition to be complete
		short unsigned transitionDelay; //How long before the transition begins

		SP_Visual_Data(){}

		SP_Visual_Data(short unsigned fadeoutDelay, float spinFade, bool colourAlter, sf::Color transitionColour, short unsigned transitionTime, short unsigned transitionDelay) :
			fadeoutDelay(fadeoutDelay),
			spinFade(spinFade),
			colourAlter(colourAlter),
			transitionColour(transitionColour),
			transitionTime(transitionTime),
			transitionDelay(transitionDelay)
		{}
	};

	//Data about how the particle will behave
	struct Mechanical_Data{
		double	speed;			//How quickley the particle will travel
		double	angle;			//Where the patrticle will travel
		double	curve;			//Any curvature to the particles travel
		double	speedFadeOut;	//The percentage of speed kept frame to frame
		short	lifespan;		//How long until the particle is deleted

		Mechanical_Data(double speed, double angle, double curve, double speedFadeOut, short lifespan) :
			speed(speed),
			angle(angle),
			curve(curve),
			speedFadeOut(speedFadeOut),
			lifespan(lifespan)
		{}

		//Both set to 0 to prevent neccessary initilisation
		short	framesPerTrail=0;//How many frames before shooting another particle
		float	modAngleVary=0;	//The modulus of how much the angle can vary from 180
								// of the direction this one is traveling
	};

	//Stores information about the system, and is instantiated
	struct _PARTICLE_SYSTEM_INFORMATION_{
		//Modes about overflowing particles
		enum Mode{
			psm_deleteFromFront,
			psm_deleteFromBack,
			psm_deleteByCondition
		};
		//How many particles allowed on screen at once
		short unsigned	maximumParticleNum = 200;
		//Stores the current number of particles
		size_t			particleNumber = 0;
		//How overflowing particles will be handled
		Mode			deletionMode = psm_deleteFromFront;
		//The function called when mode is psm_conditional
		void			(*condition)(std::vector<Particle*>&,size_t&)=nullptr;
	}systemInfo;


	//toAdd is used to keep seperation when multithreading is used later
	std::vector<Particle*> toAdd;
	std::vector<Particle*> liveParticles;

	class Particle{
	public:
		//Draws the sprite to the screen
		void draw(sf::RenderWindow& window){
			window.draw(sprite);
		}

		//Updates the particle sprite
		void update(){
			age += 1;

			updateParticle();

			alterValues();

			if (age >= d_mechanical.lifespan)_shouldKill = true;
		}

		//Returns true if the particle should be deleted
		inline bool shouldKill()const{
			return _shouldKill;
		}

		Particle(sf::Vector2f position, Visual_Data d_visual, Mechanical_Data d_mechanical) :
			d_visual(d_visual),
			d_mechanical(d_mechanical),
			sprite(*d_visual.texture),
			hasVisualSP(false),
			hasTrail(d_visual.trail!=nullptr)
		{
			sprite.setTexture(*d_visual.texture);
			sprite.setOrigin(d_visual.rect.width / 2, d_visual.rect.height / 2);
			sprite.setTextureRect(d_visual.rect);
			sprite.setPosition(position);
			sprite.setColor(d_visual.colour);

			toAdd.push_back(this);
		}

		Particle(sf::Vector2f position, Visual_Data d_visual, SP_Visual_Data d_visualSP, Mechanical_Data d_mechanical) :
			d_visual(d_visual),
			d_visualSP(d_visualSP),
			d_mechanical(d_mechanical),
			sprite(*d_visual.texture),
			hasVisualSP(true),
			hasTrail(d_visual.trail != nullptr)
		{
			sprite.setTexture(*d_visual.texture);
			sprite.setOrigin(d_visual.rect.width / 2, d_visual.rect.height / 2);
			sprite.setTextureRect(d_visual.rect);
			sprite.setPosition(position);
			sprite.setColor(d_visual.colour);

			if (d_visualSP.colourAlter){
				alterToRedVal = (d_visualSP.transitionColour.r - d_visual.colour.r) / d_visualSP.transitionTime;
				alterToGreenVal = (d_visualSP.transitionColour.g - d_visual.colour.g) / d_visualSP.transitionTime;
				alterToBlueVal = (d_visualSP.transitionColour.b - d_visual.colour.b) / d_visualSP.transitionTime;
			}

			toAdd.push_back(this);
		}

	private:
		sf::Sprite sprite;				//The particles sprite
		short unsigned age = 0;			//How many frames the particle has exited for
		bool _shouldKill = false;		//Whether the particle has reached its age limit

		Visual_Data d_visual;			//The particles visua data
		SP_Visual_Data d_visualSP;		//The particles optional SP visual data
		Mechanical_Data d_mechanical;	//The particles mechanical data

		bool hasVisualSP;				//Variables beyond the line are only used if a SP_Visual_Data struct was passed
										//  to the constructor.
		bool hasTrail;					//If the particle is supposed to leave a trail, this is true

		//////////////////////////////////////////////////////////////////////

		short alterToRedVal;			//The change in the red value per frame
		short alterToGreenVal;			//" " green " "
		short alterToBlueVal;			//" " blue " "

		void updateParticle(){
			sprite.move(math::getMove(d_mechanical.speed, d_mechanical.angle));
			sprite.rotate(d_visual.spin);
			if (!hasTrail) return;

			//Otherwise it can leave a trail, deal with here
			if (age%d_mechanical.framesPerTrail == 0){
				int twiceModVal = 2 * d_mechanical.modAngleVary;
				double alteredAngle = -d_mechanical.modAngleVary + (rand()%twiceModVal);
				d_visual.mech->angle = d_mechanical.angle + 180;
				if (d_visual.mech->angle > 360) d_visual.mech->angle -= 360;

				new Particle(sprite.getPosition(), *d_visual.trail, *d_visual.mech);
			}
		}

		void alterValues(){
			if (d_visual.fade != 0){
				sf::Color newColour = sprite.getColor();
				if (hasVisualSP){
					if (age > d_visualSP.fadeoutDelay){
						newColour.a -= d_visual.fade;
					}
				}
				else{
					newColour.a -= d_visual.fade;
				}
				sprite.setColor(newColour);
			}

			d_mechanical.angle += d_mechanical.curve;
			if (d_mechanical.angle > 360)d_mechanical.angle -= 360;
			else if (d_mechanical.angle < 0)d_mechanical.angle += 360;

			d_mechanical.speed = d_mechanical.speed*d_mechanical.speedFadeOut;

			if (hasVisualSP){

				d_visual.spin *= d_visualSP.spinFade;
				if (d_visualSP.colourAlter && age<d_visualSP.transitionTime && age>d_visualSP.transitionDelay){
					sf::Color newColour = sprite.getColor();
					newColour.r += alterToRedVal;
					newColour.g += alterToGreenVal;
					newColour.b += alterToBlueVal;
					sprite.setColor(newColour);
				}

			}
		}
	};

	/*Due to the calculations and operations called here for large
	numbers of particles this function should be run on a seperate thread
	to prevent lag*/
	void updateParticles(){
		//Cull them if needed
		if (systemInfo.particleNumber>systemInfo.maximumParticleNum){
			if (systemInfo.deletionMode = _PARTICLE_SYSTEM_INFORMATION_::psm_deleteFromFront){
				do{
					delete liveParticles[0];
					liveParticles.erase(liveParticles.begin());
					systemInfo.particleNumber -= 1;
				} while (systemInfo.particleNumber > systemInfo.maximumParticleNum);
			}
			else if (systemInfo.deletionMode = _PARTICLE_SYSTEM_INFORMATION_::psm_deleteFromBack){
				do{
					delete liveParticles[systemInfo.particleNumber - 1];
					liveParticles.pop_back();
					systemInfo.particleNumber -= 1;
				} while (systemInfo.particleNumber>systemInfo.maximumParticleNum);
			}
			else{
				do{
					(*systemInfo.condition)(liveParticles, systemInfo.particleNumber);
				} while (systemInfo.particleNumber > systemInfo.maximumParticleNum);
			}
		}


		for (size_t i = 0; i < systemInfo.particleNumber; i++){
			liveParticles[i]->update();

			if (liveParticles[i]->shouldKill()){
				systemInfo.particleNumber -= 1;
				delete liveParticles[i];
				liveParticles.erase(liveParticles.begin() + i);
				i -= 1;
			}

		}
	}

	//This adds the stored up new particles to the system
	//As this is a seperate function, added particles don't mess with the other thread
	//that is runnin the partices
	void addNewParticles(){
		for (size_t i = 0; i < toAdd.size(); i++){
			liveParticles.push_back(toAdd[i]);
		}
		systemInfo.particleNumber += toAdd.size();
		toAdd.clear();
	}

	void drawParticles(sf::RenderWindow& window){
		for (size_t i = 0; i < systemInfo.particleNumber; i++){
			liveParticles[i]->draw(window);
		}
	}

	void deleteAllParticles(){
		for (size_t i = 0; i < toAdd.size(); i++){
			delete toAdd[i];
		}
		toAdd.clear();

		for (size_t i = 0; i < systemInfo.particleNumber; i++){
			delete liveParticles[i];
		}
		liveParticles.clear();
		systemInfo.particleNumber = 0;
	}
}

#endif