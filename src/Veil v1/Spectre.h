#ifndef SPECTRE_H
#define SPECTRE_H true

#include "Guardian base.h"

const suint spectre_alert_radius = 250;

class Spectre : public Guardian_Base{
public:
	//All the virtual functions previously defined. Draw isn't overriden, neither is reset. The pure virtual function works
	void		wake()override{
		currentMode = Guardian_Mode::gm_patrolling;
		forcePathReload();
	}
	void		update(Tile_Map& map, Player& player)override{
		if (currentMode == gm_statue)return;


		if (rand() % 10 == 0){
			createMistParticle(26, 26);
		}
		switch (currentMode){
			//most likely, so check first
		case Guardian_Mode::gm_attacking:
			runAttack();
			break;
		case Guardian_Mode::gm_chasing_blind:
			moveAlongPath(6, map, player);
			if (canSeePlayer(map, player)){
				currentMode = Guardian_Mode::gm_chasing_sight;
				forcePathReload();
			}
			break;
		case Guardian_Mode::gm_chasing_sight:
			//This makes sure the second it looses sight of the player it will plot a path to
			//about where it last saw the player. This prevents a player from slipping round a
			//corner quickly before the path reloads, which would mean the guardian never rounded
			//the corner
			if (!canSeePlayer(map, player)){
				//If it tryed to get a path when the player is invunrable, it would
				//get a path length of 0 and end up crashing
				if (map.getTile(player.getPosition())->getType() != tt_invunrable) forcePathReload();
				moveAlongPath(6, map, player);
				currentMode = Guardian_Mode::gm_chasing_blind;
			}
			else{
				moveAlongPath(6, map, player);
			}
			if (math::pythagerous(getPosition(), player.getPosition()) <= 30){
				currentMode = Guardian_Mode::gm_attacking;
				player.freeze();
			}
			break;
		case Guardian_Mode::gm_patrolling:
			moveAlongPath(4, map, player);
			if (canSeePlayer(map, player)){
				currentMode = Guardian_Mode::gm_chasing_sight;
				forcePathReload();
			}

			break;
		case Guardian_Mode::gm_returning:
			moveAlongPath(4, map, player);
			if (canSeePlayer(map, player)){
				currentMode = Guardian_Mode::gm_chasing_sight;
				forcePathReload();
			}
			break;
		default:
			//Circling
			throw('f');
		}

		runAnimation();

	}
	void		alert(Tile_Map& map)override{
		if (math::pythagerous(getPosition(), alertSystem.location) < spectre_alert_radius && currentMode!=gm_attacking && currentMode!=gm_chasing_sight && currentMode!= gm_statue){
			plotAlertedPath(alertSystem.location, map);
		}
	}
	void		reset()override{
		Guardian_Base::reset();
		sprite.setTextureRect(statueRect);
	}

	void draw(sf::RenderWindow& window){
		Guardian_Base::draw(window);

		//debugDraw(window);
	}

	Spectre(Real_Coords position,std::vector<Real_Coords> patrolPath,bool* gameOver) :
		Guardian_Base(position,patrolPath,gameOver,false)
	{

		sprite.setTexture(guardianTexture);
		sprite.setPosition(position);
		sprite.setScale(0.7, 0.7);

		sprite.setOrigin(26, 26);
		sprite.setTextureRect(statueRect);

		setupAnimation();
	}
private:

	Animation	upAnimation;		//The animations to run when the spectre
	Animation	downAnimation;		//is moving up or down, and for when it
	Animation	attackUpAnimation;	//starts attacking the player
	Animation	attackDownAnimation;//--

	sf::IntRect statueRect=sf::IntRect(42, 15, 65, 77);

	suint		attackTick=0;		//The timer to keep track of attack

	//Runs whichever animation is currently appropriate
	void runAnimation(){
		if (yDirect == d_up){
			if (currentMode == Guardian_Mode::gm_attacking){
				if (attackUpAnimation.isFrameChange()){
					sprite.setTextureRect(attackUpAnimation.getCurrentFrame());
				}
			}
			else if (upAnimation.isFrameChange()){
				sprite.setTextureRect(upAnimation.getCurrentFrame());
			}
		}
		else{
			if (currentMode == Guardian_Mode::gm_attacking){
				if (attackDownAnimation.isFrameChange()){
					sprite.setTextureRect(attackDownAnimation.getCurrentFrame());
				}
			}
			else if (downAnimation.isFrameChange()){
				sprite.setTextureRect(downAnimation.getCurrentFrame());
			}
		}
	}

	//Runs the attack animation and checks
	void		runAttack(){
		attackTick += 1;
		if (attackTick == 20){
			*gameOver = true;
		}
	}

	//Called in the constructor to setup the animations
	void setupAnimation(){
		std::vector<sf::IntRect> framesDown = {
			sf::IntRect(111, 15, 74, 74),
			sf::IntRect(188, 15, 74, 74),
			sf::IntRect(263, 15, 74, 74),
			sf::IntRect(188, 15, 74, 74)
		};
		downAnimation = Animation(framesDown, 5);

		std::vector<sf::IntRect> framesUp= {
			sf::IntRect(111, 91, 74, 74),
			sf::IntRect(188, 91, 74, 74),
			sf::IntRect(263, 91, 74, 74),
			sf::IntRect(188, 91, 74, 74)
		};
		upAnimation = Animation(framesUp, 5);


		std::vector<sf::IntRect> framesDownAttack{
			sf::IntRect(340, 91, 76, 74),
			sf::IntRect(417, 91, 76, 74),
			sf::IntRect(494, 91, 76, 74),
			sf::IntRect(571, 91, 76, 74)
		};
		attackDownAnimation = Animation(framesDownAttack, 3);

		std::vector<sf::IntRect> framesUpAttack{
			sf::IntRect(340, 92, 76, 75),
			sf::IntRect(417, 92, 76, 75),
			sf::IntRect(494, 92, 76, 75),
			sf::IntRect(571, 92, 76, 75)
		};
		attackUpAnimation = Animation(framesUpAttack, 3);
	}
};

#endif