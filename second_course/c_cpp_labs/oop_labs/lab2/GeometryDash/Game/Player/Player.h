#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "../LevelStructure/TmxLevel.h"

class Player {
public:
	Player(TmxLevel&, int, int, int);

	float getX();
	float getY();

	float getDx();
	void setDx(float);

	float getDy();
	void setDy(float);

	bool getIsOnGround();
	void setIsOnGround(bool);

	bool getIsDead();
	void setIsDead(bool);

	virtual void update(float) = 0;

	sf::Sprite getPlayerSprite();

	sf::FloatRect GetPlayerHitbox();

	virtual ~Player() = default;

protected:
	std::vector<TmxObject> solid, fatal;

	sf::Texture playerTexture;
	sf::Sprite playerSprite;

	float x, y, dx, dy, angle;
	const int hitbox = 32, startPos = 288;
	int startGround, ground;
	bool isOnGround, isDead = false;

	bool CheckMoveRequest();
	void Respawn();

private:
	virtual void Move(float) = 0;
	virtual void CheckCollision() = 0;
};