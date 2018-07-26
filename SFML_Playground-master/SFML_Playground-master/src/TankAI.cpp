#include "TankAi.h"

/// <summary>
/// @author Paul Nolan
/// @version 1.0
/// </summary>

////////////////////////////////////////////////////////////
TankAi::TankAi(sf::Texture const & texture, std::vector<sf::Sprite> & wallSprites)
	: m_aiBehaviour(AiBehaviour::SEEK_PLAYER)
	, m_texture(texture)
	, m_wallSprites(wallSprites)
	, m_steering(0, 0)
{
	// Initialises the tank base and turret sprites.
	initSprites();

	m_hit = false;

	m_active = true;

	m_health = 5;

	m_timer = m_interval;
}

sf::Sprite TankAi::getSprite()
{
	return m_tankBase;
}


////////////////////////////////////////////////////////////
void TankAi::update(Tank  &playerTank, HealthPickup & healthPack, double dt)
{
	//The current state of the ai is determined at the start of each loop
	determineState(healthPack.m_active, playerTank.getPosition());

	sf::Vector2f vectorToPlayer = seek(playerTank.getPosition(), healthPack);

	//This value is used to find the distance between the ai and their current target
	float distance = sqrt(vectorToPlayer.x * vectorToPlayer.x + vectorToPlayer.y * vectorToPlayer.y);

	switch (m_aiBehaviour)
	{
	case AiBehaviour::SEEK_PLAYER:
		m_steering += thor::unitVector(vectorToPlayer);
		m_steering += collisionAvoidance();
		m_steering = MathUtility::truncate(m_steering, MAX_FORCE);

		//This line is used to give the tank turret independent rotation from the main body
		m_turretDirection = MathUtility::truncate(m_velocity + m_steering, MAX_SPEED);

		if (distance >= m_maxDifference)
		{
			m_velocity = MathUtility::truncate(m_velocity + m_steering, MAX_SPEED);
			m_previousDirection = m_velocity;
			m_stop = false;
		}

		else if (distance <= m_maxDifference)
		{
			m_stop = true;
			m_velocity = sf::Vector2f(0, 0);
			//This if situation is used to time bullet shots
			if (m_timer >= dt * 2)
			{
				requestFire();
				m_timer = 0;
			}
		}

		m_timer += m_increment;
		break;
		//This case is used to find the health oack
	case AiBehaviour::FIND_HEALTH:
		m_steering += thor::unitVector(vectorToPlayer);
		m_steering += collisionAvoidance();
		m_steering = MathUtility::truncate(m_steering, MAX_FORCE);
		m_velocity = MathUtility::truncate(m_velocity + m_steering, MAX_SPEED);

		m_previousDirection = m_velocity;
		//This is used to tell the ai to move onto a new location after searching their current one
		if (MathUtility::distance(m_searchCoordinates[m_currentSearchTarget], m_tankBase.getPosition()) < 10)
		{
			m_currentSearchTarget += 1;

			if (m_currentSearchTarget > 15)
			{
				m_currentSearchTarget = 0;
			}
		}
		break;
		//This case is used to move the ai to a safe location while the health is not present
	case AiBehaviour::FIND_COVER:
		m_steering += thor::unitVector(vectorToPlayer);
		m_steering += collisionAvoidance();
		m_steering = MathUtility::truncate(m_steering, MAX_FORCE);
		m_velocity = MathUtility::truncate(m_velocity + m_steering, MAX_SPEED);

		m_previousDirection = m_velocity;

		if (MathUtility::distance(m_safestPositions[m_currentSafestTarget], m_tankBase.getPosition()) < 2)
		{
			m_velocity = sf::Vector2f(0, 0);
		}
		break;
	default:
		break;
	}

	// Now we need to convert our velocity vector into a rotation angle between 0 and 359 degrees.
	// The m_velocity vector works like this: vector(1,0) is 0 degrees, while vector(0, 1) is 90 degrees.
	// So for example, 223 degrees would be a clockwise offset from 0 degrees (i.e. along x axis).
	// Note: we add 180 degrees below to convert the final angle into a range 0 to 359 instead of -PI to +PI
	auto dest = atan2(-1 * m_previousDirection.y, -1 * m_previousDirection.x) / thor::Pi * 180 + 180;

	auto turretDest = atan2(-1 * m_turretDirection.y, -1 * m_turretDirection.x) / thor::Pi * 180 + 180;

	auto currentRotation = m_rotation;
	auto currentTurretRotation = m_turretRotation;

	// Find the shortest way to rotate towards the player (clockwise or anti-clockwise)
	if (m_stop == false)
	{
		if (std::round(currentRotation - dest) == 0.0)
		{
			m_steering.x = 0;
			m_steering.y = 0;
		}

		else if ((static_cast<int>(std::round(dest - currentRotation + 360))) % 360 < 180)
		{
			// rotate clockwise
			m_rotation = static_cast<int>((m_rotation)+1) % 360;
		}
		else
		{
			// rotate anti-clockwise
			m_rotation = static_cast<int>((m_rotation)-1) % 360;
		}
	}

	if (std::round(currentTurretRotation - turretDest) == 0.0)
	{

	}

    else if ((static_cast<int>(std::round(turretDest - currentTurretRotation + 360))) % 360 < 180)
	{
		// rotate clockwise
		m_turretRotation = static_cast<int>((m_turretRotation)+1) % 360;
	}
	else
	{
		// rotate anti-clockwise
		m_turretRotation = static_cast<int>((m_turretRotation)-1) % 360;
	}

	m_aiPool.update(dt, m_wallSprites, playerTank.getBase(), playerTank.m_hit);

	updateMovement(dt);

	//This situation is used to take into account damage inflicted by the player and to find the current safest location
	if (m_hit == true)
	{
		takeDamage();
		findSafestPosition(playerTank.getPosition());
	}

}

