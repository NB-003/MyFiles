#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <list>
#include "tmxlevel.h"

using namespace sf;
using namespace std;

class Map {
public:
	Map() {
		backgroundTexture.loadFromFile("Resources/BG.png"); /// Изменить путь
		background.setTexture(backgroundTexture);

		background.setColor(Color::Blue);
		background.setScale(1.5f, 1.5f);

		background.setPosition(0, 0);
		background.setOrigin(0, 1024 / 2.f);

		groundTexture.loadFromFile("Resources/SBG.png"); /// Изменить путь
		ground.setTexture(groundTexture);
		ground.setColor(Color::Blue);
		ground.setPosition(0, 0);
	}

	Sprite& getBackgroundSprite() { return background; };
	Sprite& getGroundSprite() { return ground; };

private:
	Texture backgroundTexture, groundTexture;
	Sprite background, ground;
};

class Player {
public:
	Player(TmxLevel& lvl, int mapHeight, int x_pos, int y_pos) {
		solid = lvl.GetAllObjects("solid");
		fatal = lvl.GetAllObjects("fatal");

		x = x_pos;
		y = y_pos - 16.f;
		startGround = mapHeight;
		ground = startGround;
		dx = 0.245;
		dy = 0;
		angle = 0;
		if (ground == y_pos)
			isOnGround = true;
		else
			isOnGround = false;
	}

	float getX() { return x; };
	float getY() { return y; };

	float getDx() { return dx; };
	void setDx(float dx) { this->dx = dx; };

	float getDy() { return dy; };
	void setDy(float dy) { this->dy = dy; };

	bool getIsOnGround() { return isOnGround; }
	void setIsOnGround(bool isOnGround) { this->isOnGround = isOnGround; };

	bool getIsDead() { return isDead; };
	void setIsDead(bool isDead) { this->isDead = isDead; };

	Sprite getPlayerSprite() { return sprite; };

	int getStartGround() { return startGround; };

	virtual void update(float) = 0;

protected:
	std::vector<TmxObject> solid, fatal;

	float x, y, dx, dy, angle;
	int hitbox = 32, startPos = 192, startGround, ground;
	bool isOnGround, isDead = false;

	Image image;
	Texture texture;
	Sprite sprite;

	FloatRect GetPlayerHitbox() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x - hitbox / 2.f, y - hitbox / 2.f, hitbox, hitbox);//эта ф-ция нужна для проверки столкновений 
	}

private:
	virtual void Move(float) = 0;
	virtual void CheckCollision() = 0;
	virtual void Respawn() = 0;
};

class Cube : public Player {
public:
	Cube(TmxLevel& lvl, int mapHeight, int x_pos, int y_pos) : Player(lvl, mapHeight, x_pos, y_pos) {
		image.loadFromFile("Resources/Cube001.png");
		texture.loadFromImage(image);

		sprite.setTexture(texture);
		sprite.setScale(32 / 310.f, 32 / 310.f);
		sprite.setPosition(x, y);
		sprite.setOrigin(310 / 2.f, 310 / 2.f);
		sprite.setColor(Color::Green);
	}

	void update(float time) override {
		if (Keyboard::isKeyPressed(Keyboard::Up))
			Move(time);

		CheckCollision();

		if(isDead)
			Respawn();

		x += dx * time;

		if (!isOnGround) {
			dy += time * 0.0015;
			angle += time * 0.16;
		}

		y += dy * time;
		if (y > ground - hitbox / 2.f) {
			y = ground - hitbox / 2.f;
			dy = 0;
			isOnGround = true;
		}

		if (isOnGround)
			angle = 0;

		sprite.setRotation(angle);
		sprite.setPosition(x, y);
	}

	virtual ~Cube() = default;

private:
	void Move(float) override {
		if (isOnGround == true) {
			dy = -0.45;
			isOnGround = false;
		}
	}

	void CheckCollision() override {
		for (int i = 0; i < solid.size(); i++) { //проходимся по объектам
			if (GetPlayerHitbox().intersects(solid[i].rect)) { //проверяем пересечение игрока с объектом
				//cout << x + 16 << endl;
				if (dy > 0) {
					ground = solid[i].rect.top; 
					dy = 0; 
					isOnGround = true; 
				}
			}
			if (x - hitbox / 2.f > solid[i].rect.left + solid[i].rect.width - 0.3
				&&
				x - hitbox / 2.f < solid[i].rect.left + solid[i].rect.width + 0.3) {

				isOnGround = false; 
				ground = startGround;

			}

		}

		for (int i = 0; i < fatal.size(); i++) {
			if (GetPlayerHitbox().intersects(fatal[i].rect))
				isDead = true;
		}
	}

	void Respawn() override {
		x = startPos;
		y = (ground = startGround) - 16.f;
		angle = 0;
	}
};

class Ship : public Player {
public:
	Ship(TmxLevel& lvl, int mapHeight, int x_pos, int y_pos) : Player(lvl, mapHeight, x_pos, y_pos) {
		top = startTop;

		image.loadFromFile("Resources/Ship01.png");
		texture.loadFromImage(image);

		sprite.setTexture(texture);
		sprite.setScale(32 / 288.f, 32 / 288.f);
		sprite.setPosition(x, y);
		sprite.setOrigin(382 / 2.f, 288 / 2.f);
		sprite.setColor(Color::Green);
	}

