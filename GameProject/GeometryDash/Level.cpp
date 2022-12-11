#include "Level.h"

Level::Level(const char* mapPath, const char* musicPath, int levelLength, int mapHeight) {
	this->mapPath = mapPath;
	this->musicPath = musicPath;

	this->levelLength = levelLength;
	this->mapHeight = mapHeight;
}

void Level::runLevelProcess(sf::View& view, sf::RenderWindow& window) {
	levelMap.LoadFromFile(mapPath);

	cubePortal = levelMap.GetAllObjects("cubePortal");
	shipPortal = levelMap.GetAllObjects("shipPortal");

	SetMusic();

	player = new Cube(levelMap, mapHeight);
	mode = CUBE;

	sf::Clock dtClock;

	while (window.isOpen()) {
		CheckWindowCloseRequest(window);

		CheckPauseRequest();
		TimeUpdate(dtClock);

		CheckModeChangeRequest();
		Update(view, window);

		if (player->getIsDead()) {
			Respawn(view);
		}

		Display(view, window);
	}

	delete player;
}

Level::Map::Map() {
	backgroundTexture.loadFromFile("Resources/BG.png"); /// Изменить путь
	background.setTexture(backgroundTexture);

	background.setColor(sf::Color::Blue);
	background.setScale(1.5f, 1.5f);

	background.setPosition(0, 0);
	background.setOrigin(0, 1024 / 2.f);

	groundTexture.loadFromFile("Resources/SBG.png"); /// Изменить путь
	ground.setTexture(groundTexture);
	ground.setColor(sf::Color::Blue);
	ground.setPosition(0, 0);
}

sf::Sprite& Level::Map::getBackgroundSprite() { return background; };
sf::Sprite& Level::Map::getGroundSprite() { return ground; };

void Level::SetMusic() {
	music.openFromFile(musicPath);
	music.play();
	music.setVolume(50.f);
}

void Level::CheckPauseRequest() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		isPaused = true;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		isPaused = false;
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

void Level::CheckWindowCloseRequest(sf::RenderWindow& window) {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			music.stop();
			window.close();
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
	map.getBackgroundSprite().move(player->getDx() * time * 0.9, 0);

	player->update(time);
}

void Level::Respawn(sf::View& view) {
	player->setIsDead(false);
	delete player;
	mode = CUBE;
	player = new Cube(levelMap, mapHeight);
	view.reset(sf::FloatRect(0, mapHeight - 384, 854, 480));
	map.getBackgroundSprite().setPosition(0, 0);
	music.setPlayingOffset(music.getPlayingOffset() - music.getPlayingOffset());
}

void Level::Display(sf::View& view, sf::RenderWindow& window) {
	window.setView(view);
	window.clear(sf::Color::Blue);
	window.draw(map.getBackgroundSprite());
	levelMap.Draw(window);

	for (int i = 0; i < levelLength / 128; i++) {
		map.getGroundSprite().setPosition(i * 128, mapHeight);
		window.draw(map.getGroundSprite());
	}

	window.draw(player->getPlayerSprite());
	window.display();
}