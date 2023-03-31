#pragma once

#include "../Player.h"

class Ship : public Player {
public:
	Ship(TmxLevel&, int, int, int);

	void update(float) override;

private:

	const float gravity = 0.00025;
	int startTop, top;
	bool isOnTop = false;

	void Move(float) override;

	void CheckCollision() override;
};