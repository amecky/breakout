#include "Messages.h"
#include <core\math\GameMath.h>
#include "Constants.h"

Messages::Messages(GameContext* ctx) : ds::GameObject("Messages"), _context(ctx) {
	_level = math::buildTexture(440, 0, 220, 52);
	int top = 440;
	int h = 50;
	int p[] = { 220, 285, 325, 386, 444, 508, 568, 628, 688, 749 };
	int w[] = {63, 37, 58, 55, 59, 56, 55, 56, 57, 57};

	for (int i = 0; i < 10; ++i) {
		_numbers[i] = math::buildTexture(top, p[i], w[i], h);
	}
}


Messages::~Messages() {
}

// ------------------------------------------------------
// 
// ------------------------------------------------------
void Messages::onActivation() {
	for (uint32_t i = 0; i < _messages.size(); ++i) {
		Message& m = _messages[i];
		m.id = _context->world->create(m.pos, m.texture, 100);
	}
}

// -------------------------------------------------------
// 
// -------------------------------------------------------
void Messages::tick(float dt) {	
}

// ------------------------------------------------------
// 
// ------------------------------------------------------
void Messages::onDeactivation() {
	for (uint32_t i = 0; i < _messages.size(); ++i) {
		const Message& m = _messages[i];
		_context->world->remove(m.id);
	}
	_messages.clear();
}

void Messages::showLevel(int level) {
	add(v2(512, 440), _level);
	int f = 0;
	if (level < 10) {
		f = 0;
	}
	else {
		f = level / 10;
	}
	int s = level % 10;
	ds::Texture t1 = _numbers[f];
	ds::Texture t2 = _numbers[s];
	int w = t1.rect.width() + t2.rect.width() + 10;
	int x = (1024 - w) / 2;
	add(v2(x, 350), t1);
	x += t1.rect.width() + 10;
	add(v2(x, 350), t2);
}

void Messages::add(const v2& pos, const ds::Texture& t) {
	Message m;
	m.color = ds::Color::WHITE;
	m.id = INVALID_ID;// _context->world->create(pos, t, 100);
	m.timer = 0.0f;
	m.ttl = 0.0f;
	m.pos = pos;
	m.texture = t;
	_messages.push_back(m);
}

void Messages::reset() {
	_messages.clear();
}
