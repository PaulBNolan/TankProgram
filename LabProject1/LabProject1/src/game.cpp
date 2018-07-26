/// <summary>
/// @mainpage: Programming
/// @Author: Paul Nolan
/// 
/// Project Summary:A program used to run a game that is essentially a mix of space
/// invaders and Arkanoid
///
/// Total
/// 15hr
/// </summary>

#include "Game.h"
#include <iostream>



Game::Game() :
	m_window{ sf::VideoMode{ 840, 650, 32 }, "SFML Game" },
	m_exitGame{false}, //when true game will exit
	m_paddle(m_keyHandler),
	m_bolt(m_keyHandler),
	m_bricks(),
	m_invaders()
{
	m_window.setVerticalSyncEnabled(true);
	//Data is loaded from the yaml file and applied to the objects
	int currentLevel = 1;

	if (!LevelLoader::load(currentLevel, m_levelSetUp))
	{
		return;
	}

	timeLimit = m_levelSetUp.m_time.time;
	setupFontAndText(); // load font 

	m_paddle.setPosition(m_levelSetUp.m_paddle.m_position);

	m_bolt.setBolt(m_levelSetUp.m_bolt.m_position,m_levelSetUp.m_bolt.m_velocity);

	for (auto &brick : m_levelSetUp.m_bricks)
	{
		m_bricks.setPosition(brick.m_position,brick.m_column,brick.m_row);
	}

	for (auto &invader : m_levelSetUp.m_invaders)
	{
		m_invaders.setPosition(invader.m_position, invader.m_column, invader.m_row);
	}

	ExactTime = 0;
}


Game::~Game()
{
}


void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if ( sf::Event::Closed == event.type) // window message
		{
			m_window.close();
		}
		processGameEvents(event);
	}
}

//Input commands are processed here
void Game::processGameEvents(sf::Event& event)
{
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		{
			m_keyHandler.updateKey(event.key.code, true);
			break;
		}
	case sf::Event::KeyReleased:
		{
			m_keyHandler.updateKey(event.key.code, false);
			break;
		}
	default:
		break;
	}
}
//The objects are update in this function
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	m_invaders.update(m_bolt);
	m_paddle.update();
	m_bolt.update(m_paddle, &ExactTime);
	m_bricks.update(m_bolt);

	ExactTime += 1.0 / 60.0;
	time = ExactTime;
	m_currentTime.setString("Current Time:" + std::to_string(time));

	if (time == timeLimit)
	{
		m_exitGame = true;
	}
}

//The object render functions are called on here
void Game::render()
{
	m_window.clear();
	m_paddle.render(m_window);
	m_bolt.render(m_window);
	m_bricks.render(m_window);
	m_invaders.render(m_window);

	m_window.draw(m_currentTime);
	m_window.draw(m_timeLimit);
	m_window.display();
}

//The text font and values are set
void Game::setupFontAndText()
{
	if (m_font.loadFromFile("ASSETS\\FONTS\\arial_narrow_7.ttf"))
	{

	}
	m_currentTime.setCharacterSize(25);
	m_currentTime.setPosition(0, 0);
	m_currentTime.setFont(m_font);

	m_timeLimit.setCharacterSize(25);
	m_timeLimit.setPosition(420, 0);
	m_timeLimit.setFont(m_font);
	m_timeLimit.setString("Time Limit:" + std::to_string(timeLimit));

}