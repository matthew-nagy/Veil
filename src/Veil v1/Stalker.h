#ifndef STALKER_H
#define STALKER_H true

#include "Guardian base.h"

//Much larger than a spectre
const suint stalker_alert_radius = 400;

class Stalker : public Guardian_Base{
public:
	//All the virtual functions previously defined. Draw isn't overriden
	void		wake()override{
		currentMode = gm_patrolling;
		forcePathReload();
	}
	void		reset()override{
		Guardian_Base::reset();
		sprite.setTextureRect(statueRect);
	}
	void		update(Tile_Map& map, Player& player)override{
		if (currentMode == gm_statue)return;

		if (rand() % 5 == 0){
			createMistParticle(26, 26);
		}
		if (rand() % 2 == 0){
			createSmokeParticle(26, 26);
		}
		switch (currentMode){
		case gm_chasing_blind:
			//nothing- will run into the patroling code
		case gm_patrolling:
			moveAlongPath(4, map, player);
			if (canSeePlayer(map, player)){
				currentMode = gm_chasing_sight;
				forcePathReload();
			}
			break;
		//Will never lose sight of the player
		case gm_chasing_sight:
			moveAlongPath(4, map, player);

			//Make sure to alert nearby guardians as to where the player is
			alertTick -= 1;
			if (alertTick == 0){
				alertSystem.isAlert = true;
				alertSystem.location = player.getPosition();
				alertTick = 10;
			}

			if (math::pythagerous(sprite.getPosition(), player.getPosition()) <= 40){
				currentMode = gm_attacking;
				player.freeze();
			}
			break;
		case gm_attacking:
			runAttack();
			break;
		case Guardian_Mode::gm_returning:
			moveAlongPath(4, map, player);
			if (canSeePlayer(map, player)){
				currentMode = Guardian_Mode::gm_chasing_sight;
				forcePathReload();
			}
			break;
		default://circling
			throw('f');
			break;
		}
	}
	void		alert(Tile_Map& map)override{
		if (math::pythagerous(getPosition(), alertSystem.location) < stalker_alert_radius && currentMode != gm_attacking && currentMode != gm_chasing_sight && currentMode != gm_statue){
			plotAlertedPath(alertSystem.location, map);
		}
	}

	Stalker(Real_Coords position, std::vector<Real_Coords> patrolPath, bool* gameOver) :
		Guardian_Base(position, patrolPath, gameOver,true)
	{
		sprite.setTexture(guardianTexture);
		sprite.setOrigin(30, 40);
		sprite.setPosition(position);
		sprite.setScale(0.7, 0.7);
		sprite.setTextureRect(statueRect);

		setupAnimations();
	}
private:

	suint		attackTick = 0;				//Used to regulate the attack

	//Called when attacking the player
	void		runAttack(){
		attackTick += 1;
		if (attackAnimation.isFrameChange()){
			sprite.setTextureRect(attackAnimation.getCurrentFrame());
		}
		if (attackTick == 20){
			*gameOver = true;
		}
	}

	//Breaks a wall blocking their path
	void		runBreakWall(Tile_Map& map){
		attackTick += 1;
		if (attackAnimation.isFrameChange()){
			sprite.setTextureRect(attackAnimation.getCurrentFrame());
		}
		if (attackTick == 20){
			sprite.setTextureRect(walkAnimation.getCurrentFrame());
			attackTick = 0;
			breakNextTile();
		}
	}

	const sf::IntRect statueRect =				//The rect for when dormant
		sf::IntRect(31, 164, 76, 82);		
	Animation	walkAnimation;				//The animations for walking about
	Animation	attackAnimation;			//and attacking the player

	suint		alertTick = 10;				//Will alert to the players location when it has a line of sight

	//Called in the constructor to create the animations
	void setupAnimations(){
		std::vector<sf::IntRect> walkFrames{
			sf::IntRect(187, 167, 75, 76),
			sf::IntRect(264, 167, 75, 76),
			sf::IntRect(341, 167, 75, 76),
			sf::IntRect(418, 167, 75, 76),
			sf::IntRect(485, 167, 75, 76)
		};
		walkAnimation = Animation(walkFrames, 5);

		std::vector<sf::IntRect> attackFrames{
			sf::IntRect(109, 244, 76, 76),
			sf::IntRect(186, 244, 76, 76),
			sf::IntRect(263, 244, 76, 76),
			sf::IntRect(340, 244, 76, 76)
		};
		attackAnimation = Animation(attackFrames, 4);
	}

	//An override that will deal with breaking walls
	void moveAlongPath(suint speed, Tile_Map& map, Player& player)override{
		if (nextIsBreakable()){
			runBreakWall(map);
		}
		else{
			Guardian_Base::moveAlongPath(speed, map, player);
			float xScale = sprite.getScale().x;
			if (xScale > 0 && xDirect == d_left){
				sprite.setScale(-0.7, 0.7);
			}
			else if (xScale < 0 && xDirect == d_right){
				sprite.setScale(0.7, 0.7);
			}

			if (walkAnimation.isFrameChange()){
				sprite.setTextureRect(walkAnimation.getCurrentFrame());
			}
		}
	}
};

#endif