#pragma once
#include <core\base\GameObject.h>
#include "GameContext.h"

class Ball : public ds::GameObject {

public:
	Ball(GameContext* ctx);
	~Ball();
	void tick(float dt);
	void onActivation();
	void onDeactivation();
	const ID getID() const {
		return _id;
	}
	void setVelocity(const v2& v) {
		_velocity = v;
	}
	void setMoving(bool mv) {
		_moving = mv;
	}
	void start();
	void bounceY();
	void bounceX();
private:
	GameContext* _context;
	ID _id;
	v2 _previous;
	v2 _velocity;
	bool _moving;
};
