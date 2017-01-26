#include "Bricks.h"
#include <core\math\GameMath.h>
#include <base\InputSystem.h>
#include "Constants.h"

Bricks::Bricks(GameContext* ctx) : ds::GameObject("Bricks"), _context(ctx) {
	int max = GRID_X * GRID_Y * LEVELS;
	for (int i = 0; i < max; ++i) {
		_data[i] = 1;
	}
}

Bricks::~Bricks() {
}

// ------------------------------------------------------
// activate bricks - create new entities
// ------------------------------------------------------
void Bricks::onActivation() {
	int index = 0;
	int offset = GRID_X * GRID_Y * index;
	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 10; ++x) {
			int current = _data[x + y * GRID_X + offset];
			if (current == 1) {
				ID id = _context->world->create(v2(200 + x * 70, 400 + y * 40), math::buildTexture(60, 200, 60, 30), OT_BRICK);
				_context->world->attachCollider(id, ds::PST_QUAD, v2(60.f, 30.0));
				Brick* data = (Brick*)_context->world->attach_data(id, sizeof(Brick), 100);
				data->energy = 2;
			}
		}
	}
	
}

// -------------------------------------------------------
// move ball
// -------------------------------------------------------
void Bricks::tick(float dt) {	
}

// ------------------------------------------------------
// kill ball
// ------------------------------------------------------
void Bricks::onDeactivation() {	
}

