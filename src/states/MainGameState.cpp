#include "MainGameState.h"
#include <core\log\Log.h>
#include "..\Constants.h"
#include <core\math\Bitset.h>
#include <core\io\FileRepository.h>

MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGameState") , _context(context) , _world(context->world) {
	_paddle = (Paddle*)ds::game::get_game_object(SID("Paddle"));
	_ball = (Ball*)ds::game::get_game_object(SID("Ball"));
	_bricks = (Bricks*)ds::game::get_game_object(SID("Bricks"));
	/*
	_grid = new Grid(context);
	_world->create(v2(512, 384), "background");
	v2 swp = LEFT_WALL_POS;
	for (int i = 0; i < 15; ++i) {
		ds::SID wall_id = _world->create(swp, "left_side_wall", LT_BACKGROUND);
		_world->attachBoxCollider(wall_id, OT_LEFT_WALL, LT_OBJECTS);
		swp.y += 40;
	}
	swp = RIGHT_WALL_POS;
	for (int i = 0; i < 15; ++i) {
		ds::SID wall_id = _world->create(swp, "right_side_wall", LT_BACKGROUND);
		_world->attachBoxCollider(wall_id, OT_RIGHT_WALL, LT_OBJECTS);
		swp.y += 40;
	}
	swp = TOP_WALL_POS;
	for (int i = 0; i < 10; ++i) {
		ds::SID wall_id = _world->create(swp, "top_wall", LT_BACKGROUND);
		_world->attachBoxCollider(wall_id, OT_TOP_WALL, LT_OBJECTS);
		swp.x += 70;
	}
	_ball_id = _world->create(v2(512, 384), "ball", LT_OBJECTS);
	_player_id = _world->create(v2(512, 70), "player", LT_OBJECTS);
	_world->attachBoxCollider(_ball_id, OT_BALL, LT_OBJECTS);
	_world->attachBoxCollider(_player_id, OT_PLAYER, LT_OBJECTS);

	_world->ignoreCollisions(OT_BRICK, OT_TOP_WALL);
	_world->ignoreCollisions(OT_BRICK, OT_BRICK);
	_world->ignoreCollisions(OT_PLAYER, OT_RIGHT_WALL);
	_world->ignoreCollisions(OT_PLAYER, OT_LEFT_WALL);

	//_world->attach_descriptor(LT_BORDER, "background_gradient");
	//_world->create(v2(512, 700), ds::math::buildTexture(0, 0, 900, 10), 0.0f, 1.0f, 1.0f, ds::Color::WHITE, 666, LT_BORDER);
	//_world->create(v2(960, 400), ds::math::buildTexture(0, 0, 600, 10), HALF_PI, 1.0f, 1.0f, ds::Color::WHITE, 666, LT_BORDER);
	//_world->create(v2(512, 100), ds::math::buildTexture(0, 0, 900, 10), 0.0f, 1.0f, 1.0f, ds::Color::WHITE, 666, LT_BORDER);

	_sticky = true;
	
	_scalePath.add(0.0f, v2(1, 1));
	_scalePath.add(0.4f, v2(0.4f, 0.4f));
	_scalePath.add(0.8f, v2(1.8f, 1.8f));
	_scalePath.add(1.0f, v2(1, 1));
	*/
}

// -------------------------------------------------------
// destructor
// -------------------------------------------------------
MainGameState::~MainGameState(void) {
	//delete _grid;
}

// -------------------------------------------------------
// activate
// -------------------------------------------------------
void MainGameState::activate() {
	_paddle->activate();
	_ball->activate();
	_bricks->activate();
	setSticky();
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
	//_grid->clear();
	//_grid->buildLevel(0);
}

// -------------------------------------------------------
// on button up
// -------------------------------------------------------
int MainGameState::onButtonUp(int button, int x, int y) {
	/*
	if (_sticky) {
		v2 vel = ds::vector::getRadialVelocity(DEGTORAD(45.0f), 500.0f);
		_world->moveBy(_ball_id, vel, true);
		_sticky = false;
		_context->trails->add(_ball_id, 5.0f, PST_BALL_TRAIL);
	}
	*/
	return 0;
}

