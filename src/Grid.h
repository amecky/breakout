#pragma once
#include "GameContext.h"

struct Brick {
	int energy;
	int type;
};

class Grid {

public:
	Grid(GameContext* context);
	~Grid();
	void buildLevel(int level);
	int handleHit(ds::SID sid);
	void clear();
private:
	GameContext* _context;
	ds::World* _world;
	ds::Vector2fPath _scalePath;
};

