#pragma once
#include <gamestates\GameState.h>
#include "..\Breakout.h"
#include "..\GameContext.h"
#include "..\Constants.h"
#include "..\Paddle.h"
#include "..\Ball.h"
#include "..\Bricks.h"
#include "..\DirectionIndicator.h"
#include "..\Messages.h"
#include "..\HUD.h"

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
	HUD* _hud;
	bool _sticky;
	Paddle* _paddle;
	Ball* _ball;
	Bricks* _bricks;
	DirectionIndicator* _indicator;
	Messages* _messages;
	bool _running;
	int _level;
	int _numBricks;
	int _killedBricks;
	
};

