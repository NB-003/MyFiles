#include "Ship.h"

Ship::Ship(TmxLevel& levelMap, int mapHeight, int x_pos, int y_pos) : Player(levelMap, mapHeight, x_pos, y_pos) {
	top = (startTop = startGround - 384); // Upper limit

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
			dy += time * gravity; // Setting up movement for ship if button isn't pressed

		if (!isOnGround) {
			dy += time * gravity;
			angle += time * 0.12;
			if (angle > 60)
				angle = 60;
		}

		isOnTop = false;
	}

	y += dy * time;

	while (y > ground - hitbox / 2.f) { // It's here to not fly below ground
		y -= time * gravity;
	}
	if (y == ground - hitbox / 2.f) { // Setting up ground
		isOnGround = true;
		dy = 0;
	}

	if (isOnGround && angle > 0) // It's here to smoothly align the angle of ship
		angle -= time * 0.2;

	if (isOnTop && angle < 0) // And this too
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
		dy -= time * gravity; // Setting up movement for ship if button is pressed

	while (y < top + hitbox / 2.f) {
		y += time * gravity; // It's here to not fly above upper limit
	}

	if (y == top + hitbox / 2.f) { // Setting up upper limit
		isOnTop = true; 
		dy = 0;
	}

	isOnGround = false;
}

void Ship::CheckCollision() {
	for (int i = 0; i < solid.size(); i++) { 
		if (GetPlayerHitbox().intersects(solid[i].rect)) { 
			if (dy > 0 && y - hitbox / 2.f <= solid[i].rect.top) {
				ground = solid[i].rect.top; // Ship lies on this object if ship was higher
				dy = 0;
				isOnGround = true;
			}

			if (dy < 0 && y + hitbox / 2.f >= solid[i].rect.top + solid[i].rect.height) {
				top = solid[i].rect.top + solid[i].rect.height; // or leans on it if ship was lower
				dy = 0;
				isOnTop = true;
			}
		}

		if (x - hitbox / 2.f > solid[i].rect.left + solid[i].rect.width - 0.3
			&&
			x - hitbox / 2.f < solid[i].rect.left + solid[i].rect.width + 0.3) {

			if (isOnGround) {
				isOnGround = false; // Ship falls if it lies and is on edge of the object
				ground = startGround;
			}
			if (isOnTop) {
				isOnTop = false; // or gets up if it leans 
				top = startTop;
			}

		}

	}

	for (int i = 0; i < fatal.size(); i++) { // Same as for cube
		if (GetPlayerHitbox().intersects(fatal[i].rect))
			isDead = true;
	}
}