////////////////////////////////////////////////////////////
void TankAi::render(sf::RenderWindow & window)
{
	// TODO: Don't draw if off-screen...
	
	m_aiPool.render(window);

	window.draw(m_tankBase);
	window.draw(m_turret);

}

////////////////////////////////////////////////////////////
void TankAi::init(sf::Vector2f position)
{
	m_tankBase.setPosition(position);
	m_turret.setPosition(position);

	for (sf::Sprite const wallSprite : m_wallSprites)
	{
		sf::CircleShape circle(wallSprite.getTextureRect().width * 1.5f);
		circle.setOrigin(circle.getRadius(), circle.getRadius());
		circle.setPosition(wallSprite.getPosition());
		m_obstacles.push_back(circle);
	}
}

////////////////////////////////////////////////////////////
sf::Vector2f TankAi::seek(sf::Vector2f playerPosition, HealthPickup healthPack) const
{
	//The below lines are all used to determine the direction the player travels in

	if (m_aiBehaviour == AiBehaviour::SEEK_PLAYER)
	{
		return sf::Vector2f(playerPosition - m_tankBase.getPosition());
	}

	else if (m_aiBehaviour == AiBehaviour::FIND_HEALTH)
	{
		if (healthPack.m_inRange == true)
		{
			return sf::Vector2f(healthPack.m_position - m_tankBase.getPosition());
		}
		else if (healthPack.m_inRange == false)
		{
			return sf::Vector2f(m_searchCoordinates[m_currentSearchTarget] - m_tankBase.getPosition());
		}
	}

	else if (m_aiBehaviour == AiBehaviour::FIND_COVER)
	{
		return sf::Vector2f(m_safestPositions[m_currentSafestTarget] - m_tankBase.getPosition());
	}
}

////////////////////////////////////////////////////////////
sf::Vector2f TankAi::collisionAvoidance()
{
	auto headingRadians = thor::toRadian(m_rotation);
	sf::Vector2f headingVector(std::cos(headingRadians) * MAX_SEE_AHEAD, std::sin(headingRadians) * MAX_SEE_AHEAD);
	m_ahead = m_tankBase.getPosition() + headingVector;
	m_halfAhead = m_tankBase.getPosition() + (headingVector * 0.5f);
	const sf::CircleShape mostThreatening = findMostThreateningObstacle();
	sf::Vector2f avoidance(0, 0);
	if (mostThreatening.getRadius() != 0.0)
	{		
		avoidance.x = m_ahead.x - mostThreatening.getPosition().x;
		avoidance.y = m_ahead.y - mostThreatening.getPosition().y;
		avoidance = thor::unitVector(avoidance);
		avoidance *= MAX_AVOID_FORCE;
	}
	else
	{
		avoidance *= 0.0f;
	}
	return avoidance;
}

////////////////////////////////////////////////////////////
const sf::CircleShape TankAi::findMostThreateningObstacle()
{
	sf::CircleShape mostThreatening;
	for (int index = 0; index < m_obstacles.size(); index++)
	{
		if (MathUtility::lineIntersectsCircle(m_ahead, m_halfAhead, m_obstacles[index]) == true && 
			MathUtility::distance(m_tankBase.getPosition(), m_obstacles[index].getPosition()) < MathUtility::distance(m_tankBase.getPosition(), mostThreatening.getPosition()))
		{
			mostThreatening = m_obstacles.at(index);
		}
	}
	// The initialisation of mostThreatening is just a placeholder...
	//mostThreatening = m_obstacles.at(0);

	return mostThreatening;
}

