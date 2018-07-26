#pragma once
#include "CollisionDetector.h"
#include <SFML/Graphics.hpp>
#include <Thor/Vectors.hpp>
#include <iostream>
#include <queue>
#include <vector>
#include <stdio.h>  
#include <stdlib.h> 
#include <time.h> 

/// <summary>
/// @author Paul Nolan
/// @version 1.0
/// </summary>

class HealthPickup
{
public:
	HealthPickup();
	~HealthPickup();

	void update(double dt, sf::Sprite playerSprite, int &playerHealth, sf::Sprite aiSprite, int &aiHealth, std::vector<sf::Sprite> & wallSprites);

	void render(sf::RenderWindow & window);

	void spawn(std::vector<sf::Sprite> & wallSprites);

	enum class healthState
	{
		NOT_ACTIVE,
		ACTIVE
	}m_healthState;

	bool m_active;

	bool m_inRange;

	int m_widthRange;
	int m_heightRange;

	sf::Vector2f m_position;

private:
	sf::Texture m_healthTexture;
	sf::Sprite m_healthSprite;

	sf::Vector2f m_relativePlayerPosition;
	sf::Vector2f m_relativeAiPosition;

	float m_playerDistance;
	float m_aiDistance;
	float m_range;

	int m_healthValue;

	double m_timer;
	double m_spawnIntervel;
	double m_activeIntervel;
};

