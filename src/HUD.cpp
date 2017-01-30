#include "HUD.h"
#include <core\math\GameMath.h>
#include "Constants.h"
#include <core\base\EventStream.h>
#include "utils.h"

HUD::HUD(GameContext* ctx) : ds::GameObject("HUD"), _context(ctx) {
	_level = 0;
	_points = 0;
}


HUD::~HUD() {
}

// ------------------------------------------------------
// 
// ------------------------------------------------------
void HUD::onActivation() {
	_level = 0;
	_points = 0;
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
}

// -------------------------------------------------------
// 
// -------------------------------------------------------
void HUD::tick(float dt) {	
}

// ------------------------------------------------------
// 
// ------------------------------------------------------
void HUD::onDeactivation() {	
}

void HUD::setLevel(int level) {
	ds::Texture t[2];
	int num = utils::convertNumber(level, 2, t);
	for (int i = 0; i < 2; ++i) {
		_context->world->setTexture(_levelIds[i], t[i]);
	}
}

void HUD::reset() {
	_context->world->removeByType(OT_HUD);
}
