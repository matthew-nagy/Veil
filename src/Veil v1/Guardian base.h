#ifndef GUARDIAN_BASE_H
#define GUARDIAN_BASE_H true

#include "Tile map.h"
#include "Animation class.h"
#include "Guardian particles.h"
#include "Player class.h"

//An enumeration for the Guardians AI
enum Guardian_Mode{
	gm_statue,			//Unmoving state
	gm_patrolling,		//Patrolling an area
	gm_chasing_sight,	//Chasing a player with a line of sight
	gm_chasing_blind,	//Chasing where they knew the player was
	gm_returning,		//Returning to where they were patroling
	gm_circling,		//Circling the player (Seeker only)
	gm_attacking		//Attacking the player (Spectre + Stalker only)
};

//The texture all guardians will get their sprites and frames from
sf::Texture guardianTexture;

//A system used to alert other guardians of the player's position
struct Alert_Structure{
	bool isAlert = false;
	Real_Coords location;

	inline void reset(){
		isAlert = false;
	}
}alertSystem;//One is immidiately created to act as the alert system

//The base class for all of the guardians
class Guardian_Base{
public:

	/*Will reset the guardian to their spawn point
	and turn it to stone*/
	virtual void		reset(){
		sprite.setPosition(spawnPoint);

		currentPathPointer = 0;
		reloadPathTick = 0;
		patrolPosition = 0;
		currentMode = gm_statue;

	}

	/*Wakes the guardian up*/
	virtual void		wake() = 0;

	//Draws tha sprite to the window
	inline virtual void	draw(sf::RenderWindow& window){
		window.draw(sprite);
	}

	//Runs the guardian
	virtual void		update(Tile_Map &map, Player& player) = 0;

	//Tells the guardians to look at the alert system
	virtual void		alert(Tile_Map& map) = 0;

	//Gets the position of the guardian
	inline Real_Coords getPosition(){
		return sprite.getPosition();
	}

	//Sets up the base, just sets the sprites position
	Guardian_Base(Real_Coords position, std::vector<Real_Coords> patrolPath,bool* gameOver, bool breakWalls) :
		spawnPoint(position),
		patrolPath(patrolPath),
		sprite(guardianTexture),
		gameOver(gameOver),
		breakWalls(breakWalls)
	{
	}

protected:
	//Draws circles to show where the guardian is moving to and how
	void debugDraw(sf::RenderWindow& window){
		for (size_t i = 0; i < currentPath.size(); i++){
			sf::CircleShape shape(5);
			shape.setTexture(&blank);
			shape.setFillColor(sf::Color::Green);
			shape.setPosition(currentPath[i]->getCenter() - sf::Vector2f(2.5, 2.5));
			window.draw(shape);
		}
	}

