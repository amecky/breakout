#pragma once
#include <core\base\GameObject.h>
#include "GameContext.h"
#include "LevelData.h"

struct Brick {
	int energy;
};

class Bricks : public ds::GameObject {

public:
	Bricks(GameContext* ctx);
	~Bricks();
	void tick(float dt);
	void onActivation();
	void onDeactivation();
	bool handleCollision(ID id);
	int setLevel(int level);
private:
	GameContext* _context;
	LevelData _data;
	ds::Texture _textures[4];
	ds::V3Path _path;
	int _level;
	int _numBricks;
	int _movedBricks;
};
