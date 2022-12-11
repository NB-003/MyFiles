#include "Ship.h"

Ship::Ship(TmxLevel& levelMap, int mapHeight, int x_pos, int y_pos) : Player(levelMap, mapHeight, x_pos, y_pos) {
	top = (startTop = startGround - 288);

	playerTexture.loadFromFile("Resources/Ship01.png");

	playerSprite.setTexture(playerTexture);
	playerSprite.setScale(32 / 288.f, 32 / 288.f);
	playerSprite.setPosition(x, y);
	playerSprite.setOrigin(382 / 2.f, 288 / 2.f);
	playerSprite.setColor(sf::Color::Green);
}

void Ship::update(float time) {
	if (CheckMoveRequest())
		Move(time);

	CheckCollision();

	if (isDead)
		Respawn();

	x += dx * time;

	if (!CheckMoveRequest()) {
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

	playerSprite.setRotation(angle);
	playerSprite.setPosition(x, y);
}

void Ship::Move(float time) {
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

void Ship::CheckCollision() {
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