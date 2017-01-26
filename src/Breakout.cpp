#include "Breakout.h"
#include "states\MainGameState.h"
#include "GameContext.h"
#include <core\base\GameObject.h>
#include "Paddle.h"
#include "Ball.h"
#include "Bricks.h"

ds::BaseApp *app = new Breakout(); 

Breakout::Breakout() : ds::BaseApp() {
}

Breakout::~Breakout() {
	//delete _context->trails;
	delete _context;
}

// -------------------------------------------------------
// prepare
// -------------------------------------------------------
void Breakout::prepare(ds::Settings* settings) {
	settings->screenWidth = 1024;
	settings->screenHeight = 768;
	settings->clearColor = ds::Color(0, 0, 0, 255);
	settings->fullScreen = false;
	settings->reportingDirectory = "reports";
	settings->synched = true;
	settings->reloading = true;
}
// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Breakout::loadContent() {	

	RID material = ds::res::find("SpriteMaterial", ds::ResourceType::MATERIAL);
	ds::Material* m = ds::res::getMaterial(material);
	m->texture = ds::res::find("TextureArray", ds::ResourceType::TEXTURE);

	_context = new GameContext();
	_context->world = new ds::World;

	ds::game::add_game_object(new Paddle(_context));
	ds::game::add_game_object(new Ball(_context));
	ds::game::add_game_object(new Bricks(_context));
	//_context->particles = particles;
	//_context->trails = new ds::Trails(world,particles);
	//world->setBoundingRect(ds::Rect(40, 40, 940, 700));
	//world->loadBehaviors();
	addGameState(new MainGameState(_context));
	return true;
}

void Breakout::init() {
	pushState("MainGameState");
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
void Breakout::render() {
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	ds::ChannelArray* array = _context->world->getChannelArray();
	v3* positions = (v3*)array->get_ptr(ds::WEC_POSITION);
	ds::Texture* textures = (ds::Texture*)array->get_ptr(ds::WEC_TEXTURE);
	v3* rotations = (v3*)array->get_ptr(ds::WEC_ROTATION);
	v3* scales = (v3*)array->get_ptr(ds::WEC_SCALE);
	ds::Color* colors = (ds::Color*)array->get_ptr(ds::WEC_COLOR);
	for (uint32_t i = 0; i < array->size; ++i) {
		v2 p = positions[i].xy();
		float r = rotations[i].x;
		ds::Texture t = textures[i];
		sprites->draw(positions[i].xy(), textures[i], rotations[i].x, scales[i].xy(), colors[i]);
	}
	sprites->end();
}

void Breakout::OnChar( char ascii,unsigned int keyState ) {
	if ( ascii == 'y' ) {
		makeBallSticky();
	}
}
