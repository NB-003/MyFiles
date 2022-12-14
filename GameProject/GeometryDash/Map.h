#pragma once

#include <SFML/Graphics.hpp>

class Map { // Background and ground sprites
public:
	Map();

	sf::Sprite& getBackgroundSprite();
	sf::Sprite& getGroundSprite();

private:
	sf::Texture backgroundTexture, groundTexture;
	sf::Sprite background, ground;
};