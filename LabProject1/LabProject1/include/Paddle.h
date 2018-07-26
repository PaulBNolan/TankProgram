/// <summary>
/// @Author: Paul Nolan
/// @Version 2.0
/// </summary>

#pragma once
#include <SFML/Graphics.hpp>
#include <KeyHandler.h>


class Paddle
{
public:
	Paddle(KeyHandler const & object);
	~Paddle();
	//This function uppdates the paddles state
	void update();
	//Thiis function renders it
	void render(sf::RenderWindow &window);
	void handleKeyInput();

	void setPosition(sf::Vector2f const & pos);
	//The below two functions prevents the paddle from leaving the screen and allows it to move
	void positionCorrection();
	void movement();

	//These functions allow the x and y position of the paddle along wiith its width to be used outside this class
	sf::Vector2f pos();
	float recWidth();
	float x();
	float y();
	
private:
	sf::RectangleShape rectangle;

	KeyHandler const & m_keyHandler;

	//This is the paddles position
	sf::Vector2f position;
	//The below two represent the paddles velocity
	float velocity;
	float direction;
	//This value represents its width
	float width;
};

