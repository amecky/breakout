#include "Ball.h"

void Ball::reset(float x) {
	_position = ds::vec2(x, 120.0f);
	_previous = _position;
	_sticky = true;
	_moving = false;
}

void Ball::reset(const ds::vec2& p) {
	_position = p;
	_previous = _position;
	_sticky = true;
	_moving = false;
}

void Ball::tick(float dt) {
	if (!_sticky && _moving) {
		_previous = _position;
		_position += _velocity * dt;
		if (_position.x < 150.0f) {
			_position.x = 150.0f;
			_velocity.x *= -1.0f;
		}
		if (_position.x > 880.0f) {
			_position.x = 880.0f;
			_velocity.x *= -1.0f;
		}
		if (_position.y > 720.0f) {
			_position.y = 720.0f;
			_velocity.y *= -1.0f;
		}
		_aabBox.pos = _position;
	}
}

void Ball::startMoving(float angle) {
	_direction = angle;
	_velocity = ds::vec2(cosf(angle), sinf(angle)) * 400.0f;
	_sticky = false;
	_moving = true;
}

void Ball::setDirection(float angle) {
	_direction = angle;
	_velocity = ds::vec2(cosf(angle), sinf(angle)) * 400.0f;
}
