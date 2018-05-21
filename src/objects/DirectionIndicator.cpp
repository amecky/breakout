#include "DirectionIndicator.h"
#include "..\lib\math.h"

void DirectionIndicator::reset() {
	_position = ds::vec2(512, 120);
}

void DirectionIndicator::tick(float dt) {
	if (_active) {
		ds::vec2 mp = ds::getMousePosition();
		ds::vec2 diff = mp - _position;
		_rotation = math::get_rotation(diff);
		/*
		if (_rotation < ds::PI * 0.25f) {
			_rotation = ds::PI * 0.25f;
		}
		if (_rotation > ds::PI * 0.75f) {
			_rotation = ds::PI * 0.75f;
		}
		*/
	}
}
