#pragma once

#include "../Player/Cube/Cube.h"
#include "../Player/Ship/Ship.h"
#include "Map/Map.h"

class Level {
public:
	Level(const char*, const char*, int, int);

	void runLevelProcess(sf::View&, sf::RenderWindow&, sf::Color, sf::Color);

private:
	TmxLevel levelMap;
	std::vector<TmxObject> cubePortal, shipPortal;

	const char* mapPath;
	const char* musicPath;
	int levelLength, mapHeight;

	sf::Music music;
	Map map;

	bool isLevel = false, isPaused = false;
	float time;

	Player* player;

	enum Mode {
		CUBE,
		SHIP
	} mode;

	void SetMusic();
	void TimeUpdate(sf::Clock&);
	void CheckLevelStopRequest(sf::RenderWindow&);
	void CheckModeChangeRequest();
	void Update(sf::View&, sf::RenderWindow&);
	void Respawn(sf::View&);
	void Display(sf::View&, sf::RenderWindow&, sf::Color);
};