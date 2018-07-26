/// <summary>
/// @Author: Paul Nolan
/// @Version 2.0
/// </summary>

#pragma once
#include <map>
#include <SFML/Window/Keyboard.hpp>
class KeyHandler
{
public:
	KeyHandler();

	void updateKey(sf::Keyboard::Key key, bool isPressed);

	bool isPressed(sf::Keyboard::Key key) const;

private:
	typedef std::map<sf::Keyboard::Key, bool> KeyMap;

	KeyMap m_keyMap;
};

