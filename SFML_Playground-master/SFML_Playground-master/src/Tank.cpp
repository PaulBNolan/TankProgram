#include "Tank.h"

/// <summary>
/// @author Paul Nolan
/// @version 1.0
/// </summary>

double PI = 3.14159265359;
Tank::Tank(sf::Texture const & texture, std::vector<sf::Sprite> & wallSprites, KeyHandler const & object)
	: m_texture(texture)
	, m_wallSprites(wallSprites)
  ,m_keyHandler(object)
{
}

void Tank::update(double dt, sf::Sprite & TankAi, bool & AiHit)
{	 
	handleKeyInput();

	if (m_rotation != 0)
	{
		convertToRadians();
	}

	//The tanks new position is calculated and set

	newPosition.x = newPosition.x + cos(m_rotation) * m_speed * (dt / 1000);

	newPosition.y = newPosition.y + sin(m_rotation) * m_speed * (dt / 1000);

	m_tankBase.setPosition(newPosition);
	m_turret.setPosition(newPosition);

	if (m_rotation != 0)
	{
		convertToDegrees();
	}

	//The rotatons are taken into account

	m_tankBase.setRotation(m_rotation);
	m_turret.setRotation(m_turretRotation);

	//This situation checks to see if the player has collided with a wall
	if (checkWallCollision())
	{
		deflect();
	}
	//This situation helps to shoot the bullet ie projectile
	if (m_fireRequested == true)
	{
		m_shootTimer -= dt;
		if (m_shootTimer < 0)
		{
			m_shootTimer = s_TIME_BETWEEN_SHOTS;
			m_fireRequested = false;
		}
	}

	//This situation inflicts damage onto the player when they collide with the ais projectiles
	if (m_hit == true)
	{
		takeDamage();
	}

	//The projectile pool object updates itself here using factors from the ai tank
	m_pool.update(dt, m_wallSprites, TankAi, AiHit);
}

void Tank::setPosition(sf::Vector2f const & pos)
{
	initSprites(pos);
}

void Tank::render(sf::RenderWindow & window) 
{
	window.draw(m_tankBase);
	window.draw(m_turret);
	if (m_fireRequested == true)
	{
		m_pool.render(window);
	}
}


void Tank::initSprites(sf::Vector2f const & pos)
{
	// Initialise the tank base
	sf::IntRect baseRect(2, 43, 79, 43);
	m_tankBase.setTexture(m_texture);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);
	m_tankBase.setPosition(pos);
	// Initialise the turret

	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(19, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);
	m_turret.setPosition(pos);

	newPosition = pos;
}

////////////////////////////////////////////////////////////
void Tank::increaseSpeed()
{
	if (m_speed < 100.0)
	{
		m_speed += 1;
	}
}

////////////////////////////////////////////////////////////
void Tank::decreaseSpeed()
{
	if (m_speed > -100.0)
	{
		m_speed -= 1;
	}
}

////////////////////////////////////////////////////////////
void Tank::increaseRotation()
{
	m_rotation += 1;
	if (m_rotation == 360.0)
	{
		m_rotation = 0;
	}
}

void Tank::increaseTurretRotation()
{
	m_turretRotation += 1;
	if (m_turretRotation == 360.0)
	{
		m_turretRotation = 0;
	}
}

////////////////////////////////////////////////////////////
void Tank::decreaseRotation()
{
	m_rotation -= 1;
	if (m_rotation == 0.0)
	{
		m_rotation = 359.0;
	}
}

void Tank::decreaseTurretRotation()
{
	m_turretRotation -= 1;
	if (m_turretRotation == 0.0)
	{
		m_turretRotation = 359.0;
	}
}

void Tank::convertToDegrees()
{
	m_rotation = m_rotation * (180 / PI);
}

void Tank::convertToRadians()
{
	m_rotation = m_rotation * (PI / 180);
}

