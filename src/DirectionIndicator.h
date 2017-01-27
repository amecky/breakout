#pragma once
#include <core\base\GameObject.h>
#include "GameContext.h"
#include "Paddle.h"

class DirectionIndicator : public ds::GameObject {

public:
	DirectionIndicator(GameContext* ctx);
	~DirectionIndicator();
	void tick(float dt);
	void onActivation();
	void onDeactivation();	
	float getAngle() const {
		return _angle;
	}
private:
	GameContext* _context;
	ID _id;
	Paddle* _paddle;
	float _angle;
};