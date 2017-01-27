#pragma once
#include <gamestates\GameState.h>
#include "..\Breakout.h"
#include "..\GameContext.h"
#include "..\Constants.h"
#include "..\Paddle.h"
#include "..\Ball.h"
#include "..\Bricks.h"
#include "..\DirectionIndicator.h"

class MainGameState : public ds::GameState {

public:
	MainGameState(GameContext* context);
	~MainGameState(void);
	int update(float dt);
	void render();
	void activate();
	int onButtonUp(int button, int x, int y);
	int onChar(int ascii);
private:
	void handleCollisions(float dt);
	void restart();
	void setSticky();
	void handleBallPlayerCollision();
	GameContext* _context;
	bool _sticky;
	Paddle* _paddle;
	Ball* _ball;
	Bricks* _bricks;
	DirectionIndicator* _indicator;
	int _level;
	int _numBricks;
	int _killedBricks;
	
};

