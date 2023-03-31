#include "Map.h"

Map::Map() {
	backgroundTexture.loadFromFile("../Resources/BG.png");
	background.setTexture(backgroundTexture);

	background.setPosition(0, 0);
	background.setOrigin(0, 1024 / 2);

	groundTexture.loadFromFile("../Resources/SBG.png");
	ground.setTexture(groundTexture);
	ground.setPosition(0, 0);
}

sf::Sprite& Map::getBackgroundSprite() { return background; };
sf::Sprite& Map::getGroundSprite() { return ground; };