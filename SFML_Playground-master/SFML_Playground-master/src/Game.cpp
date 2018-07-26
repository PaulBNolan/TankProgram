#include "Game.h"
#include <iostream>

/// <summary>
/// @mainpage Tanks
/// @Author Paul Nolan
/// @Version 1.0
///
/// A program used to create a game that makes use of the following:
/// Game states
/// Key Handler
/// Level loading from yaml files
/// Basic Ai implementation
/// Collisions between sprites
/// </summary>

// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_width, ScreenSize::s_height, 32), "SFML Playground", sf::Style::Default)
	, m_tank(m_texture, m_wallSprites, m_keyHandler)
	, m_aiTank(m_texture, m_wallSprites)
	, m_hud(m_font) 
{
	m_window.setVerticalSyncEnabled(true);

	if (!m_texture.loadFromFile("./resources/images/SpriteSheet.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}


	int currentLevel = 1;

	if (!LevelLoader::load(currentLevel, m_level))
	{
		return;
	}

	m_tank.setPosition(m_level.m_tank.m_position);

	if (!m_bgTexture.loadFromFile(m_level.m_background.m_fileName))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}
	m_bgSprite.setTexture(m_bgTexture);
	m_bgSprite.setPosition(0, 0);

	sf::Sprite sprite;
	sprite.setTexture(myTexture);

	generateWalls();

	m_aiTank.init(m_level.m_aiTank.m_position);
	m_aiTank.initPositions(m_level.m_search, m_level.m_safest);

	if (!m_font.loadFromFile("./resources/font/akashi.ttf"))
	{
		std::string s("Error loading font");
		throw std::exception(s.c_str());
	}

	m_healthPack.m_widthRange = ScreenSize::s_width;
	m_healthPack.m_heightRange = ScreenSize::s_height;
}

////////////////////////////////////////////////////////////
void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;

	while (m_window.isOpen())
	{
		sf::Time dt = clock.restart();

		lag += dt.asMilliseconds();

		processEvents();

		while (lag > MS_PER_UPDATE)
		{
			update(MS_PER_UPDATE);
			lag -= MS_PER_UPDATE;
		}
		update(MS_PER_UPDATE);

		render();
	}
}

////////////////////////////////////////////////////////////
void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
		processGameEvents(event);
	}
}

////////////////////////////////////////////////////////////
void Game::processGameEvents(sf::Event& event)
{
	// check if the event is a a mouse button release
	
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		m_keyHandler.updateKey(event.key.code, true);
		break;
	case sf::Event::KeyReleased:
		m_keyHandler.updateKey(event.key.code, false);
		break;
	default:
		break;
	}
	
}

////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	m_hud.update(m_gameState, m_aiTank.m_aiBehaviour, m_tank.m_health, m_aiTank.m_health, m_healthPack.m_active);

	switch (m_gameState)
	{
	case(GameState::GAME_RUNNING):
	{
		m_tank.update(dt, m_aiTank.getSprite(), m_aiTank.m_hit);
		m_aiTank.update(m_tank,m_healthPack, dt);

		m_healthPack.update(dt,m_tank.getBase(), m_tank.m_health, m_aiTank.getSprite(),m_aiTank.m_health, m_wallSprites);

		if (m_aiTank.collidesWithPlayer(m_tank) == true || m_tank.m_health == 0)
		{
			m_gameState = GameState::GAME_LOSE;
		}

		if (m_aiTank.m_health == 0)
		{
			m_gameState = GameState::GAME_WIN;
		}
		break;
	}

	case(GameState::GAME_LOSE):
	{
		m_tank.setPosition(m_level.m_tank.m_position);
		m_aiTank.setPositon(m_level.m_aiTank.m_position);

		m_timer += dt;

		if (m_timer > dt * 240)
		{
			m_timer = 0;
			m_gameState = GameState::GAME_RUNNING;
		}
		break;
	}

	case(GameState::GAME_WIN):
	{
		break;
	}
	}
}

////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));
	m_window.draw(m_bgSprite);
	m_window.draw(m_sprite);

	m_tank.render(m_window);

	m_aiTank.render(m_window);

	m_healthPack.render(m_window);

	for (auto &obstacle : m_wallSprites)
	{
		m_window.draw(obstacle);
	}

	m_hud.render(m_window);

	m_window.display();
}

void Game::generateWalls()
{
	sf::IntRect wallRect(2, 129, 33, 23);
	for (ObstacleData const & obstacle : m_level.m_obstacles)
	{
		sf::Sprite sprite;
		sprite.setTexture(m_texture);
		sprite.setTextureRect(wallRect);
		sprite.setOrigin(wallRect.width / 2.0, wallRect.height / 2.0);
		sprite.setPosition(obstacle.m_position);
		sprite.setRotation(obstacle.m_rotation);
		m_wallSprites.push_back(sprite);
	}
}