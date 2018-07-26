#pragma once
#include "MathUtility.h"
#include "Tank.h"
#include "CollisionDetector.h"
#include <SFML/Graphics.hpp>
#include <Thor/Vectors.hpp>
#include <iostream>
#include <queue>
#include <vector>
#include "ProjectilePool.h"
#include "HealthPickup.h"
#include "LevelLoader.h"
#include "GameState.h"

/// <summary>
/// @author Paul Nolan
/// @version 1.0
/// </summary>

class TankAi
{
public:
	/// <summary>
	/// @brief Constructor that stores a reference to the obstacle container.
	/// Initialises steering behaviour to seek (player) mode, sets the AI tank position and
	///  initialises the steering vector to (0,0) meaning zero force magnitude.
	/// </summary>
	/// <param name="texture">A reference to the sprite sheet texture</param>
	///< param name="wallSprites">A reference to the container of wall sprites</param>
	TankAi(sf::Texture const & texture, std::vector<sf::Sprite> & wallSprites);

	/// <summary>
	/// @brief Steers the AI tank towards the player tank avoiding obstacles along the way.
	/// Gets a vector to the player tank and sets steering and velocity vectors towards
	/// the player if current behaviour is seek. If behaviour is stop, the velocity vector
	/// is set to 0. Then compute the correct rotation angle to point towards the player tank. 
	/// If the distance to the player tank is < MAX_SEE_AHEAD, then the behaviour is changed from seek to stop.
	/// Finally, recalculate the new position of the tank base and turret sprites.
	/// </summary>
	/// <param name="playerTank">A reference to the player tank</param>
	/// <param name="dt">update delta time</param>
	void update(Tank &playerTank,HealthPickup & healthPack,double dt);

	/// <summary>
	/// @brief Draws the tank base and turret.
	///
	/// </summary>
	/// <param name="window">The SFML Render window</param>
	void render(sf::RenderWindow & window);

	/// <summary>
	/// @brief Initialises the obstacle container and sets the tank base/turret sprites to the specified position.
	/// <param name="position">An x,y position</param>
	/// </summary>
	void init(sf::Vector2f position);

	/// <summary>
	/// This function initialises the safe and search positions for the ai when their health is below 2
	/// </summary>
	/// <param name="searchLocations"></param>
	/// <param name="safestPosition"></param>
	void initPositions(std::vector<SearchData> searchLocations, std::vector<SafestData> safestPosition);

	/// <summary>
	/// @brief Checks for collision between the AI and player tanks.
	///
	/// </summary>
	/// <param name="player">The player tank instance</param>
	/// <returns>True if collision detected between AI and player tanks.</returns>
	bool collidesWithPlayer(Tank const & playerTank) const;

	sf::Sprite getSprite();

	void setPositon(sf::Vector2f position);


	//This bool is used to register hits the ai receives from the player
	bool m_hit;
	//This determines if the ai is alive
	bool m_active;
	//This represents the ais health
	int m_health;

	//This objects represents the ai bullets
	ProjectilePool m_aiPool;

	//This enum represents the states of the ais behaviour
	AiBehaviour m_aiBehaviour;

	enum class AiType
	{
		AI_ID_NONE,
		AI_ID_SEEK_SHOOT_AT_PLAYER
	};

private:
	void initSprites();

	//This function helps the ai move 
	void updateMovement(double dt);

	//This function is used to determine the direction the ai moves in
	sf::Vector2f seek(sf::Vector2f playerPosition, HealthPickup healthPack) const;

	//This function prevents the ai from colliding with the wall
	sf::Vector2f collisionAvoidance();

	//This function is used to find the shape the ai should avoid the most in their current situation
	const sf::CircleShape findMostThreateningObstacle();

	//This functions helps the ai fire its bullet
	void requestFire();

	//This function determines the state of the ai
	void determineState(bool healthActive,sf::Vector2f playerPosition);

	//This function finds the furthest safe location from the player
	sf::Vector2f findSafestPosition(sf::Vector2f playerPosition);

	//This function takes damage into account
	void takeDamage();

	//These values are used to time the launching of the projectiles
	double m_timer = 0;
	double m_increment = 0.1;
	double m_interval = 20;

	// A reference to the sprite sheet texture.
	sf::Texture const & m_texture;

	// A sprite for the tank base.
	sf::Sprite m_tankBase;

	// A sprite for the turret
	sf::Sprite m_turret;

	// A reference to the container of wall sprites.
	std::vector<sf::Sprite> & m_wallSprites;

	// The current rotation as applied to tank base and turret.
	double m_rotation{ 0.0 };
	double m_turretRotation{ 0.0 };

	sf::Vector2f m_turretDirection;

	// Current velocity.
	sf::Vector2f m_velocity;

	// Steering vector.
	sf::Vector2f m_steering;

	// The ahead vector.
	sf::Vector2f m_ahead;

	// The half-ahead vector.
	sf::Vector2f m_halfAhead;

	// The maximum see ahead range.
	static float constexpr MAX_SEE_AHEAD{ 50.0f };

	// The maximum avoidance turn rate.
	static float constexpr MAX_AVOID_FORCE{ 50.0f };

	// 
	static float constexpr MAX_FORCE{ 10.0f };

	// The maximum speed for this tank.
	float MAX_SPEED = 50.0f;

	sf::Vector2f m_difference;

	float m_maxDifference = 200;

	//This value is used to help the ai to stop rotating when it has stopped
	sf::Vector2f m_previousDirection;

	//The below values are used to represent the safe spots the player will
	//go to when damaged enough and the locations it will search for the 
	//health pack.
	//The ints are used to represent the current one used.

	std::vector<sf::Vector2f> m_safestPositions;
	int m_currentSafestTarget;

	std::vector<sf::Vector2f> m_searchCoordinates;
	int m_currentSearchTarget;

	// A container of circles that represent the obstacles to avoid.
	std::vector<sf::CircleShape> m_obstacles;

	//This bool is used to stop the the tank
	bool m_stop = false;

	bool m_fireRequested = false;
};

