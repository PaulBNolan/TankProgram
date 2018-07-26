/// <summary>
/// @Author: Paul Nolan
/// @Version 2.0
/// </summary>

#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <time.h>
#include <Paddle.h>
#include <Bricks.h>
#include <Invaders.h>
#include <LevelLoader.h>
#include <Bolt.h>

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

	//The objeccts representing the onscreen entity
	LevelData m_levelSetUp;
	KeyHandler m_keyHandler;
	Paddle m_paddle;
	Bolt m_bolt;
	Bricks m_bricks;
	Invaders m_invaders;

	//The amount of time the game was on for and its maximum before the window is closed
	int time;
	double ExactTime;
	int timeLimit;
private:
	//The functions that set up the basic frame work of the program
	void processEvents();
	void update(sf::Time t_deltaTime);
	void processGameEvents(sf::Event& event);
	void render();
	
	//The function that sets up text for the hub
	void setupFontAndText();

	sf::RenderWindow m_window;
	sf::Font m_font;
	sf::Text m_currentTime;
	sf::Text m_timeLimit; 
	bool m_exitGame;
};

#endif // !GAME

