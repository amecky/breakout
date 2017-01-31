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
	void setLevel(int lvl);
	void addScore(int pts);
	void decrementLive();
private:
	GameContext* _context;
	int _points;
	int _current;
	float _timer;
	int _level;
	int _lives;
	ID _pointsIds[6];
	ID _levelIds[2];
	ID _livesId;
};

