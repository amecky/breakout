#include "Breakout.h"
#include "utils\Log.h"
#include <renderer\shader.h>
#include "states\MainGameState.h"
#include "GameContext.h"

ds::BaseApp *app = new Breakout(); 

Breakout::Breakout() : ds::BaseApp() {
}

Breakout::~Breakout() {
	delete _context;
}
// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Breakout::loadContent() {	
	_context = new GameContext();
	_context->world = world;
	world->setBoundingRect(ds::Rect(40, 40, 940, 700));
	const char* lvl = 
		"1111222233"
		"---1123---"
		"11------11"
		"1111111111"
		"--22-22-22";
	addGameState(new MainGameState(_context));
	return true;
}

void Breakout::init() {
	activate("MainGameState");
}

void Breakout::reset() {
}

// -------------------------------------------------------
// make ball sticky
// -------------------------------------------------------
void Breakout::makeBallSticky() {
}

// -------------------------------------------------------
// release sticky ball 
// -------------------------------------------------------
void Breakout::releaseBall() {
}
// -------------------------------------------------------
// Update
// -------------------------------------------------------
void Breakout::update(float dt) {
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void Breakout::draw() {

	v2 mp = getMousePos();
	mp.y = 768.0f - mp.y;
	//renderer->draw(mp,0,ds::Rect(0,200,50,50));

}

void Breakout::OnChar( char ascii,unsigned int keyState ) {
	if ( ascii == 'y' ) {
		makeBallSticky();
	}
}
