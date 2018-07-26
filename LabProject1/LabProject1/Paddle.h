#pragma once
#include <SFML/Graphics.hpp>
#include <KeyHandler.h>
class Paddle
{
public:
	Paddle();
	~Paddle();
	
	void update();
	void render();
	void handleKeyInput();

	void movement();
private:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::RectangleShape rectangle;

	KeyHandler const & m_keyHandler;

	sf::Vector2f startPosition;
	sf::Vector2f position;
	float velocity;
};

