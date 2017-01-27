#include "Paddle.h"
#include <core\math\GameMath.h>
#include <base\InputSystem.h>
#include "Constants.h"

Paddle::Paddle(GameContext* ctx) : ds::GameObject("Paddle"), _context(ctx), _velocity(300.0f) {
}


Paddle::~Paddle() {
}

// ------------------------------------------------------
// activate player - create new entity
// ------------------------------------------------------
void Paddle::onActivation() {
	_id = _context->world->create(v2(512, 80), math::buildTexture(200, 80, 102, 30), OT_PLAYER);
	_context->world->attachName(_id, "Paddle");
	_context->world->attachCollider(_id, ds::PST_QUAD, v2(100.f, 30.0));
	_previous = v2(512, 384);
	_velocity = 250.0f;
}

// -------------------------------------------------------
// move player
// -------------------------------------------------------
void Paddle::tick(float dt) {
	ZoneTracker u2("Paddle::movePlayer");
	v2 vel = v2(0.0f);
	if (ds::input::getKeyState('A')) {
		vel.x -= 1.0f;
	}
	if (ds::input::getKeyState('D')) {
		vel.x += 1.0f;
	}
	v2 wp = _context->world->getPosition(_id).xy();
	v2 pos = wp;
	pos += vel * _velocity * dt;
	if ( pos.x > 150 && pos.x < 880) {
		_context->world->setPosition(_id, pos);		
	}
}

// ------------------------------------------------------
// kill player
// ------------------------------------------------------
void Paddle::onDeactivation() {
	v2 wp = _context->world->getPosition(_id).xy();
	_context->world->remove(_id);
}


