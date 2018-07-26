#include "HealthPickup.h"

/// <summary>
/// @author Paul Nolan
/// @version 1.0
/// </summary>

HealthPickup::HealthPickup()
{
	if (m_healthTexture.loadFromFile("resources/images/healthPack.png"))
	{

	}
	m_healthSprite.setTexture(m_healthTexture);

	//These are the time limits for the spawn and duration of the health pack
	m_activeIntervel = 45000;
	m_spawnIntervel = 45000;

	m_healthValue = 2;

	m_active = false;

	m_range = 200;

	srand(time(NULL));
}


HealthPickup::~HealthPickup()
{
}


void HealthPickup::update(double dt, sf::Sprite playerSprite, int &playerHealth, sf::Sprite aiSprite, int &aiHealth,std::vector<sf::Sprite> & wallSprites)
{
	m_timer += dt;

	//The below lines of code are used to determine if the ai or player are in range of the health pack.

	m_relativePlayerPosition = m_position - playerSprite.getPosition();
	m_playerDistance = sqrt(m_relativePlayerPosition.x * m_relativePlayerPosition.x + m_relativePlayerPosition.y * m_relativePlayerPosition.y);

	m_relativeAiPosition = m_position - aiSprite.getPosition();
	m_aiDistance = sqrt(m_relativeAiPosition.x * m_relativeAiPosition.x + m_relativeAiPosition.y * m_relativeAiPosition.y);

	if (m_playerDistance < m_range)
	{
		m_inRange = true;
	}
	else if (m_aiDistance < m_range)
	{
		m_inRange = true;
	}
	else
	{
		m_inRange = false;
	}

	//This if situation is used for when either tank collides with the pack and applies the health to them
	if (m_active == true)
	{
		if (CollisionDetector::collision(m_healthSprite, playerSprite))
		{
			playerHealth += m_healthValue;
			m_active = false;
			m_timer = 0;
		}

		if (CollisionDetector::collision(m_healthSprite, aiSprite))
		{
			aiHealth += m_healthValue;
			m_active = false;
			m_timer = 0;
		}

		if (m_timer >= m_activeIntervel)
		{
			m_active = false;
			m_timer = 0;
		}
	}
	//This if situation is used to spawn the health pack
	else if (m_active == false)
	{
		if (m_timer >= m_spawnIntervel)
		{
			spawn(wallSprites);
			m_active = true;
			m_timer = 0;
		}
	}
}

void HealthPickup::render(sf::RenderWindow & window)
{
	if (m_active == true && m_inRange == true)
	{
		window.draw(m_healthSprite);
	}
}

void HealthPickup::spawn(std::vector<sf::Sprite> & wallSprites)
{
	m_position = sf::Vector2f(-1, -1);

	//This while loop is used to ensure the health doesnt intersect with the wall pieces
	while (m_position == sf::Vector2f(-1, -1))
	{
	m_position = sf::Vector2f(rand() % m_widthRange + 1, rand() % m_heightRange + 1);
	m_healthSprite.setPosition(m_position);

	//This for loop is used to check for collsions between the pack and the walls
	for (int i = 0; i < wallSprites.size(); i++)
	{
		if (CollisionDetector::collision(m_healthSprite, wallSprites[i]))
		{
			m_position = sf::Vector2f(-1, -1);
		}
	}
    }
}