// -------------------------------------------------------
// move player
// -------------------------------------------------------
void MainGameState::movePlayer() {
	/*
	// move player
	_cursor_pos = ds::renderer::getMousePosition();
	float angle = 0.0f;
	v2 playerPosition = _world->getPosition(_player_id);
	playerPosition.y = 65.0f;
	playerPosition.x = _cursor_pos.x;
	if (playerPosition.x < (LEFT_WALL_POS.x + 60.0f)) {
		playerPosition.x = LEFT_WALL_POS.x + 60.0f;
	}
	if (playerPosition.x > (RIGHT_WALL_POS.x - 60.0f)) {
		playerPosition.x = RIGHT_WALL_POS.x - 60.0f;
	}
	_world->setPosition(_player_id, playerPosition);
	if (_sticky) {
		playerPosition.y += 30.0f;
		_world->setPosition(_ball_id, playerPosition);
	}
	*/
}
// -------------------------------------------------------
// update
// -------------------------------------------------------
int MainGameState::update(float dt) {

	_context->world->tick(dt);

	if (_sticky) {
		v3 p = _context->world->getPosition(_paddle->getID());
		p.y += 30;
		_context->world->setPosition(_ball->getID(), p);
	}
	else {
		v3 p = _context->world->getPosition(_ball->getID());
		if (p.y < 60.0f) {
			setSticky();
		}
		handleCollisions(dt);
	}
	/*
	// move paddle
	movePlayer();
	// update grid
	if (!_grid->tick(dt)) {
		// GAME OVER!!!
		LOG << "GAME OVER!!!!!";
	}
	// check collisions
	handleCollisions(dt);
	// check ball out of bounds
	v2 ballPos = _world->getPosition(_ball_id);
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
	*/
	return 0;
}

