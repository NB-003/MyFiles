#pragma once

#include "../Player.h"

class Cube : public Player {
public:
	Cube(TmxLevel&, int, int, int);
	Cube(TmxLevel&, int);

	void update(float) override;

private:
	void Move(float) override;
	void CheckCollision() override;
};