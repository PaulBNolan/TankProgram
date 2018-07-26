/// <summary>
/// @Author: Paul Nolan
/// @Version 2.0
/// </summary>

#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <KeyHandler.h>
#include <Paddle.h>
#include <CollisionDetector.h>
#include <vector>

class Bolt
{
public:
	Bolt(KeyHandler const & object);
	~Bolt();

	void update(Paddle & pad, double *eTime);
	void render(sf::RenderWindow &window);
	void setBolt(sf::Vector2f const & pos, sf::Vector2f const & vel);

	//This function handles key inputs
	void handleKeyInput();

	void movement();
	//These functions deal with the deflections the bolt will experience
	void deflectionPad(Paddle & pad);
	void deflectionWall();
	void deflectionArrays(sf::Vector2f pos, float width, float height);

	void respawn(double *eTime);
	void powerUp();
	void powerDown();

	sf::Vector2f pos();
	float recWidth();
	float x();
	float y();

	//These functions set up the text and sounds
	void setSounds();
	void powerUpMessageSetUp();
private:

	sf::RectangleShape rectangle;

	//These values are used to load in sound bits from the assets folder
	sf::SoundBuffer m_paddleBuffer;
	sf::SoundBuffer m_wallBuffer;
	sf::SoundBuffer m_arrayBuffer;
	//These values are used to play the sound
	sf::Sound m_paddleDeflect;
	sf::Sound m_wallDeflect;
	sf::Sound m_arrayDeflect;

	KeyHandler const & m_keyHandler;
	//These are the values for the bolts current position and the position they will respawn in
	sf::Vector2f position;
	sf::Vector2f startPosition;
	//This is the velocity the shape moves at
	sf::Vector2f velocity;
	
	sf::Text powerUpMessage;
	sf::Font m_font;

	float distance;
	float width;
	float length;

	float startSpeed = 4;
	//These two values are used to recalulate the velocity in paddle deflection
	float speedX;
	float speedY;
	//These are the values related to the power ups
	float multiplyer = 2;
	bool powerActive = false;
	double powerTimer = 0;
	int timeLimit = 5;
	int timePenalty = 3;
	//These values are used in the deflection between the paddle and bolt
	double deflectionAmount;
	double angle;

	//These represent the number of times the power ups can be used and its max limit
	int maxUse = 3;
	int noOfUse;
};