// -------------------------------------------------------
// handle collisions
// -------------------------------------------------------
void MainGameState::handleCollisions(float dt) {
	int num = _context->world->numCollisions();
	if (num > 0) {
		LOG << "collisions: " << num;
		for (int i = 0; i < num; ++i) {
			ZoneTracker z1("MainGameState:collision");
			const ds::Collision& c = _world->getCollision(i);
			if (c.isBetween(OT_PLAYER, OT_BALL)) {
				_ball->bounceY();
			}
			else if (c.isBetween(OT_BRICK, OT_BALL)) {
				
				v3 bp = _context->world->getPosition(c.getIDByType(OT_BRICK));
				v3 pp = _context->world->getPosition(c.getIDByType(OT_BALL));

				// http://gamedev.stackexchange.com/questions/29786/a-simple-2d-rectangle-collision-algorithm-that-also-determines-which-sides-that
				float w = 0.5 * (100.0f + 18.0f);
				float h = 0.5 * (30.0f + 18.0f);
				float dx = bp.x - pp.x;
				float dy = bp.y - pp.y;

				if (abs(dx) <= w && abs(dy) <= h) {
					float wy = w * dy;
					float hx = h * dx;
					LOG << "dx: " << dx << " dy: " << dy << " wy: " << wy << " hx: " << hx;
					if (wy > hx) {
						if (wy > -hx) {
							LOG << "BOTTOM";
							_ball->bounceY();
						}
						else {
							LOG << "LEFT";
							_ball->bounceX();
						}
					}
					else {
						if (wy > -hx) {
							LOG << "RIGHT";
							_ball->bounceX();
						}
						else {
							LOG << "TOP";
							_ball->bounceY();
						}
					}
				}
				_context->world->remove(c.getIDByType(OT_BRICK));
			}
		}
	}
	/*
	// collisions
	int numCollisions = _world->getNumCollisions();
	if (numCollisions > 0) {
		LOG << "--------------- num: " << numCollisions;
		for (int i = 0; i < numCollisions; ++i) {
			ZoneTracker z1("MainGameState:collision");
			const ds::Collision& c = _world->getCollision(i);
			if (c.containsType(OT_LEFT_WALL) && c.containsType(OT_BALL)) {
				ds::SID bid = c.getSIDByType(OT_LEFT_WALL);
				_world->bounce(_ball_id, ds::BD_X, dt);
				_world->startBehavior(bid, "wall_impact");
				_world->startBehavior(_ball_id, "ball_impact");
				v2 ballPos = _world->getPosition(_ball_id);
				ballPos.x = LEFT_WALL_POS.x + 26.0f;
				_world->setPosition(_ball_id, ballPos);
			}
			else if (c.containsType(OT_RIGHT_WALL) && c.containsType(OT_BALL)) {
				ds::SID bid = c.getSIDByType(OT_RIGHT_WALL);
				_world->bounce(_ball_id, ds::BD_X, dt);
				_world->startBehavior(bid, "wall_impact");
				_world->startBehavior(_ball_id, "ball_impact");
				v2 ballPos = _world->getPosition(_ball_id);
				ballPos.x = RIGHT_WALL_POS.x - 26.0f;
				_world->setPosition(_ball_id, ballPos);
			}
			else if (c.containsType(OT_TOP_WALL) && c.containsType(OT_BALL)) {
				ds::SID bid = c.getSIDByType(OT_TOP_WALL);
				_world->startBehavior(bid, "top_wall_impact");
				_world->bounce(_ball_id, ds::BD_Y, dt);
				_world->startBehavior(_ball_id, "ball_impact");
				v2 ballPos = _world->getPosition(_ball_id);
				ballPos.y = TOP_WALL_POS.y - 16.0f;
				_world->setPosition(_ball_id, ballPos);
			}
			else if (c.containsType(OT_PLAYER) && c.containsType(OT_BALL)) {
				handleBallPlayerCollision();
			}
			else if (c.containsType(OT_BALL) && c.containsType(OT_BRICK)) {
				ds::SID bid = c.getSIDByType(OT_BRICK);
				int ret = ds::physics::testLineBox(_world->getPosition(_ball_id), _world->getPosition(bid), v2(60, 30));
				/*
				v2 bp = _world->getPosition(bid);
				v2 pp = _world->getPosition(_ball_id);

				// http://gamedev.stackexchange.com/questions/29786/a-simple-2d-rectangle-collision-algorithm-that-also-determines-which-sides-that
				float w = 0.5 * (120.0f + 18.0f);
				float h = 0.5 * (30.0f + 18.0f);
				float dx = bp.x - pp.x;
				float dy = bp.y - pp.y;

				if (abs(dx) <= w && abs(dy) <= h) {
					float wy = w * dy;
					float hx = h * dx;
					LOG << "dx: " << dx << " dy: " << dy << " wy: " << wy << " hx: " << hx;
					if (wy > hx) {
						if (wy > -hx) {
							LOG << "BOTTOM";
						}
						else {
							LOG << "LEFT";
						}
					}
					else {
						if (wy > -hx) {
							LOG << "RIGHT";
						}
						else {
							LOG << "TOP";
						}
					}
				}
				
				
				LOG << "RET: " << ret;// << " brick: " << DBG_V2(bp) << " ball: " << DBG_V2(pp);
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
					_world->startBehavior(_ball_id, "ball_impact");
				}
			}
		}
	}
	*/
}

// -------------------------------------------------------
// ball player collision
// -------------------------------------------------------
void MainGameState::handleBallPlayerCollision() {
	/*
	v2 pp = _world->getPosition(_player_id);
	v2 bp = _world->getPosition(_ball_id);
	v2 diff = bp - pp;
	float xdir = 1.0f + diff.x / 60.0f;
	float angle = DEGTORAD(135.0f) - xdir * DEGTORAD(45.0f);
	_world->stopAction(_ball_id, ds::AT_MOVE_BY);
	_world->moveBy(_ball_id, ds::vector::getRadialVelocity(angle, 500.0f));
	bp.y = 95.0f;
	_world->setPosition(_ball_id, bp);
	_world->startBehavior(_ball_id, "wiggle_scale");
	_world->startBehavior(_player_id, "wiggle_player");
	*/
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void MainGameState::render() {
	/*
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
	*/
}

// -------------------------------------------------------
// set sticky
// -------------------------------------------------------
void MainGameState::setSticky() {
	_sticky = true;
	//_world->stopAction(_ball_id, ds::AT_MOVE_BY);
	//_context->trails->remove(_ball_id);
}

// -------------------------------------------------------
// on char
// -------------------------------------------------------
int MainGameState::onChar(int ascii) {	
	if (ascii == 's') {
		setSticky();
	}
	if (ascii == 't') {
		_ball->start();
		_sticky = false;
	}
	/*
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
	*/
	return 0;
}

