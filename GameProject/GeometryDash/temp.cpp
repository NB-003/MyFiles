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
	Texture tBG, tBGS;
	Sprite BG, BGS;
	Map() {
		tBG.loadFromFile("Resources/BG.png"); /// Изменить путь
		BG.setTexture(tBG);

		BG.setColor(Color(63, 72, 204, 150));
		BG.setScale(1.5f, 1.5f);

		BG.setPosition(0, 0);
		BG.setOrigin(0, 1024 / 2);

		tBGS.loadFromFile("Resources/SBG.png"); /// Изменить путь
		BGS.setTexture(tBGS);
		BGS.setColor(Color(63, 72, 204));
		BGS.setPosition(0, 0);
	}
};

class Player {
public:
	Player(TmxLevel& lvl, int x_pos, int y_pos) {
		solid = lvl.GetAllObjects("solid");
		fatal = lvl.GetAllObjects("fatal");

		x = (startPos = x_pos);
		y = (ground = (startGround = y_pos)) - 16.f;
		dx = 0.245;
		dy = 0.f;
		angle = 0.f;
		hitbox = 32;
		isOnGround = true;
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

	virtual void update(float) = 0;

protected:
	std::vector<TmxObject> solid;
	std::vector<TmxObject> fatal;

	float x, y, dx, dy, angle;
	int hitbox, startGround, ground, startPos;
	bool isOnGround, isDead;

	Image image;
	Texture texture;
	Sprite sprite;

private:
	virtual void CheckCollision(float, float) = 0;
	virtual void Respawn() = 0;
};

class Cube : public Player {
public:
	Cube(TmxLevel& lvl, int x_pos, int y_pos) : Player(lvl, x_pos, y_pos) {
		image.loadFromFile("Resources/Cube001.png");

		texture.loadFromImage(image);

		sprite.setTexture(texture);
		sprite.setScale(0.1, 0.1);
		sprite.setPosition(x, y);
		sprite.setOrigin(310 / 2.f, 310 / 2.f);
		sprite.setColor(Color::Green);
	}

	void update(float time) override {

		CheckCollision(dx, dy);
		x += dx * time;

		if (!isOnGround) {
			dy += time * 0.0015;
			angle += time * 0.16;
			if (angle > 90)
				angle = 90;
		}

		y += dy * time;
		if (y > ground - hitbox / 2.f) {
			y = ground - hitbox / 2.f;
			dy = 0.f;
			isOnGround = true;
		}

		if (isOnGround)
			angle = 0;

		sprite.setRotation(angle);
		sprite.setPosition(x, y);
	}

private:
	FloatRect GetCubeHitbox() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x - hitbox / 2.f, y - hitbox / 2.f, hitbox, hitbox);//эта ф-ция нужна для проверки столкновений 
	}

	void CheckCollision(float dx, float dy) override {
		for (int i = 0; i < solid.size(); i++) { //проходимся по объектам
			if (GetCubeHitbox().intersects(solid[i].rect)) { //проверяем пересечение игрока с объектом
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
			if (GetCubeHitbox().intersects(fatal[i].rect))
				Respawn();
		}
	}

	void Respawn() override {
		isDead = true;
		x = startPos;
		y = (ground = startGround);
		angle = 0;
	}
};

class Level {
public:
	Level(const char* musicPath, const char* mapPath, int levelLength) {
		this->musicPath = musicPath;
		this->mapPath = mapPath;
		this->levelLength = levelLength;
	}

	void LevelProcess(View& view, RenderWindow& window, TmxLevel& levelMap) {
		levelMap.LoadFromFile(mapPath);

		TmxObject playerPos = levelMap.GetFirstObject("player");

		Cube cube(levelMap, playerPos.rect.left, playerPos.rect.top);

		SetMusic();

		float time;
		Clock dtClock;

		while (window.isOpen()) {
			CheckIfCloseWindow(window);

			TimeUpdate(time, dtClock);
			Update(view, &cube, time);

			Display(view, window, levelMap, &cube);
		}
	}

private:
	const char* musicPath;
	const char* mapPath;
	int levelLength;
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

	void CheckIfCloseWindow(RenderWindow& window) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
				music.stop();
				window.close();
			}
		}
	}

	void Update(View& view, Player* player, float time) {
		if ((Keyboard::isKeyPressed(Keyboard::Up)) && (player->getIsOnGround() == true)) {
			player->setDy(-0.45);
			player->setIsOnGround(false);
		}
		view.move(player->getDx() * time, 0);

		if (player->getY() < 192 && player->getDy() != 0) {
			view.move(0, player->getDy() * time);
		}
		map.BG.move(player->getDx() * time * 0.9, 0);

		player->update(time);
		if (player->getIsDead()) {
			player->setIsDead(false);
			view.reset(FloatRect(0, 0, 854, 480));
			map.BG.setPosition(0, 0);
			music.setPlayingOffset(music.getPlayingOffset() - music.getPlayingOffset());
		}
	}

	void Display(View& view, RenderWindow& window, TmxLevel& levelMap, Player* player) {
		window.setView(view);
		window.clear(Color::Blue);
		window.draw(map.BG);
		levelMap.Draw(window);

		for (int i = 0; i < levelLength / 128; i++) {
			map.BGS.setPosition(i * 128, 352);
			window.draw(map.BGS);
		}

		window.draw(player->getPlayerSprite());
		window.display();
	}
};

int main() {
	View view;
	RenderWindow window(VideoMode(854, 480), "Geometry Dash");

	view.setSize(854.f, 480.f);
	view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);

	TmxLevel levelMap;

	Level StereoMadness("Resources/StereoMadness.ogg" ,"Resources/tiles/map2.tmx", 27264);
	StereoMadness.LevelProcess(view, window, levelMap);
	return 0;
}