void Tank::handleKeyInput()
{
	if (m_keyHandler.isPressed(sf::Keyboard::Up))
	{
		m_previousSpeed = m_speed; // NEW
		increaseSpeed();
	}
	if (m_keyHandler.isPressed(sf::Keyboard::Down))
	{
		m_previousSpeed = m_speed; // NEW
		decreaseSpeed();
	}

	if (m_enableRotation == true) // NEW
	{
		if (m_keyHandler.isPressed(sf::Keyboard::Right))
		{
			m_previousRotation = m_rotation; // NEW
			increaseRotation();

			m_previousTurretRotation = m_turretRotation;
			increaseTurretRotation();
		}
		if (m_keyHandler.isPressed(sf::Keyboard::Left))
		{
			m_previousRotation = m_rotation; // NEW
			decreaseRotation();

			m_previousTurretRotation = m_turretRotation;
			decreaseTurretRotation();
		}

		if (m_keyHandler.isPressed(sf::Keyboard::Z) == true)
		{
			m_previousTurretRotation = m_turretRotation;
			increaseTurretRotation();
		}
		if (m_keyHandler.isPressed(sf::Keyboard::X) == true)
		{
			m_previousTurretRotation = m_turretRotation;
			decreaseTurretRotation();
		}
	}

	if (m_keyHandler.isPressed(sf::Keyboard::Space) == true)
	{
		requestFire();
	}

}

////////////////////////////////////////////////////////////
bool Tank::checkWallCollision()
{
	for (sf::Sprite const & sprite : m_wallSprites)
	{
		// Checks if either the tank base or turret has collided with the current wall sprite.
		if (CollisionDetector::collision(m_turret, sprite) ||
			CollisionDetector::collision(m_tankBase, sprite))
		{
			return true;
		}
	}
	m_enableRotation = true;
	return false;
}

void Tank::deflect()
{
	// In case tank was rotating.
	adjustRotation();

	// If tank was moving.
	if (m_speed != 0)
	{
		// Temporarily disable turret rotations on collision.
		m_enableRotation = false;
		// Back up to position in previous frame.
		m_tankBase.setPosition(m_previousPosition);
		// Apply small force in opposite direction of travel.
		if (m_previousSpeed < 0)
		{
			m_speed = 8;
		}
		else
		{
			m_speed = -8;
		}
	}
}

void Tank::adjustRotation()
{
	// If tank was rotating...
	if (m_rotation != m_previousRotation)
	{
		// Work out which direction to rotate the tank base post-collision.
		if (m_rotation > m_previousRotation)
		{
			m_rotation = m_previousRotation - 1;
		}
		else
		{
			m_rotation = m_previousRotation + 1;
		}
	}
	// If turret was rotating while tank was moving
	if (m_turretRotation != m_previousTurretRotation)
	{
		// Set the turret rotation back to it's pre-collision value.
		m_turretRotation = m_previousTurretRotation;
	}
}


////////////////////////////////////////////////////////////
void Tank::requestFire()
{
	m_fireRequested = true;
	if (m_shootTimer == s_TIME_BETWEEN_SHOTS)
	{
		sf::Vector2f tipOfTurret(m_turret.getPosition().x + 2.0f, m_turret.getPosition().y);
		tipOfTurret.x += std::cos(MathUtility::DEG_TO_RAD  * m_turret.getRotation()) * ((m_turret.getLocalBounds().top + m_turret.getLocalBounds().height) * 1.7f);
		tipOfTurret.y += std::sin(MathUtility::DEG_TO_RAD  * m_turret.getRotation()) * ((m_turret.getLocalBounds().top + m_turret.getLocalBounds().height) * 1.7f);
		m_pool.create(m_texture, tipOfTurret.x, tipOfTurret.y, m_turret.getRotation());
	}
}

////////////////////////////////////////////////////////////
sf::Vector2f Tank::getPosition() const
{
	return m_tankBase.getPosition();
}


/////////////////////////////////////////////////////////////////////////////////////
sf::Sprite Tank::getBase() const
{
	return m_tankBase;
}

/////////////////////////////////////////////////////////////////////////////////////
sf::Sprite Tank::getTurret() const
{
	return m_turret;
}

//This function reduces the players health when they are hit by a ai bullet
void Tank::takeDamage()
{
	m_hit = false;
	m_health -= 1;
}