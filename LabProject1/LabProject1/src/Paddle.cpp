/// <summary>
/// @Author: Paul Nolan
/// @Version 2.0
/// </summary>


#include "Paddle.h"

Paddle::Paddle(KeyHandler const & object) :
	m_keyHandler(object)
{
	direction = 10;
	width = 80;
	rectangle.setFillColor(sf::Color::White);
	rectangle.setSize(sf::Vector2f(width, 5));
}


Paddle::~Paddle()
{
}


void Paddle::setPosition(sf::Vector2f const & pos)
{
	rectangle.setPosition(pos);
	position = pos;
}

sf::Vector2f Paddle::pos()
{
	return sf::Vector2f(x(),y());
}

float Paddle::x()
{
	return position.x;
}

float Paddle::y()
{
	return position.y;
}

float Paddle::recWidth()
{
	return width;
}
//This function automatically updates the state of the paddle
void Paddle::update()
{
	handleKeyInput();
	positionCorrection();
}
//This function draws the paddle
void Paddle::render(sf::RenderWindow &window)
{
	window.draw(rectangle);
}
//This function responds to key commands
void Paddle::handleKeyInput()
{
	if (m_keyHandler.isPressed(sf::Keyboard::Left) == true)
	{
		velocity = -direction;
		movement();
	}

	 if (m_keyHandler.isPressed(sf::Keyboard::Right) == true)
	{
		velocity = direction;
		movement();
	}
}
//This function causes the paddle to move
void Paddle::movement()
{
	position.x += velocity;
	rectangle.setPosition(position);
}
//This function prevents it from going off screen
void Paddle::positionCorrection()
{
	if (position.x < 0)
	{
		position.x = 0;
	}
	else if (position.x > 760)
	{
		position.x = 760;
	}

	rectangle.setPosition(position);
}