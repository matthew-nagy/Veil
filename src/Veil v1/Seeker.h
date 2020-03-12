#ifndef SEEKER_H
#define SEEKER_H true

#include "Guardian base.h"

class Seeker;

//Used to prevent multiple Seekers surrounding the player
Seeker* seekerAroundPlayer = nullptr;

class Seeker : public Guardian_Base{
public:
	//All the virtual functions previously defined.
	void		draw(sf::RenderWindow& window)override{
		window.draw(sightSprite);
		window.draw(sprite);
	}
	//Just returns, because it never becomes a statue
	void		wake()override{
		return;
	}
	void		reset()override{
		Guardian_Base::reset();

		//set it to patroling, make sure that it reloads the path
		currentMode = Guardian_Mode::gm_patrolling;
		sprite.setColor(sf::Color(255, 255, 255, 100));
		resetTick = 15;
		sightSprite.setPosition(sprite.getPosition());
	}

	//Runs the AI states
	void		update(Tile_Map& map, Player& player)override{
		//Makes sure that it resets
		if (resetTick != 0){
			sprite.setColor(sf::Color(255, 255, 255, 255 - 15 * resetTick));
			resetTick -= 1;
			return;
		}
		
		checkAnimation();

		if (rand() % 15 == 0){
			createSmokeParticle(13, 11);
		}

		//Run the finite state AI
		switch (currentMode){
		case Guardian_Mode::gm_patrolling:
			moveAlongPath(2, map, player);

			if (math::pythagerous(sprite.getPosition(), player.getPosition()) < sightRadius){
				if (canSeePlayer(map, player) && seekerAroundPlayer==nullptr){
					currentMode = Guardian_Mode::gm_circling;
					seekerAroundPlayer = this;
					distanceToPlayer = math::pythagerous(sprite.getPosition(), player.getPosition());
					player.speedUpCountDown();
					playerLastPosition = player.getPosition();
				}
			}
			break;
		case Guardian_Mode::gm_circling:
			alertTimer -= 1;
			if (alertTimer == 0){
				alertTimer = 5;
				alertSystem.isAlert = true;
				alertSystem.location = player.getPosition();
			}
			circlePlayer(player, map);
			break;
		default:
			//Statue, attacking, chasing with or without sight or returning
			//Shouldn't ever be these
			//throw an error
			throw('g');
		}

		sightSprite.setPosition(sprite.getPosition());
	}

	//Seekers have to need to be alerted by other seekers
	void		alert(Tile_Map& map)override{
		return;
	}

	Seeker(Real_Coords position, std::vector<Real_Coords> patrolPath, bool* gameOver) :
		Guardian_Base(position, patrolPath,gameOver,false)
	{
		sprite.setTexture(guardianTexture);
		setupAnimation();
		sprite.setTextureRect(downAnimation.getCurrentFrame());

		sprite.setOrigin(15, 15);
		sprite.setScale(0.7, 0.7);
		sprite.setPosition(position);

		sightSprite.setTexture(&blank);
		sightSprite.setOrigin(sightRadius, sightRadius);
		sightSprite.setPosition(position);
		sightSprite.setFillColor(sf::Color(40, 0, 40, 120));
		sightSprite.setRadius(sightRadius);

		currentMode = Guardian_Mode::gm_patrolling;
	}

private:
	double		angleToPlayer = 0;	//The current angle of rotation around the player
	double		distanceToPlayer;	//Radius around player currently

	const							//The maximum radius around the player
		double  maxDistanceToPlayer = 30;
	Real_Coords	playerLastPosition;	//Used to move the sprite to match the player
	suint		alertTimer = 5;	//How often it will alert other guardians

