#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

struct ObstacleData
{
	std::string m_type;
	sf::Vector2f m_position;
	double m_rotation;
};


struct BrickData
{
	std::string m_fileName;
};


struct PaddleData
{
	sf::Vector2f m_position;
};


struct LevelData
{
	BrickData m_brick;
	PaddleData m_paddle;
	std::vector<ObstacleData> m_obstacles;
};

class LevelLoader
{
public:
	LevelLoader();
	~LevelLoader();
};

