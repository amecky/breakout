#pragma once
#include <diesel.h>

class DirectionIndicator {

public:
	void reset();
	void tick(float dt);
	const ds::vec2& getPosition() const {
		return _position;
	}
	void setPosition(const ds::vec2& pos) {
		_position = pos;
	}
	float getRotation() const {
		return _rotation;
	}
private:
	bool _active;
	ds::vec2 _position;
	float _rotation;
};