	//also deals with exiting the collision
	void		circlePlayer(Player& player,Tile_Map& map){
		//make sure to move with the player first
		sprite.move(player.getPosition() - playerLastPosition);
		playerLastPosition = player.getPosition();

		sprite.setPosition(math::rotateAroundPoint(player.getPosition(), sprite.getPosition(), 4));
		sf::Vector2f velocityAway = math::getMove(1, math::getAngle(player.getPosition(), sprite.getPosition()));

		if (inCollision(map)){
			do{
				sprite.move(-velocityAway.x,-velocityAway.y);
				distanceToPlayer -= 1;
			} while (inCollision(map) && distanceToPlayer>10);
		}
		else{
			suint moveCounter = 0;
			while ((!inCollision(map)) && distanceToPlayer < maxDistanceToPlayer && moveCounter<3){
				sprite.move(velocityAway);
				distanceToPlayer += 1;
				moveCounter += 1;
			}
			//move back out
			sprite.move(-velocityAway.x, -velocityAway.y);
			distanceToPlayer -= 1;
		}

		if (math::pythagerous(sprite.getPosition(), player.getPosition()) != distanceToPlayer){
			readjustDistance(player,velocityAway);
		}
	}

	//Adjusts distance to the player to make it equal to the distanceToPlayer variable
	//If something caused them to de-couple
	void		readjustDistance(Player& player,sf::Vector2f& velocityAway){

		double distance = math::pythagerous(sprite.getPosition(), player.getPosition());
		if (distance > distanceToPlayer){
			double multiplier = distance - distanceToPlayer;
			sprite.move(-velocityAway.x*multiplier, -velocityAway.y*multiplier);
		}
		else{
			double multiplier = distanceToPlayer - distance;
			sprite.move(velocityAway.x*multiplier, velocityAway.y*multiplier);
		}
	}

	//Checks to see if the sprite is in the wall
	bool		inCollision(Tile_Map& map){
		sf::Vector2f centre = sprite.getPosition();
		if (map.collides(centre + sf::Vector2f(-14, -14))) return true;
		if (map.collides(centre + sf::Vector2f(14, -14))) return true;
		if (map.collides(centre + sf::Vector2f(-14, 14))) return true;
		if (map.collides(centre + sf::Vector2f(14, 14))) return true;
		return false;
	}

	//Runs animation
	void		checkAnimation(){
		sf::Vector2f scale = sprite.getScale();
		if ((xDirect == d_right && scale.x < 0) || (xDirect == d_left && scale.x > 0)){
			scale.x *= -1;
			sprite.setScale(scale);
		}

		if (yDirect == d_up){
			if (upAnimation.isFrameChange()){
				sprite.setTextureRect(upAnimation.getCurrentFrame());
			}
		}
		else{
			if (downAnimation.isFrameChange()){
				sprite.setTextureRect(downAnimation.getCurrentFrame());
			}
		}

		if (yDirect != lastAnimation){
			lastAnimation = yDirect;
			if (yDirect == d_up){
				sprite.setTextureRect(upAnimation.getCurrentFrame());
			}
			else{
				sprite.setTextureRect(downAnimation.getCurrentFrame());
			}
		}
	}

	Animation	downAnimation;		//The animations used on the
	Animation	upAnimation;		//	sprite depending on direction
	Direction	lastAnimation		//Used to control animation and scaling
		= d_down;

	sf::CircleShape sightSprite;	//The sprite to show the player where they will be seen
	suint		sightRadius=30;		//The radius the player must enter to be seen
	suint resetTick = 0;			//The timer after reseting for the fadeIn

	//Used on initilisation to create the animations
	void		setupAnimation(){
		std::vector<sf::IntRect> downFrames;
		downFrames.push_back(sf::IntRect(110, 327, 38, 33));
		downFrames.push_back(sf::IntRect(152, 327, 38, 33));
		downFrames.push_back(sf::IntRect(192, 327, 38, 33));
		downFrames.push_back(sf::IntRect(152, 327, 38, 33));

		downAnimation = Animation(downFrames, 10);


		std::vector<sf::IntRect> upFrames;
		upFrames.push_back(sf::IntRect(233, 327, 38, 33));
		upFrames.push_back(sf::IntRect(274, 327, 38, 33));
		upFrames.push_back(sf::IntRect(315, 327, 38, 33));
		upFrames.push_back(sf::IntRect(274, 327, 38, 33));

		upAnimation = Animation(upFrames, 10);
	}

};

#endif