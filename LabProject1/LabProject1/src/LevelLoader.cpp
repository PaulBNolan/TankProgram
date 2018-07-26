#include "LevelLoader.h"

void operator >> (const YAML::Node& InvaderNode, EnemyData& Invaders)
{
	Invaders.m_type = InvaderNode["Type"].as < std::string>();
	Invaders.m_row = InvaderNode["Row"].as<int>();
	Invaders.m_column = InvaderNode["Column"].as<int>();
	Invaders.m_position.x = InvaderNode["position"]["x"].as<float>();
	Invaders.m_position.y = InvaderNode["position"]["y"].as<float>();
}

void operator >> (const YAML::Node& BrickNode, BrickData& Bricks)
{
	Bricks.m_type = BrickNode["Type"].as < std::string>();
	Bricks.m_row = BrickNode["Row"].as<int>();
	Bricks.m_column = BrickNode["Column"].as<int>();
	Bricks.m_position.x = BrickNode["position"]["x"].as<float>();
	Bricks.m_position.y = BrickNode["position"]["y"].as<float>();
}

void operator >> (const YAML::Node& paddleNode, PaddleData& paddle)
{
	paddle.m_position.x = paddleNode["position"]["x"].as<float>();
	paddle.m_position.y = paddleNode["position"]["y"].as<float>();
}

void operator >> (const YAML::Node& boltNode, BoltData& Bolt)
{
	Bolt.m_position.x = boltNode["position"]["x"].as<float>();
	Bolt.m_position.y = boltNode["position"]["y"].as<float>();
	Bolt.m_velocity.x = boltNode["velocity"]["x"].as<float>();
	Bolt.m_velocity.y = boltNode["velocity"]["y"].as<float>();
}

void operator >> (const YAML::Node& timeNode, TimeData& Time)
{
	Time.time = timeNode["TimeLimit"].as<int>();
}

void operator >> (const YAML::Node& levelNode, LevelData& level)
{

	levelNode["Paddle"] >> level.m_paddle;

	levelNode["Bolt"] >> level.m_bolt;

	levelNode["Time"] >> level.m_time;
	const YAML::Node& BricksNode = levelNode["bricks"].as<YAML::Node>();

	for (unsigned i = 0; i < BricksNode.size(); ++i)
	{
		BrickData brick;
		BricksNode[i] >> brick;
		level.m_bricks.push_back(brick);
	}

	const YAML::Node& InvadersNode = levelNode["Invaders"].as<YAML::Node>();

	for (unsigned i = 0; i < InvadersNode.size(); ++i)
	{
		std::cout << i << std::endl;
		EnemyData Invader;
		InvadersNode[i] >> Invader;
		level.m_invaders.push_back(Invader);
	}
}

bool LevelLoader::load(int nr, LevelData& level)
{
	std::stringstream ss;
	ss << "level";
	ss << nr;
	ss << ".yaml";
	
	try
	{
		YAML::Node baseNode = YAML::LoadFile("ASSETS/levels/" + ss.str());
		if (baseNode.IsNull())
		{
			std::string message("file: " + ss.str() + " not found");
			throw std::exception(message.c_str());
		}
		baseNode >> level;
	}

	catch (YAML::ParserException& e)
	{
		std::cout << e.what() << "\n";
		return false;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << "\n";
		return false;
	}

	return true;
}