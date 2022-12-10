#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include "TmxLevel.h"

using namespace sf;
using namespace std;

class Player {
public:
	Player(TmxLevel& levelMap, int mapHeight, int x_pos, int y_pos) {
		solid = levelMap.GetAllObjects("solid");
		fatal = levelMap.GetAllObjects("fatal");

		x = x_pos;
		y = y_pos - 16.f;
		ground = (startGround = mapHeight);
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

	virtual void update(float) = 0;

	Sprite getPlayerSprite() {
		return sprite;
	}

protected:
	std::vector<TmxObject> solid, fatal;

	Image image;
	Texture texture;
	Sprite sprite;

	float x, y, dx, dy, angle;
	int hitbox = 32, startPos = 192, startGround, ground;
	bool isOnGround, isDead = false;

	FloatRect GetPlayerHitbox() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x - hitbox / 2.f, y - hitbox / 2.f, hitbox, hitbox);//эта ф-ция нужна для проверки столкновений 
	}

	void Respawn() {
		x = startPos;
		y = (ground = startGround) - 16.f;
		angle = 0;
	}

private:
	virtual void Move(float) = 0;
	virtual void CheckCollision() = 0;
};

class Cube : public Player {
public:
	Cube(TmxLevel& levelMap, int mapHeight, int x_pos, int y_pos) : Player(levelMap, mapHeight, x_pos, y_pos) {
		image.loadFromFile("Resources/Cube001.png");
		texture.loadFromImage(image);

		sprite.setTexture(texture);
		sprite.setScale(32 / 310.f, 32 / 310.f);
		sprite.setPosition(x, y);
		sprite.setOrigin(310 / 2.f, 310 / 2.f);
		sprite.setColor(Color::Green);
	}

	Cube(TmxLevel& levelMap, int mapHeight) : Cube(levelMap, mapHeight, 192, mapHeight) {}

	void update(float time) override {
		if (Keyboard::isKeyPressed(Keyboard::Up))
			Move(time);

		CheckCollision();

		if (isDead)
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
};

class Ship : public Player {
public:
	Ship(TmxLevel& levelMap, int mapHeight, int x_pos, int y_pos) : Player(levelMap, mapHeight, x_pos, y_pos) {
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
			if (dy < 0)
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
};

class Level {
public:
	Level(const char* mapPath, const char* musicPath, int levelLength, int mapHeight) {
		levelMap.LoadFromFile(mapPath);

		cubePortal = levelMap.GetAllObjects("cubePortal");
		shipPortal = levelMap.GetAllObjects("shipPortal");

		this->levelLength = levelLength;
		this->mapHeight = mapHeight;

		SetMusic(musicPath);

		player = new Cube(levelMap, mapHeight);
		mode = CUBE;
	}

	void runLevelProcess(View& view, RenderWindow& window) {
		float time;
		Clock dtClock;

		while (window.isOpen()) {
			CheckWindowCloseRequest(window);

			TimeUpdate(time, dtClock);
			CheckModeChangeRequest();
			Update(window, view, time);

			if (player->getIsDead()) {
				Respawn(view);
			}

			Display(view, window);
		}
	}

	~Level() {
		delete player;
	}

private:
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
	} map;

	TmxLevel levelMap;
	std::vector<TmxObject> cubePortal, shipPortal;

	int levelLength, mapHeight;
	Music music;

	Player* player;

	enum Mode {
		CUBE,
		SHIP
	} mode;

	void SetMusic(const char* musicPath) {
		music.openFromFile(musicPath);
		music.play();
		music.setVolume(50.f);
	}

	void TimeUpdate(float& time, Clock& dtClock) {
		time = dtClock.getElapsedTime().asMicroseconds() / 800.f;
		dtClock.restart();
	}

	void CheckWindowCloseRequest(RenderWindow& window) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
				music.stop();
				window.close();
			}
		}
	}

	void CheckModeChangeRequest() {
		float x = player->getX();
		float y = player->getY();

		for (int i = 0; i < cubePortal.size(); i++) {
			if (FloatRect(x - 16, y - 16, 32, 32).intersects(cubePortal[i].rect) && mode != CUBE) {
				delete player;
				mode = CUBE;
				player = new Cube(levelMap, mapHeight, x, y + 16);
			}
		}

		for (int i = 0; i < shipPortal.size(); i++) {
			if (FloatRect(x - 16, y - 16, 32, 32).intersects(shipPortal[i].rect) && mode != SHIP) {
				delete player;
				mode = SHIP;
				player = new Ship(levelMap, mapHeight, x, y + 16);
			}
		}
	}

	void Update(RenderWindow& window, View& view, float time) {
		view.move(player->getDx() * time, 0);

		if (player->getY() < mapHeight - 192 && player->getDy() != 0) {
			view.move(0, player->getDy() * time * 0.8);
		}
		map.getBackgroundSprite().move(player->getDx() * time * 0.9, 0);

		player->update(time);
	}

	void Respawn(View& view) {
		player->setIsDead(false);
		delete player;
		mode = CUBE;
		player = new Cube(levelMap, mapHeight);
		view.reset(FloatRect(0, mapHeight - 384, 854, 480));
		map.getBackgroundSprite().setPosition(0, 0);
		music.setPlayingOffset(music.getPlayingOffset() - music.getPlayingOffset());
	}

	void Display(View& view, RenderWindow& window) {
		window.setView(view);
		window.clear(Color::Blue);
		window.draw(map.getBackgroundSprite());
		levelMap.Draw(window);

		for (int i = 0; i < levelLength / 128; i++) {
			map.getGroundSprite().setPosition(i * 128, mapHeight);
			window.draw(map.getGroundSprite());
		}

		window.draw(player->getPlayerSprite());
		window.display();
	}
};

int main() {
	View view;

	int windowWidth = 854, windowHeight = 480, mapHeight = 18 * 32, levelLength = 852 * 32;
	RenderWindow window(VideoMode(windowWidth, windowHeight), "Geometry Dash");

	view.setSize(windowWidth, windowHeight);
	view.setCenter(windowWidth / 2.f, (2 * mapHeight - 288) / 2.f);

	TmxLevel levelMap;

	Level StereoMadness("Resources/tiles/map4.tmx", "Resources/StereoMadness.ogg", levelLength, mapHeight);
	StereoMadness.runLevelProcess(view, window);
	return 0;
}
