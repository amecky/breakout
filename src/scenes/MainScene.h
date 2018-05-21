#pragma once
#include <ds_base_app.h>

struct GameContext;

class MainScene : public ds::BaseScene {

public:
	MainScene(GameContext* gameContext);
	virtual ~MainScene();
	void initialize();
};