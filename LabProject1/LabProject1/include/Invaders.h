/// <summary>
/// @Author: Paul Nolan
/// @Version 2.0
/// </summary>

#pragma once
#include <SFML\Graphics.hpp>
#include <Bolt.h>

class Invaders
{
public:
	Invaders();
	~Invaders();
	//The functions which update and render the class
	void update(Bolt & bo);
	void render(sf::RenderWindow &window);
	//The functions which set the invaders positions, moves them abd changes their directions
	void setPosition(sf::Vector2f const & pos, int column, int row);
	void movement();
	void changeDirection();
private:
	//The shapes representing the invaders
	sf::RectangleShape rectangles[6][2];
	//The invaders positions
	sf::Vector2f position[6][2];
	//The velocity of the shapes
	float velocity;
	//The bool which represents the direction the shape takes
	bool direction[2];
	//The bool which determines if the invader is drawn
	bool active[6][2];

	float width;
	float height;
};