	void debugCheck(){
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)){
			for (size_t i = 0; i < patrolPath.size(); i++){
				std::cout << patrolPath[i].x << "," << patrolPath[i].y << "\n";
			}
			std::cout << "\n--------\n";
		}
	}

	bool*				gameOver;//The power to trigger a game over should it be needed

	Guardian_Mode		currentMode	//The current guardian mode
						=gm_statue;	//Starts as statue
	sf::Sprite			sprite;		//The sprite. Does what it says on the tin.

	//Their current direction of travel. Used for animations
	Direction			xDirect = d_right;
	Direction			yDirect = d_down;
	
	//Progresses along the current path
	virtual void		moveAlongPath(suint speed,Tile_Map& map,Player& player){

		if (reloadPathTick == 0){
			getNewPath(map, player);
		}
		else{
			reloadPathTick -= 1;
		}

		//This protects from crashing in the event the path is out of range. Normally this doesn't happen, but
		//some errors such as faliure to find a path could result in this. While it is unlikely, it is better to check
		//than to let the program crash
		if (currentPathPointer >= currentPath.size()){
			std::cout << "Path error caught\n";
			forcePathReload();
			return;
		}

		short difference = math::pythagerous(sprite.getPosition(), currentPath[currentPathPointer]->getCenter());
		if (difference < speed){
			sprite.setPosition(currentPath[currentPathPointer]->getCenter());
			currentPathPointer += 1;
			//If it has reached the end of its path, make sure to get a new path
			if (currentPathPointer == currentPath.size()){
				forcePathReload();
			}

		}
		else{
			double angle = math::getAngle(sprite.getPosition(), currentPath[currentPathPointer]->getCenter());
			
			xDirect = angle < 180 && angle>0 ? d_right : d_left;
			yDirect = angle < 90 || angle > 270 ? d_up : d_down;

			sprite.move(math::getMove(speed, angle));
		}
	}

	//Is the next tile a breakable tile
	inline bool			nextIsBreakable(){
		if (currentPathPointer < currentPath.size()){
			return currentPath[currentPathPointer]->getType() == tt_breakable;
		}
		return false;
	}

	//Breaks the wall infront of them. Defined in Special Tiles.h
	inline void			breakNextTile();

	//Used to set reload path tick to 0, reloading the path
	inline void			forcePathReload(){
		reloadPathTick = 0;
	}

	//Called when alerted
	void				plotAlertedPath(Real_Coords& position, Tile_Map& map){
		//unlikely to ever be reset
		reloadPathTick = 10000;
		currentPath = map.getPath(getPosition(), position);
		currentPathPointer = 0;
		currentMode = Guardian_Mode::gm_chasing_blind;
	}

	//Checks to see if the guardian can see the player
	bool				canSeePlayer(Tile_Map& map, Player& player){
		Real_Coords beam = sprite.getPosition();
		double angle = math::getAngle(beam, player.getPosition());
		sf::Vector2f velocity = math::getMove(2, angle);
		size_t runTime = math::pythagerous(beam, player.getPosition()) / 2;

		for (size_t i = 0; i < runTime; i++){
			beam += velocity;
			if (map.collides(beam)){
				if (map.getTile(beam)->getType() != tt_breakable){
					return false;
				}
			}
		}

		//must have reached the player
		if (map.getTile(player.getPosition())->getType() != Tile_Type::tt_invunrable) return true;
		//Must be invunrable
		return false;

	}

	//These two functions are used to create guardians signiture particles
	void				createSmokeParticle(int width, int height){
		sf::Vector2f startPos = sprite.getPosition();
		startPos.x += (rand() % (width * 2) + 1) - width;
		startPos.y += (rand() % (height * 2) + 1) - height;
		smoke_mech.angle = rand() % 360;
		new ps::Particle(startPos, smoke_vis[rand() % 2], smoke_sp[rand() % 3], smoke_mech);
	}
	void				createMistParticle(int width, int height){
		sf::Vector2f startPos = sprite.getPosition();
		startPos.x += (rand() % (width * 2) + 1) - width;
		startPos.y += (rand() % (height * 2) + 1) - height;
		mist_mech.angle = rand() % 360;
		new ps::Particle(startPos, mist_vis, mist_mech);
	}

private:
	const Real_Coords	spawnPoint;	//The point it will spawn

	bool				breakWalls; //Can the guardian break through walls

	std::vector
		<Real_Coords>	patrolPath;	//The path on the map
	size_t				patrolPosition = 0;//The position on their patrol path they are

	std::vector
		<Tile*>			currentPath;	//The current path the guardian is on
	size_t				currentPathPointer = 0;//The target on this path the guardian is after
	suint				reloadPathTick = 0;  //How long until the path needs to be reloaded

	//Depending on the mode, gets a path to the target
	void getNewPath(Tile_Map& map,Player& player){
		switch (currentMode){
		case Guardian_Mode::gm_returning:
			currentMode = Guardian_Mode::gm_patrolling;
			//reloadPathTick = 0;//Make sure to get your path later
			patrolPosition = 0;//Will be incremented by 1, and at 0, so this works gud
			//no break, so that it also triggers gm_patrolling
		case Guardian_Mode::gm_patrolling:

			patrolPosition += 1;
			if (patrolPosition == patrolPath.size()){
				patrolPosition = 0;
			}
			reloadPathTick = 10000;

			currentPath = map.getPath(sprite.getPosition(), patrolPath[patrolPosition], breakWalls);
			break;
		case Guardian_Mode::gm_chasing_blind:
			if (currentPathPointer == currentPath.size()){
				//Lost sight of the player, can't find them, got to return to patroling
				currentMode = gm_returning;
				patrolPosition = 0;
				//suitably long amount of time so that it won't need to reload
				reloadPathTick = 10000;
				currentPath = map.getPath(sprite.getPosition(), patrolPath[0],breakWalls);
			}
			else{
				reloadPathTick = 10000;
				return;//prevent the path pointer from being reset
			}
			break;
		//If they are chasing the player, get a path to the player
		case Guardian_Mode::gm_chasing_sight:
			currentPath = map.getPath(sprite.getPosition(), player.getPosition(),breakWalls);
			reloadPathTick = 10;
			break;

		default:
			//Stone, attacking, circling
			//None of these should have triggered getPath, so this is an error
			//Therefore, throw an error
			throw('g');
		}

		currentPathPointer = 0;
	}
};

//Include all child classes here
#include "Seeker.h"
#include "Spectre.h"
#include "Stalker.h"

#endif