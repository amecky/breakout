#include "MainGameState.h"
#include <utils\Log.h>
#include "..\Constants.h"
#include <math\Bitset.h>
#include <io\FileRepository.h>



MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGameState") , _context(context) , _world(context->world) {
	_grid = new Grid(context);
	_world->create(v2(512, 384), "background", LT_BACKGROUND);
	v2 swp = v2(100, 40);
	for (int i = 0; i < 24; ++i) {
		ds::SID wall_id = _world->create(swp, "side_wall", LT_BACKGROUND);
		_world->attachBoxCollider(wall_id, OT_SIDE_WALL, LT_OBJECTS);
		swp.y += 30;
	}
	swp.x = 900.0f;
	swp.y = 40.0f;
	for (int i = 0; i < 24; ++i) {
		ds::SID wall_id = _world->create(swp, "side_wall", LT_BACKGROUND);
		_world->attachBoxCollider(wall_id, OT_SIDE_WALL, LT_OBJECTS);
		swp.y += 30;
	}
	swp.x = 125.0f;
	swp.y = 735.0f;
	for (int i = 0; i < 26; ++i) {
		ds::SID wall_id = _world->create(swp, "top_wall", LT_BACKGROUND);
		_world->attachBoxCollider(wall_id, OT_TOP_WALL, LT_OBJECTS);
		swp.x += 30;
	}
	_ball_id = _world->create(v2(512, 384), "ball", LT_OBJECTS);
	_player_id = _world->create(v2(512, 60), "player", LT_OBJECTS);
	_world->attachBoxCollider(_ball_id, OT_BALL, LT_OBJECTS);
	_world->attachBoxCollider(_player_id, OT_PLAYER, LT_OBJECTS);
	//_world->attach_descriptor(LT_BORDER, "background_gradient");
	//_world->create(v2(512, 700), ds::math::buildTexture(0, 0, 900, 10), 0.0f, 1.0f, 1.0f, ds::Color::WHITE, 666, LT_BORDER);
	//_world->create(v2(960, 400), ds::math::buildTexture(0, 0, 600, 10), HALF_PI, 1.0f, 1.0f, ds::Color::WHITE, 666, LT_BORDER);
	//_world->create(v2(512, 100), ds::math::buildTexture(0, 0, 900, 10), 0.0f, 1.0f, 1.0f, ds::Color::WHITE, 666, LT_BORDER);

	_sticky = true;
	
	_scalePath.add(0.0f, v2(1, 1));
	_scalePath.add(0.4f, v2(0.4f, 0.4f));
	_scalePath.add(0.8f, v2(1.8f, 1.8f));
	_scalePath.add(1.0f, v2(1, 1));

	

	int size = 0;
	char* l = ds::repository::load("levels\\Level1.txt",&size);
	int num = 0;
	const char* p = l;
	Level level;
	while (*p != 0) {
		if (ds::string::isDigit(*p)) {
			if (*p != '-') {
				if (num < GRID_SIZE_X * GRID_SIZE_Y) {
					int type = *p - '0';
					LOG << "type: " << type;
					level.grid[num] = type;
				}
			}
			else if (num < GRID_SIZE_X * GRID_SIZE_Y) {
				level.grid[num] = -1;
			}
			++num;
		}
		++p;
	}
	_levels.push_back(level);
}


MainGameState::~MainGameState(void) {
	delete _grid;
}

void MainGameState::clear() {
}

void MainGameState::fill(const char* level) {
	clear();
}

void MainGameState::activate() {
	setSticky();
	_grid->buildLevel(_levels[0]);
	_showPanel = false;
	_panelPosition = v2(10, 760);
	_delta = 0.0f;
	_type = 0;
}

void MainGameState::restart() {
	setSticky();
	_grid->clear();
	_grid->buildLevel(1);
}

int MainGameState::onButtonUp(int button, int x, int y) {
	if (_sticky) {
		v2 vel = ds::vector::getRadialVelocity(DEGTORAD(45.0f), 400.0f);
		_world->moveBy(_ball_id, vel, true);
		_sticky = false;
		_context->trails->add(_ball_id, 5.0f, PST_BALL_TRAIL);
	}
	return 0;
}

