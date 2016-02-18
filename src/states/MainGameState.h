#pragma once
#include <base\GameState.h>
#include "..\Breakout.h"
#include "..\GameContext.h"
#include "..\Grid.h"
#include "..\Constants.h"
#include <effects\RenderEffect.h>
#include <effects\BloomRenderEffect.h>
#include <effects\GrayFadeRenderEffect.h>

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
	void restart();
	void setSticky();
	bool readLevel(int index);
	GameContext* _context;
	ds::World* _world;
	v2 _cursor_pos;
	ds::SID _player_id;
	ds::SID _ball_id;
	bool _sticky;
	ds::Vector2fPath _scalePath;
	Grid* _grid;
	v2 _panelPosition;
	bool _showPanel;
	float _delta;
	int _type;
	ds::Array<Level> _levels;

	//ds::GrayScaleEffect* _effect;
	ds::BloomRenderEffect* _effect;
	//ds::GrayFadeEffect* _effect;
	
};

