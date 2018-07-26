#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"

/// <summary>
/// @author Paul Nolan
/// @version 1.0
/// </summary>

/// <summary>
/// @brief A basic HUD implementation.
/// 
/// Shows current game state only.
/// </summary>

class HUD
{
public:
	/// <summary>
	/// @brief Default constructor that stores a font for the HUD and initialises the general HUD appearance.
	/// </summary>
	HUD(sf::Font & hudFont);

	/// <summary>
	/// @brief Checks the current game state and sets the appropriate status text on the HUD.
	/// </summary>
	/// <param name="gameState">The current game state</param>
	void update(GameState const & gameState, AiBehaviour const & aiState, int playerHealth, int enemyHealth, bool healthActive);

	/// <summary>
	/// @brief Draws the HUD outline and text.
	///
	/// </summary>
	/// <param name="window">The SFML Render window</param>
	void render(sf::RenderWindow & window);

private:
	// The font for this HUD.
	sf::Font m_textFont;

	// A container for the current HUD text.
	sf::Text m_gameStateText;

	sf::Text m_playerHealthText;

	sf::Text m_aiHealthText;

	sf::Text m_healthPackActiveText;

	sf::Text m_aiStatusText;

	// A simple background shape for the HUD.
	sf::RectangleShape m_hudOutline;
};