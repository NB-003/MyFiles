#pragma once
#include "Cube.h"
#include "Ship.h"

class Level {
public:
	Level(const char*, const char*, int, int);

	void runLevelProcess(sf::View&, sf::RenderWindow&);

private:
	class Map {
	public:
		Map();

		sf::Sprite& getBackgroundSprite();
		sf::Sprite& getGroundSprite();

	private:
		sf::Texture backgroundTexture, groundTexture;
		sf::Sprite background, ground;
	} map;

	TmxLevel levelMap;
	std::vector<TmxObject> cubePortal, shipPortal;

	const char* mapPath;
	const char* musicPath;
	int levelLength, mapHeight;
	sf::Music music;

	bool isPaused = false;
	float time;

	Player* player;

	enum Mode {
		CUBE,
		SHIP
	} mode;

	void SetMusic();
	void CheckPauseRequest();
	void TimeUpdate(sf::Clock&);
	void CheckWindowCloseRequest(sf::RenderWindow&);
	void CheckModeChangeRequest();
	void Update(sf::View&, sf::RenderWindow&);
	void Respawn(sf::View&);
	void Display(sf::View&, sf::RenderWindow&);
};