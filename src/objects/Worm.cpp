#include "Worm.h"
#include "..\lib\math.h"
#include <SpriteBatchBuffer.h>

static ds::vec2 add_radial(const ds::vec2& pos, float angle, float radius) {
	return pos + radius * ds::vec2(cosf(angle), sinf(angle));
}

Worm::Worm() {
	_pos = ds::vec2(512, 384);
	for (int i = 0; i < NUM_SEGMENTS; ++i) {
		_segments[i].pos = _pos;
	}
	_timer = 0.0f;
}

void Worm::move(float dt, float minDist, float relaxation) {
	_timer += dt;
	_previous = _pos;
	_pos.y = (15.0f * cos(_timer * -6.0f)) + (240.0f + (180.0f * sin(_timer * 1.3f)));
	_pos.x = (15.0f * sin(_timer * -6.0f)) + (320.0f + (200.0f * cos(_timer / 1.5f)));

	_segments[0].pos = _pos;
	_segments[0].rotation = 0.0f;
	for (int i = 1; i < NUM_SEGMENTS; ++i) {
		ds::vec2 d = _segments[i - 1].pos - _segments[i].pos;
		if (sqr_length(d) > minDist * minDist) {
			_segments[i].pos = _segments[i].pos + (d * relaxation);
			//_segments[i].pos += add_radial(ds::vec2(1.0f),sin(_timer),4.0f);
			_segments[i].rotation = math::get_rotation(_segments[i - 1].pos - _segments[i].pos);
		}
	}
}

void Worm::render(SpriteBatchBuffer * sprites) {
	sprites->add(_segments[0].pos, ds::vec4(480, 0, 48, 48), ds::vec2(2.2f), _segments[0].rotation, ds::Color(192, 192, 192, 96));
	sprites->add(_segments[0].pos, ds::vec4(480, 0, 48, 48), ds::vec2(1.8f), _segments[0].rotation, ds::Color(192, 0, 192, 192));
	float a = math::get_rotation(_pos - _previous);
	ds::vec2 p = add_radial(_segments[0].pos, a + math::deg2rad(45.0f), 8.0f);
	sprites->add(p, ds::vec4(480, 0, 48, 48), ds::vec2(0.4f), a, ds::Color(192, 192, 192, 255));
	p = add_radial(_segments[0].pos, a - math::deg2rad(45.0f), 8.0f);
	sprites->add(p, ds::vec4(480, 0, 48, 48), ds::vec2(0.4f), a, ds::Color(192, 192, 192, 255));

	for (int i = 1; i < NUM_SEGMENTS; ++i) {
		sprites->add(_segments[i].pos, ds::vec4(480, 0, 48, 48), ds::vec2(1.8f), _segments[i].rotation, ds::Color(192, 192, 0, 96));
		float s = 0.25f + sin(static_cast<float>(i) / static_cast<float>(NUM_SEGMENTS) * ds::PI) * 1.2f;
		sprites->add(_segments[i].pos, ds::vec4(480, 0, 48, 48), ds::vec2(0.2f, s), _segments[i].rotation, ds::Color(0, 192, 0, 255));
		sprites->add(_segments[i].pos, ds::vec4(480, 0, 48, 48), ds::vec2(0.6f), _segments[i].rotation, ds::Color(192, 128, 0, 255));
	}
}
