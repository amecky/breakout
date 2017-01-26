#pragma once
#include <gamestates\GameState.h>
#include "..\Breakout.h"
#include "..\GameContext.h"
#include "..\Constants.h"
#include "..\Paddle.h"
#include "..\Ball.h"
#include "..\Bricks.h"

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
	void movePlayer();
	void handleCollisions(float dt);
	void restart();
	void setSticky();
	void handleBallPlayerCollision();
	GameContext* _context;
	ds::World* _world;
	v2 _cursor_pos;
	ds::SID _player_id;
	ds::SID _ball_id;
	bool _sticky;
	ds::Vector2fPath _scalePath;
	//Grid* _grid;
	v2 _panelPosition;
	bool _showPanel;
	float _delta;
	int _type;

	Paddle* _paddle;
	Ball* _ball;
	Bricks* _bricks;
	//ds::GrayScaleEffect* _effect;
	//ds::GrayFadeEffect* _effect;
	
};

