#include "MainGameState.h"
#include <utils\Log.h>
#include "..\Constants.h"
#include <math\Bitset.h>
#include <io\FileRepository.h>
#include <utils\FileUtils.h>

MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGameState") , _context(context) , _world(context->world) {
	_grid = new Grid(context);
	_world->create(v2(512, 384), "background");
	v2 swp = v2(150, 66);
	for (int i = 0; i < 15; ++i) {
		ds::SID wall_id = _world->create(swp, "side_wall", LT_BACKGROUND);
		_world->attachBoxCollider(wall_id, OT_SIDE_WALL, LT_OBJECTS);
		swp.y += 40;
	}
	swp.x = 920.0f;
	swp.y = 66.0f;
	for (int i = 0; i < 15; ++i) {
		ds::SID wall_id = _world->create(swp, "side_wall", LT_BACKGROUND);
		_world->attachBoxCollider(wall_id, OT_SIDE_WALL, LT_OBJECTS);
		swp.y += 40;
	}
	swp.x = 215.0f;
	swp.y = 665.0f;
	for (int i = 0; i < 10; ++i) {
		ds::SID wall_id = _world->create(swp, "top_wall", LT_BACKGROUND);
		_world->attachBoxCollider(wall_id, OT_TOP_WALL, LT_OBJECTS);
		swp.x += 70;
	}
	_ball_id = _world->create(v2(512, 384), "ball", LT_OBJECTS);
	_player_id = _world->create(v2(512, 70), "player", LT_OBJECTS);
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

	
	for (int i = 0; i < 10; ++i) {
		readLevel(i);
	}
	LOG << "Levels: " << _levels.size();

	//_effect = new ds::GrayScaleEffect();
	_effect = new ds::BloomRenderEffect();
	//_effect = new ds::GrayFadeEffect();
	ds::editor::addSettingsEditor("Bloom", _effect->getSettings());
}

// -------------------------------------------------------
// destructor
// -------------------------------------------------------
MainGameState::~MainGameState(void) {
	delete _effect;
	delete _grid;
}

// -------------------------------------------------------
// activate
// -------------------------------------------------------
void MainGameState::activate() {
	restart();

	_showPanel = false;
	_panelPosition = v2(10, 760);
	_delta = 0.0f;
	_type = 0;
}

// -------------------------------------------------------
// restart game
// -------------------------------------------------------
void MainGameState::restart() {
	setSticky();
	_grid->clear();
	_grid->buildLevel(0);
}

// -------------------------------------------------------
// on button up
// -------------------------------------------------------
int MainGameState::onButtonUp(int button, int x, int y) {
	if (_sticky) {
		v2 vel = ds::vector::getRadialVelocity(DEGTORAD(45.0f), 500.0f);
		_world->moveBy(_ball_id, vel, true);
		_sticky = false;
		_context->trails->add(_ball_id, 5.0f, PST_BALL_TRAIL);
	}
	return 0;
}

// -------------------------------------------------------
// update
// -------------------------------------------------------
int MainGameState::update(float dt) {

	// move player
	_cursor_pos = ds::renderer::getMousePosition();
	float angle = 0.0f;
	v2 playerPosition = _world->getPosition(_player_id);
	playerPosition.y = 65.0f;
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

	// update grid
	_grid->tick(dt);

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
				//_world->scaleByPath(_ball_id, &_scalePath, 0.4f);
				_world->startBehavior(_ball_id, "ball_impact");
			}
			else if (c.containsType(OT_TOP_WALL) && c.containsType(OT_BALL)) {
				ds::SID bid = c.getSIDByType(OT_SIDE_WALL);
				_world->flashColor(bid, ds::Color(255, 255, 255, 255), ds::Color(192, 0, 0, 255), 0.4f);
				_world->scaleByPath(bid, &_scalePath, 0.4f);
				_world->bounce(_ball_id, ds::BD_Y, dt);
				//_world->scaleByPath(_ball_id, &_scalePath, 0.4f);
				_world->startBehavior(_ball_id, "wiggle_scale");
			}
			else if (c.containsType(OT_PLAYER) && c.containsType(OT_BALL)) {
				//LOG << "BOUNCE";
				_world->bounce(_ball_id, ds::BD_Y, dt);
				//_world->scaleByPath(_ball_id, &_scalePath, 0.4f);
				_world->startBehavior(_ball_id, "wiggle_scale");
				_world->startBehavior(_player_id, "wiggle_player");
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

	// check ball out of bounds
	v2 ballPos = _world->getPosition(_ball_id);
	// FIXME: check all sides due to bug in bouncing
	if (ballPos.y < 65.0f) {
		setSticky();
	}

	// update trails
	_context->trails->tick(dt);
	// update particles
	_context->particles->update(dt);

	// no clue what delta is supposed to be
	_delta += dt / 2.0f;
	if (_delta > 1.0f) {
		_delta = 1.0f;
	}

	// update the render effect
	_effect->tick(dt);

	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void MainGameState::render() {
	_effect->start();
	_world->renderSingleLayer(LT_BACKGROUND);
	//_world->renderSingleLayer(LT_BORDER);
	_world->renderSingleLayer(LT_OBJECTS);
	//_effect->render();
	//_effect->start();
	_context->particles->render();
	_effect->render();
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

// -------------------------------------------------------
// set sticky
// -------------------------------------------------------
void MainGameState::setSticky() {
	_sticky = true;
	_world->stopAction(_ball_id, ds::AT_MOVE_BY);
	_context->trails->remove(_ball_id);
}

// -------------------------------------------------------
// on char
// -------------------------------------------------------
int MainGameState::onChar(int ascii) {
	if (ascii == 's') {
		setSticky();
	}
	if (ascii == 'd') {
		_showPanel = !_showPanel;
	}
	if (ascii == 'e') {
		_world->startBehavior(_ball_id, "ball_impact");
	}
	if (ascii == 'g') {
		if (_effect->isActive()) {
			_effect->deactivate();
		}
		else {
			_effect->activate();
		}
	}
	if (ascii == 'q') {		
		_grid->moveDown();
		_grid->createNewLine(4);
	}
	if (ascii == 'w') {
		_grid->createNewLine(3);
	}
	return 0;
}

// -------------------------------------------------------
// read level
// -------------------------------------------------------
bool MainGameState::readLevel(int index) {
	int size = 0;
	char name[128];
	sprintf_s(name, 128, "levels\\Level%d.txt", index);
	if (ds::file::fileExists(name)) {
		char* l = ds::repository::load(name, &size);
		int num = 0;
		const char* p = l;
		Level level;
		while (*p != 0) {
			if (ds::string::isDigit(*p)) {
				if (*p != '-') {
					if (num < GRID_SIZE_X * GRID_SIZE_Y) {
						int type = *p - '0';
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
		delete[] l;
		_levels.push_back(level);
		return true;
	}
	return false;
}