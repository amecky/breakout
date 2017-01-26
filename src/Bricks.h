#pragma once
#include <core\base\GameObject.h>
#include "GameContext.h"

class Bricks : public ds::GameObject {

public:
	Bricks(GameContext* ctx);
	~Bricks();
	void tick(float dt);
	void onActivation();
	void onDeactivation();
private:
	GameContext* _context;

};
