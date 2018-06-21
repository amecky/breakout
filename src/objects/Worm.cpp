#include "Worm.h"
#include "..\lib\math.h"
#include <SpriteBatchBuffer.h>
#include "..\utils\TweakValue.h"

static ds::vec2 add_radial(const ds::vec2& pos, float angle, float radius) {
	return pos + radius * ds::vec2(cosf(angle), sinf(angle));
}

ds::matrix srt(float scale, float rotation, float x, float y) {
	float ca = cos(rotation);
	float sa = sin(rotation);
	ds::matrix m = ds::matIdentity();
	m._11 = scale * ca;
	m._12 = scale * sa;
	m._13 = x;
	m._21 = scale * -sa;
	m._22 = scale * ca;
	m._23 = y;
	return m;
}

namespace enemies {

	// --------------------------------------------------------------
	// render
	// --------------------------------------------------------------
	void render(const EnemyDesc& desc, SpriteBatchBuffer * sprites) {
		ds::vec2 p = desc.transformation.pos;		
		for (int i = 0; i < desc.num; ++i) {
			const Segment& s = desc.segments[i];
			ds::matrix world = srt(desc.transformation.scale.x, desc.transformation.rotation, s.pos.x,s.pos.y);
			ds::vec3 tmp = world * ds::vec3(s.pos.x,s.pos.y,0.0f);

			ds::vec2 scale = _TV(ds::vec2(1.4f, 1.4f));

			sprites->add(p + ds::vec2(tmp.x,tmp.y), _TV(ds::vec4(480, 0, 48, 48)), scale, s.rotation + desc.transformation.rotation, s.color);
		}
	}

	// --------------------------------------------------------------
	// build shape
	// --------------------------------------------------------------
	void build_shape(EnemyDesc* desc, ds::vec2* points, int num) {
		desc->num = 0;
		desc->transformation.pos = ds::vec2(0.0f);
		desc->transformation.previous = desc->transformation.pos;
		desc->transformation.rotation = 0.0f;
		desc->transformation.scale = ds::vec2(1.0f);
		desc->transformation.timer = 0.0f;
		for (int i = 0; i < num; ++i) {
			desc->segments[desc->num++] = { points[i], 0.0f, ds::vec2(0.5f, 0.5f), ds::Color(49, 237, 191, 255), -1 };
		}
	}

	// --------------------------------------------------------------
	// build shape and connect segments
	// --------------------------------------------------------------
	void build_shape(EnemyDesc* desc, int num) {
		desc->num = 0;
		desc->transformation.pos = ds::vec2(0.0f);
		desc->transformation.previous = desc->transformation.pos;
		desc->transformation.rotation = 0.0f;
		desc->transformation.scale = ds::vec2(1.0f);
		desc->transformation.timer = 0.0f;
		for (int i = 0; i < num; ++i) {
			float angle = ds::TWO_PI * static_cast<float>(i) / static_cast<float>(num);
			float size = _TV(40.0f);
			ds::vec2 np(cosf(angle), sinf(angle));
			np *= size;
			desc->segments[desc->num++] = { np, 0.0f, ds::vec2(0.5f, 0.5f), ds::Color(49, 237, 191, 255), -1 };
		}

		for (int i = 0; i < num; ++i) {
			const Segment& f = desc->segments[i];
			int next = i + 1;
			if (next == num) {
				next = 0;
			}
			const Segment& s = desc->segments[next];
			ds::vec2 np = (f.pos + s.pos) * 0.5f;
			ds::vec2 d = s.pos - f.pos;
			float l = length(d);
			float sc = l / 48.0f;
			sc += sc * 0.2f;
			float angle = math::get_rotation(d);
			desc->segments[desc->num++] = { np, angle, ds::vec2(sc, 0.25f), ds::Color(255, 32, 91, 255), -1 };
		}
	}

	// --------------------------------------------------------------
	// connect segments by adding lines
	// --------------------------------------------------------------
	void connect_segments(EnemyDesc* desc, int num, bool connectFirst) {
		int total = num - 1;
		if (connectFirst) {
			total = num;
		}
		for (int i = 0; i < total; ++i) {
			const Segment& f = desc->segments[i];
			int next = i + 1;
			if (next == num) {
				next = 0;
			}
			const Segment& s = desc->segments[next];
			ds::vec2 np = (f.pos + s.pos) * 0.5f;
			ds::vec2 d = s.pos - f.pos;
			float l = length(d);
			float sc = l / 48.0f;
			sc += sc * 0.2f;
			float angle = math::get_rotation(d);
			desc->segments[desc->num++] = { np, angle, ds::vec2(sc, 0.25f), ds::Color(255, 32, 91, 255), -1 };
		}
	}

	// --------------------------------------------------------------
	// rebuild connections
	// --------------------------------------------------------------
	void rebuild_segments(EnemyDesc* desc, int num, bool connectFirst) {
		int total = num - 1;
		if (connectFirst) {
			total = num;
		}
		for (int i = 0; i < total; ++i) {
			const Segment& f = desc->segments[i];
			int next = i + 1;
			if (next == num) {
				next = 0;
			}
			const Segment& s = desc->segments[next];
			ds::vec2 np = (f.pos + s.pos) * 0.5f;
			ds::vec2 d = s.pos - f.pos;
			float l = length(d);
			float sc = l / 48.0f;
			sc += sc * 0.2f;
			float angle = math::get_rotation(d);
			desc->segments[num + i] = { np, angle, ds::vec2(sc, 0.25f), ds::Color(255, 32, 91, 255), -1 };
		}
	}

}

Worm::Worm() {
	ds::vec2 pos = ds::vec2(512, 384);
	for (int i = 0; i < NUM_SEGMENTS; ++i) {
		_segments[i].pos = pos;
	}
}

void Worm::move(const Transformation& movement, float dt, float minDist, float relaxation) {
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
