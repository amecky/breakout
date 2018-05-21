#include "DirectionIndicator.h"
#include <core\math\GameMath.h>
#include <base\InputSystem.h>
#include "Constants.h"

DirectionIndicator::DirectionIndicator(GameContext* ctx) : ds::GameObject("DirectionIndicator"), _context(ctx) {
	_paddle = (Paddle*)ds::game::get_game_object(SID("Paddle"));
	_angle = 0.0f;
}

DirectionIndicator::~DirectionIndicator() {
}

// ------------------------------------------------------
// activate player - create new entity
// ------------------------------------------------------
void DirectionIndicator::onActivation() {
	_id = _context->world->create(v2(512, 80), math::buildTexture(300, 280, 120, 120), 32);
	_context->world->attachName(_id, "DirectionIndicator");
}

// -------------------------------------------------------
// move player
// -------------------------------------------------------
void DirectionIndicator::tick(float dt) {
	ZoneTracker u2("DirectionIndicator::tick");
	v2 cp = ds::input::getMousePosition();
	v2 wp = _context->world->getPosition(_paddle->getID()).xy();
	v2 dp = wp;
	dp.y += 30.0f;
	ds::math::followRelative(cp, wp, &_angle, 5.0f, 1.0f);
	_angle = math::clamp(_angle, DEGTORAD(45.0f), DEGTORAD(135.0f));
	_context->world->setPosition(_id, dp);
	_context->world->setRotation(_id, _angle);
}

// ------------------------------------------------------
// kill player
// ------------------------------------------------------
void DirectionIndicator::onDeactivation() {
	_context->world->remove(_id);
}


