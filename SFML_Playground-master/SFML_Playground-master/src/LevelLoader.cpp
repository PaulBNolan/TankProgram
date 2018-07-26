#include "LevelLoader.h"

/// <summary>
/// @author Paul Nolan
/// @version 1.0
/// </summary>

/// The various operator >> overloads below are non-member functions used to extract
///  the game data from the YAML data structure.

/// <summary>
/// @brief Extracts the obstacle type, position and rotation values.
/// 
/// </summary>
/// <param name="obstacleNode">A YAML node</param>
/// <param name="obstacle">A simple struct to store the obstacle data</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& obstacleNode, ObstacleData& obstacle)
{
	obstacle.m_type = obstacleNode["type"].as<std::string>();
	obstacle.m_position.x = obstacleNode["position"]["x"].as<float>();
	obstacle.m_position.y = obstacleNode["position"]["y"].as<float>();
	obstacle.m_rotation = obstacleNode["rotation"].as<double>();
}

/// <summary>
/// @brief Extracts the filename for the game background texture.
/// 
/// </summary>
/// <param name="backgroundNode">A YAML node</param>
/// <param name="background">A simple struct to store background related data</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& backgroundNode, BackgroundData& background)
{
	background.m_fileName = backgroundNode["file"].as<std::string>();
}

/// <summary>
/// @brief Extracts the initial screen position for the player tank.
/// 
/// </summary>
/// <param name="tankNode">A YAML node</param>
/// <param name="tank">A simple struct to store data related to the player tank</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& tankNode, TankData& tank)
{
	tank.m_position.x = tankNode["position"]["x"].as<float>();
	tank.m_position.y = tankNode["position"]["y"].as<float>();
}

void operator >> (const YAML::Node& searchNode, SearchData& search)
{
	search.m_position.x = searchNode["position"]["x"].as<float>();
	search.m_position.y = searchNode["position"]["y"].as<float>();
}

void operator >> (const YAML::Node& safestNode, SafestData& safest)
{
	safest.m_position.x = safestNode["position"]["x"].as<float>();
	safest.m_position.y = safestNode["position"]["y"].as<float>();
}

/// <summary>
/// @brief Top level function that extracts various game data from the YAML data stucture.
/// 
/// Invokes other functions to extract the background, tank and obstacle data.
//   Because there are multiple obstacles, obstacle data are stored in a vector.
/// </summary>
/// <param name="tankNode">A YAML node</param>
/// <param name="tank">A simple struct to store data related to the player tank</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& levelNode, LevelData& level)
{
	levelNode["background"] >> level.m_background;

	levelNode["tank"] >> level.m_tank;

	levelNode["ai_tank"] >> level.m_aiTank;  // ADD THIS

	const YAML::Node& obstaclesNode = levelNode["obstacles"].as<YAML::Node>();
	for (unsigned i = 0; i < obstaclesNode.size(); ++i)
	{
		ObstacleData obstacle;
		obstaclesNode[i] >> obstacle;
		level.m_obstacles.push_back(obstacle);
	}

	const YAML::Node& searchNode = levelNode["searchLocations"].as<YAML::Node>();
	for (unsigned i = 0; i < searchNode.size(); ++i)
	{
		SearchData searchPos;
		searchNode[i] >> searchPos;
		level.m_search.push_back(searchPos);
	}

	const YAML::Node& safestNode = levelNode["safestPosition"].as<YAML::Node>();
	for (unsigned i = 0; i < safestNode.size(); ++i)
	{
		SafestData safestPos;
		safestNode[i] >> safestPos;
		level.m_safest.push_back(safestPos);
	}
}

////////////////////////////////////////////////////////////
bool LevelLoader::load(int nr, LevelData& level)
{
	std::stringstream ss;
	ss << "level";
	ss << nr;
	ss << ".yaml";

	try
	{
		YAML::Node baseNode = YAML::LoadFile("resources/levels/" + ss.str());
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

