#pragma once
#include <diesel.h>
#include <ds_intersection.h>

class Ball {

public:
	Ball() {
		_aabBox.dim = ds::vec2(18, 18);
	}
	void reset(float x = 512.0f);
	void reset(const ds::vec2& pos);
	void tick(float dt);
	const ds::vec2& getPosition() const {
		return _position;
	}
	void setPosition(const ds::vec2& pos) {
		_position = pos;
	}
	bool isSticky() const {
		return _sticky;
	}
	void startMoving(float angle);
	void setDirection(float angle);
	float getDirection() const {
		return _direction;
	}
	void stopMoving() {
		_moving = false;
	}
	bool isMoving() const {
		return _moving;
	}
	const ds::vec2& getVelocity() const {
		return _velocity;
	}
	const ds::vec2& getPrevious() const {
		return _previous;
	}
	const phAABBox getBox() const {
		return _aabBox;
	}
private:
	phAABBox _aabBox;
	ds::vec2 _position;
	ds::vec2 _previous;
	bool _sticky;
	ds::vec2 _velocity;
	float _direction;
	bool _moving;
	float timer;
	bool wiggling;
};