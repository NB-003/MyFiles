#include "Player.h"

Player::Player(TmxLevel& levelMap, int mapHeight, int x_pos, int y_pos) {
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

float Player::getX() { return x; };
float Player::getY() { return y; };

float Player::getDx() { return dx; };
void Player::setDx(float dx) { this->dx = dx; };

float Player::getDy() { return dy; };
void Player::setDy(float dy) { this->dy = dy; };

bool Player::getIsOnGround() { return isOnGround; }
void Player::setIsOnGround(bool isOnGround) { this->isOnGround = isOnGround; };

bool Player::getIsDead() { return isDead; };
void Player::setIsDead(bool isDead) { this->isDead = isDead; };

sf::Sprite Player::getPlayerSprite() { return playerSprite; }

sf::FloatRect Player::GetPlayerHitbox() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
	return sf::FloatRect(x - hitbox / 2.f, y - hitbox / 2.f, hitbox, hitbox);//эта ф-ция нужна для проверки столкновений 
}

bool Player::CheckMoveRequest() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Space)
		   ||
		   sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
		   ||
		   sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

void Player::Respawn() {
	x = startPos;
	y = (ground = startGround) - 16.f;
	angle = 0;
}