int MainGameState::update(float dt) {
	_cursor_pos = ds::renderer::getMousePosition();
	float angle = 0.0f;
	v2 playerPosition = _world->getPosition(_player_id);
	//ds::math::followRelative(_cursor_pos, playerPosition, &angle, 5.0f, 1.1f * dt);
	playerPosition.y = 60.0f;
	playerPosition.x = _cursor_pos.x;
	if (playerPosition.x < 150.0f) {
		playerPosition.x = 150.0f;
	}
	if (playerPosition.x > 850.0f) {
		playerPosition.x = 850.0f;
	}
	ds::vector::clamp(playerPosition, v2(60, 60), v2(980, 840));
	_world->setPosition(_player_id, playerPosition);
	if (_sticky) {
		playerPosition.y += 30.0f;
		_world->setPosition(_ball_id, playerPosition);
	}

	// collisions
	int numCollisions = _world->getNumCollisions();
	if (numCollisions > 0) {
		for (int i = 0; i < numCollisions; ++i) {
			ZoneTracker z1("MainGameState:collision");
			const ds::Collision& c = _world->getCollision(i);
			if (c.containsType(OT_SIDE_WALL) && c.containsType(OT_BALL)) {
				ds::SID bid = c.getSIDByType(OT_SIDE_WALL);
				_world->flashColor(bid, ds::Color(255, 255, 255, 255), ds::Color(192, 0, 0, 255), 0.4f);
				_world->scaleByPath(bid, &_scalePath, 0.4f);
				_world->bounce(_ball_id, ds::BD_X, dt);
				_world->scaleByPath(_ball_id, &_scalePath, 0.4f);
			}
			else if (c.containsType(OT_TOP_WALL) && c.containsType(OT_BALL)) {
				ds::SID bid = c.getSIDByType(OT_SIDE_WALL);
				_world->flashColor(bid, ds::Color(255, 255, 255, 255), ds::Color(192, 0, 0, 255), 0.4f);
				_world->scaleByPath(bid, &_scalePath, 0.4f);
				_world->bounce(_ball_id, ds::BD_Y, dt);
				_world->scaleByPath(_ball_id, &_scalePath, 0.4f);
			}
			else if (c.containsType(OT_PLAYER) && c.containsType(OT_BALL)) {
				//LOG << "BOUNCE";
				_world->bounce(_ball_id, ds::BD_Y, dt);
				_world->scaleByPath(_ball_id, &_scalePath, 0.4f);
			}
			else if (c.containsType(OT_BALL) && c.containsType(OT_BRICK)) {
				ds::SID bid = c.getSIDByType(OT_BRICK);
				int ret = ds::physics::testLineBox(_world->getPosition(_ball_id), _world->getPosition(bid), v2(60, 30));
				if (ds::bit::is_set(ret, 0) || ds::bit::is_set(ret, 2)) {
					_world->bounce(_ball_id, ds::BD_Y, dt);
				}
				if (ds::bit::is_set(ret, 1) || ds::bit::is_set(ret, 3)) {
					_world->bounce(_ball_id, ds::BD_X, dt);
				}
				int hit = _grid->handleHit(c.getSIDByType(OT_BRICK));
				if (hit > 0) {
					
					_context->particles->start(PST_BRICK_EXPLOSION, _world->getPosition(bid));
					_world->remove(bid);
				}
				else {
					_world->fadeColorTo(_ball_id, ds::Color(255, 0, 0, 255), ds::Color(162, 221, 247, 255), 0.4f);
					_world->scaleByPath(_ball_id, &_scalePath, 0.4f);
				}
			}
		}
	}

	v2 ballPos = _world->getPosition(_ball_id);
	if (ballPos.y < 45.0f) {
		setSticky();
	}

	_context->trails->tick(dt);
	_context->particles->update(dt);

	_delta += dt / 2.0f;
	if (_delta > 1.0f) {
		_delta = 1.0f;
	}
	return 0;
}

void MainGameState::render() {
	_world->renderSingleLayer(LT_BACKGROUND);
	//_world->renderSingleLayer(LT_BORDER);
	_world->renderSingleLayer(LT_OBJECTS);
	_context->particles->render();
	int state = 1;
	if (_showPanel) {
		gui::start(100, &_panelPosition);
		gui::begin("Debug", &state);
		if (gui::Button("Restart")) {
			restart();
		}
		gui::InputInt("Type", &_type);
		if (gui::Button("Restart Tweening")) {
			_delta = 0.0f;
		}
		gui::end();
	}

	//tweening::draw(tweening::get_by_index(_type), ds::math::buildTexture(0, 0, 4, 4), 0.02f, _delta);
}

void MainGameState::setSticky() {
	_sticky = true;
	_world->stopAction(_ball_id, ds::AT_MOVE_BY);
	_context->trails->remove(_ball_id);
}

int MainGameState::onChar(int ascii) {
	if (ascii == 's') {
		setSticky();
	}
	if (ascii == 'd') {
		_showPanel = !_showPanel;
	}
	return 0;
}
