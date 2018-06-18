#pragma once
#include <ds_base_app.h>
#include <SpriteBatchBuffer.h>
#include "objects\Ball.h"
#include "objects\DirectionIndicator.h"
#include "objects\Bricks.h"
#include "objects\Worm.h"
#include "objects\Metaballs.h"
#include "utils\ExpressionManager.h"

struct GameContext;

struct MoveDesc {
	float data[16];
	ds::vec2 center;
};

struct Paddle {
	ds::vec2 position;
	ds::vec4 texture;
	// FIXME: flag for squeezing
	bool wiggling;
	float timer;
};





class Breakout : public ds::BaseApp {

public:	
	Breakout();
	virtual ~Breakout();
	void initialize();
	void update(float dt);
	void render();
	void drawLeftPanel();
	void drawBottomPanel();
private:
	void handleButtons();
	bool _buttonPressed[2];
	bool _buttonClicked[2];
	GameContext* _context;
	bool _mousePressed;
	bool _rightButtonPressed;
	SpriteBatchBuffer* _sprites;
	
	
	Paddle _paddle;
	Ball _ball;
	Bricks _bricks;
	DirectionIndicator _indicator;
	Transformation _movement;
	Transformation _shipMovement;
	Transformation _hexagonMovement;
	Transformation _shapeMovement;
	Hexagon _hexagon;
	MoveDesc _desc;
	Worm _worm;
	Ship _ship;
	Shape _shape;
	bool _dbgFollow;
	float _dbgRelaxation;
	float _dbgMinDist;
	int _dbgIndex;
	ExpressionManager _expressionManager;
	int _moveXId;
	int _moveYId;
	char _moveXStr[128];
	char _moveYStr[128];

	Metaballs* _metaBalls;
};