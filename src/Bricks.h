#pragma once
#include <core\base\GameObject.h>
#include "GameContext.h"

struct Brick {
	int energy;
};

const int GRID_X = 10;
const int GRID_Y = 8;
const int LEVELS = 3;

class Bricks : public ds::GameObject {

public:
	Bricks(GameContext* ctx);
	~Bricks();
	void tick(float dt);
	void onActivation();
	void onDeactivation();
private:
	GameContext* _context;
	int _data[GRID_X * GRID_Y * LEVELS];
};
