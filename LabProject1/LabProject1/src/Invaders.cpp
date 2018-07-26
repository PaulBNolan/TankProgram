/// <summary>
/// @Author: Paul Nolan
/// @Version 2.0
/// </summary>

#include "Invaders.h"
//The invaders and their respective values are set up
Invaders::Invaders()
{
	width = 40;
	height = 40;
	for (int index = 0; index < 6; index++)
	{
		for (int index2 = 0; index2 < 2; index2++)
		{
			rectangles[index][index2].setFillColor(sf::Color::Blue);
			rectangles[index][index2].setSize(sf::Vector2f(width, height));
			active[index][index2] = true;
			direction[index2] = true;
		}
	}
	velocity = 2;
}


Invaders::~Invaders()
{
}
//The position of the invaders are set
void Invaders::setPosition(sf::Vector2f const & pos, int column, int row)
{
	rectangles[column][row].setPosition(pos);
	position[column][row] = pos;
}
//This function updates the state of the invaders
void Invaders::update(Bolt & bo)
{
	//This function moves the invaders
	movement();
	//This function changes their direction
	changeDirection();
	//This for loop is used to check for collsions with the bolt
	for (int index = 0; index < 6; index++)
	{
		for (int index2 = 0; index2 < 2; index2++)
		{
			if ((position[index][index2].x < bo.x() + bo.recWidth() && position[index][index2].x + width > bo.x())
				&&
				(position[index][index2].y < bo.y() + bo.recWidth() && position[index][index2].y + height > bo.y())
				&&
				active[index][index2] == true)
			{
				active[index][index2] = false;
				bo.deflectionArrays(position[index][index2], width, height);
			}
		}
	}
}
void Invaders::render(sf::RenderWindow &window)
{
	for (int index = 0; index < 6; index++)
	{
		for (int index2 = 0; index2 < 2; index2++)
		{
			if (active[index][index2] == true)
			{
				window.draw(rectangles[index][index2]);
			}
		}
	}
}
//This function helps move the invaders
void Invaders::movement()
{
	for (int index = 0; index < 6; index++)
	{
		for (int index2 = 0; index2 < 2; index2++)
		{
			if (direction[index2] == true)
			{
				position[index][index2].x -= velocity;
			}
			else if (direction[index2] == false)
			{
				position[index][index2].x += velocity;
			}
			rectangles[index][index2].setPosition(position[index][index2]);
		}
	}
}
//This function causes a row of invaders to change direction in case one of them hits the screens border
void Invaders::changeDirection()
{
	for (int index = 0; index < 6; index++)
	{
		for (int index2 = 0; index2 < 2; index2++)
		{
			if (active[index][index2] == true)
			{
				if (position[index][index2].x <= 0)
				{
					direction[index2] = false;
				}
				else if (position[index][index2].x >= 800)
				{
					direction[index2] = true;
				}
			}
		}
	}
}