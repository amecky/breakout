#include "Bricks.h"
#include <core\math\GameMath.h>
#include <base\InputSystem.h>
#include "Constants.h"
#include <core\base\EventStream.h>

Bricks::Bricks(GameContext* ctx) : ds::GameObject("Bricks") , _context(ctx) {
	_data.load();
	for (int i = 0; i < 4; ++i) {
		_textures[i] = math::buildTexture(60, 200 + i * 65, 60, 30);
	}
	_path.add(0.0f, v3(1.0f, 1.0f, 1.0f));
	_path.add(0.25f, v3(0.6f, 0.6f, 0.7f));
	_path.add(0.5f, v3(1.0f, 1.0f, 1.0f));
	_path.add(0.75f, v3(0.8f, 0.8f, 0.9f));
	_path.add(1.0f, v3(1.0f, 1.0f, 1.0f));
}

Bricks::~Bricks() {
}

// ------------------------------------------------------
// activate bricks - create new entities
// ------------------------------------------------------
void Bricks::onActivation() {
}

// ------------------------------------------------------
// set level and returns number of bricks
// ------------------------------------------------------
int Bricks::setLevel(int level) {
	_context->world->removeByType(OT_BRICK);
	int* data = _data.getDataPtr(level);
	_numBricks = 0;
	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 10; ++x) {
			int current = data[x + y * GRID_X];
			if (current > 0) {
				v2 p = v2(200 + x * 70, 400 + y * 40);
				float dy = math::random(0.0f, 80.0f) + 500.0f;
				ID id = _context->world->create(v2(p.x, p.y + dy), _textures[current - 1], OT_BRICK);
				_context->world->attachCollider(id, ds::PST_QUAD, v2(60.f, 30.0));				
				_context->world->moveTo(id, v3(p.x, p.y + dy, 0.0f), v3(p), 1.0f, tweening::easeInCubic);
				Brick* data = (Brick*)_context->world->attach_data(id, sizeof(Brick), 100);
				data->energy = current;
				++_numBricks;
			}
		}
	}
	_movedBricks = 0;
	return _numBricks;
}

// -------------------------------------------------------
// move ball
// -------------------------------------------------------
void Bricks::tick(float dt) {	
	if (_context->world->numEvents() > 0) {
		for (uint32_t i = 0; i < _context->world->numEvents(); ++i) {
			const ds::ActionEvent& ev = _context->world->getEvent(i);
			if (ev.action == ds::AT_MOVE_TO && ev.type == OT_BRICK) {
				++_movedBricks;
				if (_movedBricks == _numBricks) {
					ds::events::send(BRICKS_MOVED);
				}
			}
		}
	}
}

// ------------------------------------------------------
// kill ball
// ------------------------------------------------------
void Bricks::onDeactivation() {	
}

// ------------------------------------------------------
// handle collision
// ------------------------------------------------------
bool Bricks::handleCollision(ID id) {
	Brick* data = (Brick*)_context->world->get_data(id);
	--data->energy;
	if (data->energy <= 0) {
		_context->world->remove(id);
		return true;
	}
	else {
		_context->world->scaleByPath(id, &_path, 0.3f);
	}
	return false;
}
