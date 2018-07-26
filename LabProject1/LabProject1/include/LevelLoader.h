/// <summary>
/// @Author: Paul Nolan
/// @Version 2.0
/// </summary>

#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "yaml-cpp\yaml.h"

struct EnemyData
{
	std::string m_type;
	int m_column;
	int m_row;
	sf::Vector2f m_position;
};
struct PaddleData
{
	sf::Vector2f m_position;
};
struct BrickData
{
	std::string m_type;
	int m_column;
	int m_row;
	sf::Vector2f m_position;
};
struct BoltData
{
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
};
struct TimeData
{
	int time;
};
struct LevelData
{
	std::vector<BrickData> m_bricks;
	std::vector<EnemyData> m_invaders;
	PaddleData m_paddle;
	BoltData m_bolt;
	TimeData m_time;
};

class LevelLoader
{
public:
	LevelLoader() = default;

	static bool load(int nr, LevelData& level);
};

