#pragma once
#include <ds_base_app.h>
#include <SpriteBatchBuffer.h>
#include "objects\Ball.h"
#include "objects\DirectionIndicator.h"
#include "objects\Bricks.h"

struct GameContext;

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

	bool _dbgFollow;
};