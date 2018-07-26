/// <summary>
/// @Author: Paul Nolan
/// @Version 2.0
/// </summary>

#pragma once
#include <SFML\Graphics.hpp>
#include <Bolt.h>

class Bricks
{
public:
	Bricks();
	~Bricks();
	//The functions used to update the bricks state, render them and set their position
	void update(Bolt & bo);
	void render(sf::RenderWindow &window);
	void setPosition(sf::Vector2f const & pos, int column, int row);
private:
	//The 2d arrays representing the Bricks, their positions and whether or not they are activee
	sf::RectangleShape rectangles[6][2];
	sf::Vector2f position[6][2];
	bool active[6][2];
	//The float representing the length and width of the bricks
	float width;
	float height;
};

