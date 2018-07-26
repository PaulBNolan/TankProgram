#pragma once

#include <map>
#include <SFML/Window/Keyboard.hpp>

/// <summary>
/// @author Paul Nolan
/// @version 1.0
/// </summary>

/// <summary>
/// @brief Class to manage simultaneous key input in SFML.
/// 
/// </summary>
class KeyHandler
{
public:

   /// <summary>
   /// @brief Default constructor.
   /// 
   /// Creates an empty mapping of keypresses to bool.
   /// </summary>
   KeyHandler();

   /// <summary>
   /// @brief Updates key pressed status.
   /// Assigns a value of true or false to the specified key. 
   /// </summary>
   /// <param name="key">An SFML key</param>
   /// <param name="isPressed">true if key pressed</param>
   void updateKey(sf::Keyboard::Key key, bool isPressed);

   /// <summary>
   /// @brief Checks if a specific key was pressed.
   /// Searches the map to check if the specified key was pressed. 
   /// </summary>
   /// <param name="key">An SFML key</param>
   /// <returns>true if specified key was pressed</returns>
   bool isPressed(sf::Keyboard::Key key) const;

private:

   // KeyMap is an alias for a std::map of sf::Keyboard::Key to bool
   typedef std::map<sf::Keyboard::Key, bool> KeyMap;

   // A std::map to manage the status of key presses.
   KeyMap m_keyMap;
};

