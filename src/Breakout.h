#pragma comment(lib, "D11.lib")
#pragma comment(lib, "DieselCore.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include "base\BaseApp.h"
#include <renderer\render_types.h>

struct GameContext;

class Breakout : public ds::BaseApp {

public:	
	Breakout();
	virtual ~Breakout();
	bool loadContent();
	bool initialize() {
		return true;
	}
	const char* getTitle() {
		return "Breakout";
	}
	void update(float dt);
	void render();
	void init();
protected:
	void prepare(ds::Settings* settings);
private:
	GameContext* _context;
};