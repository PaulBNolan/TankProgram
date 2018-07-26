#pragma once
#include <SFML/Graphics.hpp>
#include "KeyHandler.h"
#include "CollisionDetector.h"
#include "ProjectilePool.h"
#include <iostream>
/// <summary>
/// @brief A simple tank controller.
/// 
/// This class will manage all tank movement and rotations.
/// </summary>
class Tank
{
public:	
	Tank(sf::Texture const & texture, std::vector<sf::Sprite> & wallSprites, KeyHandler const & object);
	void update(double dt, sf::Sprite & tankAi, bool & AiHit);
	void render(sf::RenderWindow & window);
	void setPosition(sf::Vector2f const & pos);
	void handleKeyInput();

	/// <summary>
	/// @brief Increases the speed by 1, max speed is capped at 100.
	/// 
	/// </summary>
	void increaseSpeed();

	/// <summary>
	/// @brief Decreases the speed by 1, min speed is capped at -100.
	/// 
	/// </summary>
	void decreaseSpeed();

	/// <summary>
	/// @brief Increases the rotation by 1 degree, wraps to 0 degrees after 359.
	/// 
	/// </summary>
	void increaseRotation();
	void increaseTurretRotation();
	/// <summary>
	/// @brief Decreases the rotation by 1 degree, wraps to 359 degrees after 0.
	/// 
	/// </summary>
	void decreaseRotation();
	void decreaseTurretRotation();

	/// <summary>
	/// @brief Checks for collisions between the tank and the walls.
	/// 
	/// </summary>
	/// <returns>True if collision detected between tank and wall.</returns>
	bool checkWallCollision();

	/// <summary>
	/// @brief Stops the tank if moving and applies a small increase in speed in the opposite direction of travel.
	/// If the tank speed is currently 0, the rotation is set to a value that is less than the previous rotation value
	///  (scenario: tank is stopped and rotates into a wall, so it gets rotated towards the opposite direction).
	/// If the tank is moving, further rotations are disabled and the previous tank position is restored.
	/// The tank speed is adjusted so that it will travel slowly in the opposite direction. The tank rotation 
	///  is also adjusted as above if necessary (scenario: tank is both moving and rotating, upon wall collision it's 
	///  speed is reversed but with a smaller magnitude, while it is rotated in the opposite direction of it's 
	///  pre-collision rotation).
	/// </summary>
	void deflect();

	void adjustRotation();

	/// <summary>
	/// @brief Generates a request to fire a projectile.
	/// A projectile will be fired from the tip of the turret only if time limit between shots
	///  has expired. Fire requests are not queued.
	/// </summary>
	void requestFire();

	void convertToRadians();
	void convertToDegrees();

	void takeDamage();

	/// <summary>
	/// @brief Reads the player tank position.
	/// <returns>The tank base position.</returns>
	/// </summary>
	sf::Vector2f getPosition() const;

	sf::Sprite getTurret() const;
	sf::Sprite getBase() const;
	
	// The tank speed.
	double m_speed{ 0.0 };


	// The current rotation as applied to tank base.
	double m_rotation{ 0.0 };
	double m_turretRotation{ 0.0 };

	bool m_hit = false;
	int m_health = 5;

private:
	void initSprites(sf::Vector2f const & pos);

	sf::Sprite m_tankBase;
	sf::Sprite m_turret;
	std::vector<sf::Sprite> & m_wallSprites;

	bool m_enableRotation = true;
	sf::Vector2f m_previousPosition;
	double m_previousSpeed;
	double m_previousRotation = 0;
	double m_previousTurretRotation = 0;

	sf::Vector2f newPosition = { 0,0 };
	sf::Texture const & m_texture;
	KeyHandler const & m_keyHandler;
	ProjectilePool m_pool;

	// The delay time between shots in milliseconds.
	static int const s_TIME_BETWEEN_SHOTS{ 800 };
	bool m_fireRequested;
	int m_shootTimer = 800;
};
