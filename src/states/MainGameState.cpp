#include "MainGameState.h"
#include <utils\Log.h>

MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGameState") , _context(context) , _world(context->world) {
	_ball_id = _world->create(v2(512, 384), "ball");
	_player_id = _world->create(v2(512, 60), "player");
	_world->attachBoxCollider(_ball_id, 1, 0);
	_world->attachBoxCollider(_player_id, 2, 0);
	_sticky = true;

	_scalePath.add(0.0f, v2(1, 1));
	_scalePath.add(0.4f, v2(0.7f, 0.7f));
	_scalePath.add(0.8f, v2(1.2f, 1.2f));
	_scalePath.add(1.0f, v2(1, 1));
}


MainGameState::~MainGameState(void) {
}

void MainGameState::clear() {
}

void MainGameState::fill(const char* level) {
	clear();
}

void MainGameState::activate() {
	_sticky = true;
}

int MainGameState::onButtonUp(int button, int x, int y) {
	v2 vel = ds::vector::getRadialVelocity(DEGTORAD(45.0f), 400.0f);
	_world->moveBy(_ball_id, vel, true);
	_sticky = false;
	return 0;
}

int MainGameState::update(float dt) {
	_cursor_pos = ds::renderer::getMousePosition();
	float angle = 0.0f;
	v2 playerPosition = _world->getPosition(_player_id);
	ds::math::followRelative(_cursor_pos, playerPosition, &angle, 5.0f, 1.1f * dt);
	playerPosition.y = 60.0f;
	playerPosition.x = _cursor_pos.x;
	ds::vector::clamp(playerPosition, v2(60, 60), v2(980, 840));
	_world->setPosition(_player_id, playerPosition);
	if (_sticky) {
		playerPosition.y += 40.0f;
		_world->setPosition(_ball_id, playerPosition);
	}

	// collisions
	int numCollisions = _world->getNumCollisions();
	if (numCollisions > 0) {
		for (int i = 0; i < numCollisions; ++i) {
			ZoneTracker z1("MainGameState:collision");
			const ds::Collision& c = _world->getCollision(i);
			if (c.containsType(1) && c.containsType(2)) {
				//LOG << "BOUNCE";
				_world->bounce(_ball_id, ds::BD_Y);
				_world->scaleByPath(_ball_id, &_scalePath, 0.4f);
			}
		}
	}

	v2 ballPos = _world->getPosition(_ball_id);
	if (ballPos.y < 65.0f) {
		_sticky = true;
		_world->stopAction(_ball_id, ds::AT_MOVE_BY);
	}

	return 0;
}

void MainGameState::render() {
	_world->render();
}
