/// <summary>
/// @Author: Paul Nolan
/// @Version 2.0
/// </summary>

#include "Bricks.h"
#include <iostream>

Bricks::Bricks()
{
	//The bricks are set up here
     width = 140;
	 height = 60;
	for (int index = 0; index < 6; index++)
	{
		for (int index2 = 0; index2 < 2; index2++)
		{
			rectangles[index][index2].setFillColor(sf::Color::Red);
			rectangles[index][index2].setSize(sf::Vector2f(width, height));
			active[index][index2] = true;
		}
	}
}

//The position is set in this function
void Bricks::setPosition(sf::Vector2f const & pos ,int column, int row)
{
	rectangles[column][row].setPosition(pos);
	position[column][row] = pos;
}
Bricks::~Bricks()
{
}



//This is used to update the bricks state. Due to their role this function is only used for collsions
void Bricks::update(Bolt & bo)
{
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
//This function renders the bricks
void Bricks::render(sf::RenderWindow &window)
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
