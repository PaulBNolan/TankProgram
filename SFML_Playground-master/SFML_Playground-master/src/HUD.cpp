#include "HUD.h"
#include <iostream>

/// <summary>
/// @author Paul Nolan
/// @version 1.0
/// </summary>

////////////////////////////////////////////////////////////
HUD::HUD(sf::Font & hudFont)
	: m_textFont(hudFont)
{
	//Hub text and shape are set up

	m_gameStateText.setFont(hudFont);
	m_gameStateText.setCharacterSize(30);
	m_gameStateText.setFillColor(sf::Color::Yellow);
	m_gameStateText.setPosition(sf::Vector2f(640, 5));
	m_gameStateText.setString("Game Running");

	m_healthPackActiveText.setFont(hudFont);
	m_healthPackActiveText.setCharacterSize(30);
	m_healthPackActiveText.setFillColor(sf::Color::Green);
	m_healthPackActiveText.setPosition(sf::Vector2f(250, 5));

	m_playerHealthText.setFont(hudFont);
	m_playerHealthText.setCharacterSize(30);
	m_playerHealthText.setFillColor(sf::Color::Blue);
	m_playerHealthText.setPosition(sf::Vector2f(10, 5));

	m_aiHealthText.setFont(hudFont);
	m_aiHealthText.setCharacterSize(30);
	m_aiHealthText.setFillColor(sf::Color::Red);
	m_aiHealthText.setPosition(sf::Vector2f(1260, 5));

	m_aiStatusText.setFont(hudFont);
	m_aiStatusText.setCharacterSize(30);
	m_aiStatusText.setFillColor(sf::Color::Cyan);
	m_aiStatusText.setPosition(sf::Vector2f(870, 5));

	//Setting up our hud properties 
	m_hudOutline.setSize(sf::Vector2f(1440.0f, 40.0f));
	m_hudOutline.setFillColor(sf::Color(0, 0, 0, 38));
	m_hudOutline.setOutlineThickness(-.5f);
	m_hudOutline.setOutlineColor(sf::Color(0, 0, 0, 100));
	m_hudOutline.setPosition(0, 0);
}

////////////////////////////////////////////////////////////
void HUD::update(GameState const & gameState, AiBehaviour const & aiState, int playerHealth, int enemyHealth,bool healthActive)
{
	m_playerHealthText.setString("Player Health:" + std::to_string(playerHealth));
	m_aiHealthText.setString("Ai Health:" + std::to_string(enemyHealth));

	if (healthActive == true)
	{
		m_healthPackActiveText.setString("Healthpack:Available");
	}
	else
	{
		m_healthPackActiveText.setString("Healthpack:Not Available");
	}

	//These two switch cases are used to alter the hub text depending on the current situation
	switch (gameState)
	{
	case GameState::GAME_RUNNING:
		m_gameStateText.setString("Game Running");
		break;
	case GameState::GAME_WIN:
		m_gameStateText.setString("You Won");
		break;
	case GameState::GAME_LOSE:
		m_gameStateText.setString("You Lost");
		break;
	default:
		break;
	}

	switch (aiState)
	{
	case AiBehaviour::SEEK_PLAYER:
		m_aiStatusText.setString("Ai State:Seeking Player");
		break;
	case AiBehaviour::FIND_HEALTH:
		m_aiStatusText.setString("Ai State:Seeking Health");
		break;
	case AiBehaviour::FIND_COVER:
		m_aiStatusText.setString("Ai State::Seeking Cover");
		break;
	default:
		break;
	}
}

void HUD::render(sf::RenderWindow & window)
{
	window.draw(m_hudOutline);

	window.draw(m_playerHealthText);
	window.draw(m_aiHealthText);
	window.draw(m_aiStatusText);

	window.draw(m_gameStateText);
	window.draw(m_healthPackActiveText);
}