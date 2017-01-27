#include "MainGameState.h"
#include <core\log\Log.h>
#include "..\Constants.h"
#include <core\math\Bitset.h>
#include <core\io\FileRepository.h>
#include "..\WarpingGrid.h"

MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGameState") , _context(context) {
	_paddle = (Paddle*)ds::game::get_game_object(SID("Paddle"));
	_ball = (Ball*)ds::game::get_game_object(SID("Ball"));
	_bricks = (Bricks*)ds::game::get_game_object(SID("Bricks"));
	_indicator = (DirectionIndicator*)ds::game::get_game_object(SID("DirectionIndicator"));
	_sticky = false;
	_numBricks = 0;
	_killedBricks = 0;
	_level = 0;
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
	_sticky = false;
	_paddle->activate();
	_ball->activate();
	_bricks->activate();	
	setSticky();
	restart();
	_numBricks = _bricks->setLevel(_level);
	_killedBricks = 0;
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
	if (_sticky) {
		_ball->start(_indicator->getAngle());
		_indicator->deactivate();
		_sticky = false;
	}
	return 0;
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
			const ds::Collision& c = _context->world->getCollision(i);
			if (c.isBetween(OT_PLAYER, OT_BALL)) {
				//FIXME: push ball away
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
				_context->grid->applyForce(pp.xy(), 0.3f, 5.0f, 40.0f);
				if (_bricks->handleCollision(c.getIDByType(OT_BRICK))) {
					_context->world->remove(c.getIDByType(OT_BRICK));
					_context->particles->start(2, bp.xy());
					++_killedBricks;
					if (_killedBricks == _numBricks) {
						LOG << "we are done - NEXT LEVEL!";
					}
				}
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
}

// -------------------------------------------------------
// set sticky
// -------------------------------------------------------
void MainGameState::setSticky() {
	if (!_sticky) {
		_sticky = true;
		_indicator->activate();
		_ball->setMoving(false);
	}
}

// -------------------------------------------------------
// on char
// -------------------------------------------------------
int MainGameState::onChar(int ascii) {	
	if (ascii == 's') {
		setSticky();
	}
	if (ascii == 't') {
		_ball->start(_indicator->getAngle());
		_indicator->deactivate();
		_sticky = false;
	}
	if (ascii == 'r') {
		_context->particles->start(1, v2(512, 384));
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