	void update(float time) override {
		if (Keyboard::isKeyPressed(Keyboard::Up))
			Move(time);

		CheckCollision();

		if (isDead)
			Respawn();

		x += dx * time;

		if (!Keyboard::isKeyPressed(Keyboard::Up)) {
			if(dy < 0)
				dy += time * gravity;

			if (!isOnGround) {
				dy += time * gravity;
				angle += time * 0.12;
				if (angle > 60)
					angle = 60;
			}

			isOnTop = false;
		}

		y += dy * time;

		while (y > ground - hitbox / 2.f) {
			y -= time * gravity;
		}
		if (y == ground - hitbox / 2.f) {
			isOnGround = true;
			dy = 0;
		}

		if (isOnGround && angle > 0)
			angle -= time * 0.2;

		if (isOnTop && angle < 0)
			angle += time * 0.22;

		sprite.setRotation(angle);
		sprite.setPosition(x, y);
	}

	virtual ~Ship() = default;

private:
	float gravity = 0.00025;
	int startTop = startGround - 288, top;
	bool isOnTop = false;

	void Move(float time) override {
		if (!isOnTop) {
			dy -= time * gravity;
			angle -= time * 0.12;
			if (angle < -60)
				angle = -60;
		}

		if (dy > 0)
			dy -= time * gravity;

		while (y < top + hitbox / 2.f) {
			y += time * gravity;
		}

		if (y == top + hitbox / 2.f) {
			isOnTop = true;
			dy = 0;
		}

		isOnGround = false;
	}

	void CheckCollision() override {
		for (int i = 0; i < solid.size(); i++) { //проходимся по объектам
			if (GetPlayerHitbox().intersects(solid[i].rect)) { //проверяем пересечение игрока с объектом
				//cout << x + 16 << endl;
				if (dy > 0 && y - hitbox / 2.f <= solid[i].rect.top) {
					ground = solid[i].rect.top;
					dy = 0;
					isOnGround = true;
				}

				if (dy < 0 && y + hitbox / 2.f >= solid[i].rect.top + solid[i].rect.height) {
					top = solid[i].rect.top + solid[i].rect.height;
					dy = 0;
					isOnTop = true;
				}
			}

			if (x - hitbox / 2.f > solid[i].rect.left + solid[i].rect.width - 0.3
				&&
				x - hitbox / 2.f < solid[i].rect.left + solid[i].rect.width + 0.3) {

				if (isOnGround) {
					isOnGround = false;
					ground = startGround;
				}
				if (isOnTop) {
					isOnTop = false;
					top = startTop;
				}

			}

		}

		for (int i = 0; i < fatal.size(); i++) {
			if (GetPlayerHitbox().intersects(fatal[i].rect))
				isDead = true;
		}
	}

	void Respawn() override {
		x = startPos;
		y = (ground = startGround) - 16.f;
		angle = 0;
	}
};

class Level {
public:
	Level(const char* musicPath, const char* mapPath, int levelLength, int mapHeight) {
		this->musicPath = musicPath;
		this->mapPath = mapPath;
		this->levelLength = levelLength;
		this->mapHeight = mapHeight;
	}

	void LevelProcess(View& view, RenderWindow& window, TmxLevel& levelMap) {
		levelMap.LoadFromFile(mapPath);

		TmxObject playerPos = levelMap.GetFirstObject("player");

		Cube cube(levelMap, mapHeight, playerPos.rect.left, playerPos.rect.top);

		SetMusic();

		float time;
		Clock dtClock;

		while (window.isOpen()) {
			CheckIfClosingWindow(window);

			TimeUpdate(time, dtClock);
			Update(window, view, &cube, time);

			Display(view, window, levelMap, &cube);
		}
	}

private:
	const char* musicPath;
	const char* mapPath;
	int levelLength, mapHeight;
	Music music;
	Map map;

	void SetMusic() {
		music.openFromFile(musicPath);
		music.play();
		music.setVolume(50.f);
	}

	void TimeUpdate(float& time, Clock& dtClock) {
		time = dtClock.getElapsedTime().asMicroseconds() / 800.f;
		dtClock.restart();
	}

	void CheckIfClosingWindow(RenderWindow& window) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
				music.stop();
				window.close();
			}
		}
	}

	void Update(RenderWindow& window, View& view, Player* player, float time) {
		view.move(player->getDx() * time, 0);

		if (player->getY() < 192 && player->getDy() != 0) {
			view.move(0, player->getDy() * time * 0.6);
		}
		map.getBackgroundSprite().move(player->getDx() * time * 0.9, 0);

		player->update(time);

		if (player->getIsDead()) {
			player->setIsDead(false);
			view.reset(FloatRect(0, 0, 854, 480));
			map.getBackgroundSprite().setPosition(0, 0);
			music.setPlayingOffset(music.getPlayingOffset() - music.getPlayingOffset());
		}
	}

	void Display(View& view, RenderWindow& window, TmxLevel& levelMap, Player* player) {
		window.setView(view);
		window.clear(Color::Blue);
		window.draw(map.getBackgroundSprite());
		levelMap.Draw(window);

		for (int i = 0; i < levelLength / 128; i++) {
			map.getGroundSprite().setPosition(i * 128, player->getStartGround());
			window.draw(map.getGroundSprite());
		}

		window.draw(player->getPlayerSprite());
		window.display();
	}
};

int main() {
	View view;

	int windowWidth = 854, windowHeight = 480;
	RenderWindow window(VideoMode(windowWidth, windowHeight), "Geometry Dash");

	view.setSize(windowWidth, windowHeight);
	view.setCenter(windowWidth / 2, windowHeight / 2);

	TmxLevel levelMap;

	Level StereoMadness("Resources/StereoMadness.ogg" ,"Resources/tiles/map2.tmx", 27264, 13 * 32);
	StereoMadness.LevelProcess(view, window, levelMap);
	return 0;
}
