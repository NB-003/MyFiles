#include "Cube.h"

Cube::Cube(TmxLevel& levelMap, int mapHeight, int x_pos, int y_pos) : Player(levelMap, mapHeight, x_pos, y_pos) {
	playerTexture.loadFromFile("Resources/Cube001.png");

	playerSprite.setTexture(playerTexture);
	playerSprite.setScale(32 / 310.f, 32 / 310.f);
	playerSprite.setPosition(x, y);
	playerSprite.setOrigin(310 / 2.f, 310 / 2.f);
	playerSprite.setColor(sf::Color::Green);
}

Cube::Cube(TmxLevel& levelMap, int mapHeight) : Cube(levelMap, mapHeight, 288, mapHeight) {}

void Cube::update(float time) {
	if (CheckMoveRequest())
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

	playerSprite.setRotation(angle);
	playerSprite.setPosition(x, y);
}

void Cube::Move(float) {
	if (isOnGround == true) {
		dy = -0.45;
		isOnGround = false;
	}
}

void Cube::CheckCollision() {
	for (int i = 0; i < solid.size(); i++) { // Going through all objects with type of "solid"
		if (GetPlayerHitbox().intersects(solid[i].rect)) { // Checking collision with this type of object
			if (dy > 0) {
				ground = solid[i].rect.top; // Cube stands on this object if cube was higher
				dy = 0;
				isOnGround = true;
			}
		}
		if (x - hitbox / 2.f > solid[i].rect.left + solid[i].rect.width - 0.6
			&&
			x - hitbox / 2.f < solid[i].rect.left + solid[i].rect.width + 0.6) {

			isOnGround = false; // Cube falls if it is on edge of the object
			ground = startGround;

		}

	}

	for (int i = 0; i < fatal.size(); i++) { // Going through all objects with type of "fatal"
		if (GetPlayerHitbox().intersects(fatal[i].rect))
			isDead = true; // If cube hits this object, he dies and respawns at the start
	}
}