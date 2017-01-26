#include "Ball.h"
#include <core\math\GameMath.h>
#include <base\InputSystem.h>
#include "Constants.h"

Ball::Ball(GameContext* ctx) : ds::GameObject("Ball"), _context(ctx), _moving(false) {
}


Ball::~Ball() {
}

// ------------------------------------------------------
// activate player - create new entity
// ------------------------------------------------------
void Ball::onActivation() {
	_id = _context->world->create(v2(512, 120), math::buildTexture(60, 160, 18, 18), OT_BALL);
	_context->world->attachName(_id, "Ball");
	_context->world->attachCollider(_id, ds::PST_QUAD, v2(18.f, 18.0));
	_previous = v2(100, 384);
	_velocity = v2(100.0f, 100.0f);
}

// -------------------------------------------------------
// move ball
// -------------------------------------------------------
void Ball::tick(float dt) {
	ZoneTracker u2("Ball::move");
	if (_moving) {
		bool bouncing = false;
		v3 p = _context->world->getPosition(_id);
		p += v3(_velocity, 0.0f) * dt;
		if (p.x < 100.0f || p.x > 800.0f ) {
			_velocity.x *= -1.0f;
			bouncing = true;
		}
		if (p.y < 30.0f || p.y > 700.0f) {
			_velocity.y *= -1.0f;
			bouncing = true;
		}
		if (!bouncing) {
			_context->world->setPosition(_id, p);
		}
		// else -> squeeze ball
	}
}

// ------------------------------------------------------
// kill ball
// ------------------------------------------------------
void Ball::onDeactivation() {
	v2 wp = _context->world->getPosition(_id).xy();
	_context->world->remove(_id);
}

// ------------------------------------------------------
// start ball
// ------------------------------------------------------
void Ball::start() {
	v2 cp = ds::input::getMousePosition();
	v2 wp = _context->world->getPosition(_id).xy();
	float angle = 0.0f;
	ds::math::followRelative(cp, wp, &angle, 5.0f, 1.0f);
	_velocity = math::getRadialVelocity(angle, 400.0f);
	_moving = true;
}

void Ball::bounceX() {
	// FIXME: squeeze y scale
	_velocity.x *= -1.0f;
}

void Ball::bounceY() {
	// FIXME: squeeze y scale
	_velocity.y *= -1.0f;
}