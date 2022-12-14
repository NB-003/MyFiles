#include "Level.h"

Level::Level(const char* mapPath, const char* musicPath, int levelLength, int mapHeight) {
	this->mapPath = mapPath;
	this->musicPath = musicPath;

	this->levelLength = levelLength;
	this->mapHeight = mapHeight;
}

void Level::runLevelProcess(sf::View& view, sf::RenderWindow& window, sf::Color backgroundColor, sf::Color groundColor) {
	isLevel = true;

	map.getBackgroundSprite().setPosition(0, mapHeight - 384);
	map.getBackgroundSprite().setColor(backgroundColor);
	map.getGroundSprite().setColor(groundColor);

	levelMap.LoadFromFile(mapPath); // Loading level structure from .tmx file (our map)

	cubePortal = levelMap.GetAllObjects("cubePortal");
	shipPortal = levelMap.GetAllObjects("shipPortal");

	SetMusic();

	player = new Cube(levelMap, mapHeight); // Starting always as cube
	mode = CUBE;

	sf::Clock dtClock;

	while (window.isOpen()) {
		if (!isLevel)
			break;

		CheckLevelStopRequest(window);

		TimeUpdate(dtClock);

		CheckModeChangeRequest();
		Update(view, window);

		if (player->getIsDead()) {
			Respawn(view);
		}

		Display(view, window, backgroundColor);
	}

	delete player;
}

void Level::SetMusic() {
	music.openFromFile(musicPath);
	music.play();
	music.setVolume(50.f);
}

void Level::TimeUpdate(sf::Clock& dtClock) {
	if (!isPaused) {
		if (music.getStatus() == sf::Sound::Paused)
			music.play();

		time = dtClock.getElapsedTime().asMicroseconds() / 800.f;
	}
	else {
		if (music.getStatus() == sf::Sound::Playing)
			music.pause();

		time = 0;
	}
	dtClock.restart();
}

void Level::CheckLevelStopRequest(sf::RenderWindow& window) {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			music.stop();
			window.close();
		}
		if (!isPaused && event.type == sf::Event::KeyReleased
			&&
			event.key.code == sf::Keyboard::Escape)
			isPaused = true;

		else if (isPaused && event.type == sf::Event::KeyReleased
			&&
			event.key.code == sf::Keyboard::Space)
			isPaused = false;

		else if ((isPaused && event.type == sf::Event::KeyReleased
			&&
			event.key.code == sf::Keyboard::Escape) || player->getX() >= levelLength) {
			music.stop();
			isLevel = false;
		}
	}
}

void Level::CheckModeChangeRequest() {
	float x = player->getX();
	float y = player->getY();

	for (int i = 0; i < cubePortal.size(); i++) {
		if (player->GetPlayerHitbox().intersects(cubePortal[i].rect) && mode != CUBE) {
			delete player;
			mode = CUBE;
			player = new Cube(levelMap, mapHeight, x, y + 16);
		}
	}

	for (int i = 0; i < shipPortal.size(); i++) {
		if (player->GetPlayerHitbox().intersects(shipPortal[i].rect) && mode != SHIP) {
			delete player;
			mode = SHIP;
			player = new Ship(levelMap, mapHeight, x, y + 16);
		}
	}
}

void Level::Update(sf::View& view, sf::RenderWindow& window) {
	view.move(player->getDx() * time, 0);

	if (player->getY() < mapHeight - 192 && player->getDy() != 0) {
		view.move(0, player->getDy() * time * 0.8);
	}
	map.getBackgroundSprite().move(player->getDx() * time * 0.95, 0);

	player->update(time);
}

void Level::Respawn(sf::View& view) {
	player->setIsDead(false);
	delete player;
	mode = CUBE;
	player = new Cube(levelMap, mapHeight);
	view.reset(sf::FloatRect(0, mapHeight - 384, 854, 480));
	map.getBackgroundSprite().setPosition(0, mapHeight - 384);
	music.setPlayingOffset(music.getPlayingOffset() - music.getPlayingOffset());
}

void Level::Display(sf::View& view, sf::RenderWindow& window, sf::Color color) {
	window.setView(view);
	window.clear(color);

	window.draw(map.getBackgroundSprite());
	levelMap.Draw(window);

	for (int i = 0; i < levelLength / 128; i++) {
		map.getGroundSprite().setPosition(i * 128, mapHeight);
		window.draw(map.getGroundSprite());
	}

	window.draw(player->getPlayerSprite());
	window.display();
}