#include "HUD.h"
#include <core\math\GameMath.h>
#include "Constants.h"
#include <core\base\EventStream.h>
#include "utils.h"

HUD::HUD(GameContext* ctx) : ds::GameObject("HUD"), _context(ctx) {
	_level = 0;
	_points = 0;
	_lives = 3;
	_timer = 0.0f;
	_current = 0;
}


HUD::~HUD() {
}

// ------------------------------------------------------
// on activation
// ------------------------------------------------------
void HUD::onActivation() {
	_level = 0;
	_points = 0;
	_lives = 3;
	_timer = 0.0f;
	_current = 0;
	ds::Texture t[6];
	int num = utils::convertNumber(0, 6, t);
	// FIXME: center numbers
	for (int i = 0; i < 6; ++i) {
		_pointsIds[i] = _context->world->create(v2(40, 560 - i * 60), t[i], OT_HUD);
	}
	num = utils::convertNumber(0, 2, t);
	for (int i = 0; i < 2; ++i) {
		_levelIds[i] = _context->world->create(v2(980, 440 - i * 60), t[i], OT_HUD);
	}
	utils::convertNumber(_lives, &t[0]);
	_livesId = _context->world->create(v2(980, 600), t[0], OT_HUD);
}

// -------------------------------------------------------
// tick - increase points 
// -------------------------------------------------------
void HUD::tick(float dt) {	
	if (_current < _points) {
		_timer += dt;
		if (_timer > 0.2f) {
			_timer -= 0.2f;
			_current += 10;
			ds::Texture t[6];
			int num = utils::convertNumber(_current, 6, t);
			// FIXME: center numbers
			for (int i = 0; i < 6; ++i) {
				_context->world->setTexture(_pointsIds[i], t[i]);
			}
		}
	}
}

// ------------------------------------------------------
// on deactivation
// ------------------------------------------------------
void HUD::onDeactivation() {	
	_context->world->removeByType(OT_HUD);
}

// ------------------------------------------------------
// add score
// ------------------------------------------------------
void HUD::addScore(int pts) {
	_points += pts;
	_timer = 0.0f;
}

// ------------------------------------------------------
// set level
// ------------------------------------------------------
void HUD::setLevel(int level) {
	ds::Texture t[2];
	int num = utils::convertNumber(level, 2, t);
	for (int i = 0; i < 2; ++i) {
		_context->world->setTexture(_levelIds[i], t[i]);
	}
}

// ------------------------------------------------------
// decrement live counter
// ------------------------------------------------------
void HUD::decrementLive() {
	--_lives;
	if (_lives < 0) {
		_lives = 0;
	}
	ds::Texture t;
	utils::convertNumber(_lives, &t);
	_context->world->setTexture(_livesId, t);
}
