#include "Worm.h"
#include "..\lib\math.h"
#include <SpriteBatchBuffer.h>

static ds::vec2 add_radial(const ds::vec2& pos, float angle, float radius) {
	return pos + radius * ds::vec2(cosf(angle), sinf(angle));
}

Worm::Worm() {
	ds::vec2 pos = ds::vec2(512, 384);
	for (int i = 0; i < NUM_SEGMENTS; ++i) {
		_segments[i].pos = pos;
	}
	//_timer = 0.0f;
}

void Worm::move(const Transformation& movement, float dt, float minDist, float relaxation) {
	//_timer += dt;
	//_previous = _pos;
	//_pos = movement.pos;
	//_pos.y = (15.0f * cos(_timer * -6.0f)) + (240.0f + (180.0f * sin(_timer * 1.3f)));
	//_pos.x = (15.0f * sin(_timer * -6.0f)) + (320.0f + (200.0f * cos(_timer / 1.5f)));

	_segments[0].pos = movement.pos;
	_segments[0].rotation = 0.0f;
	for (int i = 1; i < NUM_SEGMENTS; ++i) {
		ds::vec2 d = _segments[i - 1].pos - _segments[i].pos;
		if (sqr_length(d) > minDist * minDist) {
			_segments[i].pos = _segments[i].pos + (d * relaxation);
			_segments[i].rotation = math::get_rotation(_segments[i - 1].pos - _segments[i].pos);
		}
	}
}

void Worm::render(const Transformation& movement, SpriteBatchBuffer * sprites) {
	sprites->add(_segments[0].pos, ds::vec4(480, 0, 48, 48), ds::vec2(2.2f), _segments[0].rotation, ds::Color(192, 192, 192, 96));
	sprites->add(_segments[0].pos, ds::vec4(480, 0, 48, 48), ds::vec2(1.8f), _segments[0].rotation, ds::Color(192, 0, 192, 192));
	float a = movement.rotation;
	ds::vec2 p = add_radial(_segments[0].pos, a + math::deg2rad(45.0f), 8.0f);
	sprites->add(p, ds::vec4(480, 0, 48, 48), ds::vec2(0.4f), a, ds::Color(192, 192, 192, 255));
	p = add_radial(_segments[0].pos, a - math::deg2rad(45.0f), 8.0f);
	sprites->add(p, ds::vec4(480, 0, 48, 48), ds::vec2(0.4f), a, ds::Color(192, 192, 192, 255));

	for (int i = 1; i < NUM_SEGMENTS; ++i) {
		sprites->add(_segments[i].pos, ds::vec4(480, 0, 48, 48), ds::vec2(1.8f), _segments[i].rotation, ds::Color(192, 192, 0, 96));
		float s = 0.25f + cos(static_cast<float>(i) / static_cast<float>(NUM_SEGMENTS) * ds::PI) * 1.2f;
		if (s < 0.0f) {
			s *= -1.0f;
		}
		sprites->add(_segments[i].pos, ds::vec4(480, 0, 48, 48), ds::vec2(0.2f, s), _segments[i].rotation, ds::Color(0, 192, 0, 255));
		sprites->add(_segments[i].pos, ds::vec4(480, 0, 48, 48), ds::vec2(0.6f), _segments[i].rotation, ds::Color(192, 128, 0, 255));
	}
}
/*
void Ship::render(const Transformation & movement, SpriteBatchBuffer * sprites) {
*/
Ship::Ship() {
	ds::vec2 p(0.0f);
	float start = ds::PI * 0.25f;
	float step = ds::PI / 7.0f * 1.5f;
	float angle = start;
	for (int i = 0; i < 8; ++i) {
		ds::vec2 np = add_radial(p, angle, 25.0f);
		_segments[_num++] = { np, 0.0f, ds::vec2(1.0f), ds::Color(12, 198, 215, 192), -1 };
		_segments[_num++] = { np, angle, ds::vec2(0.5f, 0.2f), ds::Color(49, 237, 191, 255), -1 };
		_segments[_num++] = { np, angle, ds::vec2(0.6f, 0.6f), ds::Color(192, 0, 0, 255), -1 };
		angle += step;
	}
}


void Enemy::render(const Transformation & movement, SpriteBatchBuffer * sprites) {
	ds::vec2 p = movement.pos;
	for (int i = 0; i < _num; ++i) {
		const Segment& s = _segments[i];
		sprites->add(p + s.pos, ds::vec4(480, 0, 48, 48), s.scale, s.rotation, s.color);
	}
}

int Enemy::add() {
	if (_num < 32) {
		Segment& s = _segments[_num++];
		s.pos = ds::vec2(0.0f);
		s.color = ds::Color(255, 255, 255, 255);
		s.rotation = 0.0f;
		s.scale = ds::vec2(1.0f);
		s.timer = 0.0f;
		return _num - 1;
	}
	return -1;
}
int Enemy::remove(int index) {
	if (_num > 0) {
		_segments[index] = _segments[_num - 1];
		--_num;
		return _num;
	}
	return -1;
}