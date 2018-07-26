/// <summary>
/// @Author: Paul Nolan
/// @Version 2.0
/// </summary>

#include "Bolt.h"
#include <iostream>
//The bolt and its values are set up here
Bolt::Bolt(KeyHandler const & object) :
	m_keyHandler(object)
{
	width = 10;
	speedX = 4;
	speedY = -4;
	rectangle.setFillColor(sf::Color::Green);
	rectangle.setSize(sf::Vector2f(width, 10));
	powerUpMessageSetUp();
	setSounds();
}

Bolt::~Bolt()
{
}

sf::Vector2f Bolt::pos()
{
	return sf::Vector2f(x(), y());
}

float Bolt::x()
{
	return position.x;
}

float Bolt::y()
{
	return position.y;
}

float Bolt::recWidth()
{
	return width;
}
//This sets up the message showing the number of power ups left
void Bolt::powerUpMessageSetUp()
{
	if (m_font.loadFromFile("ASSETS\\FONTS\\arial_narrow_7.ttf"))
	{

	}

	powerUpMessage.setCharacterSize(20);
	powerUpMessage.setPosition(640, 0);
	powerUpMessage.setFont(m_font);
	powerUpMessage.setString("Power Ups Remaining:" + std::to_string(maxUse - noOfUse));
}
//This sets up the soounds used when deflections occur
void Bolt::setSounds()
{
	if (m_wallBuffer.loadFromFile("ASSETS\\SOUNDS\\WallDeflect.wav"))
	{


	}
	m_wallDeflect.setBuffer(m_wallBuffer);

	if (m_arrayBuffer.loadFromFile("ASSETS\\SOUNDS\\ArrayDeflect.wav"))
	{

	}

	m_arrayDeflect.setBuffer(m_arrayBuffer);

	if (m_paddleBuffer.loadFromFile("ASSETS\\SOUNDS\\PaddleDeflect.wav"))
	{

	}

	m_paddleDeflect.setBuffer(m_paddleBuffer);
}
//This updates the games state
void Bolt::update(Paddle & pad, double *eTime)
{
	handleKeyInput();
	movement();

	//The below two situations are for deflections between the bolt and walls/paddle

	if ((pad.y() - position.y < 10 && pad.y() - position.y >= 0) &&((position.x + width > pad.x()) && (position.x < pad.x() + pad.recWidth() )))
	{
		deflectionPad(pad);
	}

	if (position.x < 0 || position.x + width > 840 || position.y < 0)
	{
		deflectionWall();
	}
	//This resspawns the bolt if it goes off screeb
	if (position.y > 650)
	{
		respawn(eTime);
	}
	//This if sitation helps to limit the power up and limit its duration
	if (powerActive == true)
	{
		powerTimer += 1.0 / 60;

		if (powerTimer > timeLimit)
		{
			powerDown();
		}
	}
}
//This function draws the bolt
void Bolt::render(sf::RenderWindow &window)
{
	window.draw(powerUpMessage);
	window.draw(rectangle);
}
//This function sets the bolts position and velocity
void Bolt::setBolt(sf::Vector2f const & pos, sf::Vector2f const & vel)
{
	position = pos;
	velocity = vel;

	startPosition = position;
}
//This function is used to activate the power up
void Bolt::handleKeyInput()
{
	if (m_keyHandler.isPressed(sf::Keyboard::Up))
	{
		if (noOfUse < maxUse && speedX == startSpeed)
		{
			powerUp();
		}
	}
}
//Tjjis function moves the bolt
void Bolt::movement()
{
	position += velocity;
	rectangle.setPosition(position);
}

//This function is used for deflections that concern the paddle
void Bolt::deflectionPad(Paddle & pad)
{

	distance = (position.x + width/2) - (pad.x());

	deflectionAmount = distance / pad.recWidth() - 0.5;
	angle = 90 - (90 * abs(deflectionAmount));

	if (deflectionAmount < 0)
	{
		speedX = -speedX;
	}
	angle = angle * ((22.0 / 7.0) * 1.0 / 180.0);

	velocity.x = cos(angle) * speedX;

	velocity.y = sin(angle) * speedY;

	if (speedX < 0)
	{
		speedX = -speedX;
	}

	m_paddleDeflect.play();
}

void Bolt::deflectionWall()
{
	//The below if situations prevent the bolt from leaving the screen by the top, left and right sides.
	if (position.x < 0)
	{
		position.x = 0;
		velocity.x = -velocity.x;
	}

	else if (position.x + width > 840)
	{
		position.x = 840 - width;
		velocity.x = -velocity.x;
	}

	if (position.y < 0)
	{
		position.y = 0;
		velocity.y = -velocity.y;
	}
	//This plays the associated sounds
	m_wallDeflect.play();
}

void Bolt::deflectionArrays(sf::Vector2f pos, float aWidth, float height)
{
	//This is the if situation for y axis deflection. Two velocity changes are used for situations where the bolt strikes two bricks going up at the same time
		if ((pos.y > position.y && pos.y < position.y + width) || (pos.y + height > position.y && pos.y + height < position.y + width))
		{
			if (velocity.y > 0 && (pos.y + height > position.y && pos.y + height < position.y + width))
			{
				velocity.y = -velocity.y;
			}
			velocity.y = -velocity.y;
		}
		//This is the situation used for axis collsions. Note: This situation is not perfect and does not work for upper left corners
		else if ((pos.x > position.x && pos.x < position.x + width) || (pos.x + aWidth > position.x && pos.x + aWidth < position.x + width))
		{
		   velocity.x = -velocity.x;
		}
	//This situation is used for corner collsions
	else if ((position.x < pos.x && position.y < pos.y) || (position.x < pos.x + aWidth && position.y < pos.y + height)
		||
		(position.x < pos.x + aWidth && position.y < pos.y) || (position.x < pos.x && position.y < pos.y + height))
	{
		float x = pos.x;
		float y = pos.y;

		if (position.x > pos.x)
		{
			x += aWidth;
		}
		if (position.y > pos.y)
		{
			y += height;
		}
		//These two are used to find which side the bolt is closest two and so which to deflect from
		if (x - position.x < y - position.y)
		{
			velocity.y = -velocity.y;
		}
		if (x - position.x > y - position.y)
		{
			velocity.x = -velocity.x;
		}
	}
		//This plays the associated sounds
	m_arrayDeflect.play();
}

//This function resets thee bolt if it goes offscreen and applies a time penalty
void Bolt::respawn(double *eTime)
{
	position = startPosition;

	rectangle.setPosition(position);

	if (speedX == startSpeed * multiplyer)
	{
		powerDown();
	}

	*eTime += timePenalty;
}

//This function applies the power up
void Bolt::powerUp()
{
	speedX = speedX * multiplyer;
	speedY = speedY * multiplyer;

	velocity.x = velocity.x * multiplyer;
	velocity.y = velocity.y * multiplyer;

	noOfUse++;
	powerUpMessage.setString("Power Ups Remaining:" + std::to_string(maxUse - noOfUse));
	powerActive = true;
}
//This function reverses the changes from the above function
void Bolt::powerDown()
{
	speedX = speedX / multiplyer;
	speedY = speedY / multiplyer;

	velocity.x = velocity.x / multiplyer;
	velocity.y = velocity.y / multiplyer;

	powerTimer = 0;
	powerActive = false;
}