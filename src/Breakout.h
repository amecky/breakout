#pragma comment(lib, "Diesel2D.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include "base\BaseApp.h"
#include "dxstdafx.h"
#include <renderer\render_types.h>

struct GameContext;

class Breakout : public ds::BaseApp {

public:	
	Breakout();
	virtual ~Breakout();
	bool loadContent();
	const char* getTitle() {
		return "Breakout";
	}
	void update(float dt);
	void draw();
	void init();
private:
	void reset();
	virtual void OnChar( char ascii,unsigned int keyState );
	void makeBallSticky();
	void releaseBall();
	GameContext* _context;
};