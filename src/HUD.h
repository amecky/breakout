#pragma once
#include <core\base\GameObject.h>
#include "GameContext.h"

class HUD : public ds::GameObject {

public:
	HUD(GameContext* ctx);
	~HUD();
	void tick(float dt);
	void onActivation();
	void onDeactivation();	
	void reset();
	void setLevel(int lvl);
private:
	GameContext* _context;
	int _points;
	int _level;
	ID _pointsIds[6];
	ID _levelIds[2];
};

