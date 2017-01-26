#pragma once
#include <core\base\GameObject.h>
#include "GameContext.h"

class Paddle : public ds::GameObject {

public:
	Paddle(GameContext* ctx);
	~Paddle();
	void tick(float dt);
	void onActivation();
	void onDeactivation();
	const ID getID() const {
		return _id;
	}
	void setVelocity(float v) {
		_velocity = v;
	}
private:
	GameContext* _context;
	ID _id;
	v2 _previous;
	float _velocity;
};