////////////////////////////////////////////////////////////
void TankAi::initSprites()
{
	// Initialise the tank base
	m_tankBase.setTexture(m_texture);
	sf::IntRect baseRect(103, 43, 79, 43);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);

	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(122, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);
}

////////////////////////////////////////////////////////////
void TankAi::updateMovement(double dt)
{
	//The below value is used to increase the ais movement when looking for cover or health
	int multi;
	if (m_aiBehaviour == AiBehaviour::FIND_COVER || m_aiBehaviour == AiBehaviour::FIND_HEALTH)
	{
		multi = 2;
	}
	else
	{
		multi = 1;
	}

	double speed = thor::length(m_velocity);
	sf::Vector2f newPos(m_tankBase.getPosition().x + std::cos(MathUtility::DEG_TO_RAD  * m_rotation) * (speed * multi) * (dt / 1000),
		m_tankBase.getPosition().y + std::sin(MathUtility::DEG_TO_RAD  * m_rotation) * (speed * multi) * (dt / 1000));
	m_tankBase.setPosition(newPos.x, newPos.y);
	m_tankBase.setRotation(m_rotation);
	m_turret.setPosition(m_tankBase.getPosition());
	m_turret.setRotation(m_turretRotation);
}

bool TankAi::collidesWithPlayer(Tank const & playerTank) const
{
	// Checks if the AI tank has collided with the player tank.
	if (CollisionDetector::collision(m_turret, playerTank.getTurret()) ||
		CollisionDetector::collision(m_tankBase, playerTank.getBase()))
	{
		return true;
	}
	return false;
}

//This function takes into account the damage and kills the ai once the health reaches zero
void TankAi::takeDamage()
{
	m_hit = false;

	m_health -= 1;

	if (m_health <= 0)
	{
		m_active = false;
	}
}

void TankAi::setPositon(sf::Vector2f position)
{
	m_tankBase.setPosition(position);
	m_turret.setPosition(position);
}

void TankAi::requestFire()
{
	//The below lines of code determine the spawn point of the ai bullets and creates them
	sf::Vector2f tipOfTurret(m_turret.getPosition().x + 2.0f, m_turret.getPosition().y);
	tipOfTurret.x += std::cos(MathUtility::DEG_TO_RAD  * m_turret.getRotation()) * ((m_turret.getLocalBounds().top + m_turret.getLocalBounds().height) * 1.7f);
	tipOfTurret.y += std::sin(MathUtility::DEG_TO_RAD  * m_turret.getRotation()) * ((m_turret.getLocalBounds().top + m_turret.getLocalBounds().height) * 1.7f);
	m_aiPool.create(m_texture, tipOfTurret.x, tipOfTurret.y, m_turret.getRotation());
}

void TankAi::determineState(bool healthActive, sf::Vector2f playerPosition)
{
	//These if situations are used to determine the ais current state
	if (m_health > 2)
	{
		m_aiBehaviour = AiBehaviour::SEEK_PLAYER;
		
	}

	//Find_Health and Find_Cover share similar conditions with the 
	//difference being the bool which determines whether the health pack
	//is active or not
	else if(m_health <= 2 && healthActive == true)
	{
		m_stop = false;
		m_aiBehaviour = AiBehaviour::FIND_HEALTH;
		
	}
	else if(m_health <= 2 && healthActive == false)
	{
		m_stop = false;
		m_aiBehaviour = AiBehaviour::FIND_COVER;
	}
}

//This function is used to find the safest location for the ai
sf::Vector2f TankAi::findSafestPosition(sf::Vector2f playerPosition)
{
	//Initital value is found
	float m_distance = MathUtility::distance(playerPosition, m_safestPositions[0]);

	//For loop checks all four locations and their distance from the player
	for (int i = 0; i < m_safestPositions.size(); i++)
	{
		if (MathUtility::distance(playerPosition, m_safestPositions[i]) >= m_distance)
		{
			m_currentSafestTarget = i;
			m_distance = MathUtility::distance(playerPosition, m_safestPositions[i]);
		}
	}

	return m_safestPositions[m_currentSafestTarget];
}

//This function is used to initialize the search and safe locations
void TankAi::initPositions(std::vector<SearchData> searchLocations, std::vector<SafestData> safestPosition)
{
	m_searchCoordinates.resize(searchLocations.size());
	for (int i = 0; i < m_searchCoordinates.size(); i++)
	{
		m_searchCoordinates[i] = searchLocations[i].m_position;
	}
	m_currentSearchTarget = rand() % (int)m_searchCoordinates.size();

	m_safestPositions.resize(safestPosition.size());
	for (int i = 0; i < m_safestPositions.size(); i++)
	{
		m_safestPositions[i] = safestPosition[i].m_position;
	}

	m_currentSafestTarget = 0;
}