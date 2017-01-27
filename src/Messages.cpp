#include "Messages.h"
#include <core\math\GameMath.h>
#include "Constants.h"

Messages::Messages(GameContext* ctx) : ds::GameObject("Messages"), _context(ctx) {
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
