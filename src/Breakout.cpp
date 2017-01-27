#include "Breakout.h"
#include "states\MainGameState.h"
#include "GameContext.h"
#include <core\base\GameObject.h>
#include "Paddle.h"
#include "Ball.h"
#include "Bricks.h"
#include "WarpingGrid.h"
#include "ElasticBorder.h"
#include "DirectionIndicator.h"

ds::BaseApp *app = new Breakout(); 

Breakout::Breakout() : ds::BaseApp() {
}

Breakout::~Breakout() {
	delete _context->world;
	delete _context->settings;
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
	_context->settings = new GameSettings;
	_context->settings->load();
	_context->world = new ds::World;
	_context->grid = new WarpingGrid(_context->settings);
	_context->grid->createGrid();
	_context->elasticBorder = new ElasticBorder();
	_context->particles = ds::res::getParticleManager();

	ds::game::add_game_object(new Paddle(_context));
	ds::game::add_game_object(new Ball(_context));
	ds::game::add_game_object(new Bricks(_context));
	ds::game::add_game_object(new DirectionIndicator(_context));
	//world->setBoundingRect(ds::Rect(40, 40, 940, 700));
	//world->loadBehaviors();
	addGameState(new MainGameState(_context));
	return true;
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void Breakout::init() {
	pushState("MainGameState");
}

// -------------------------------------------------------
// Update
// -------------------------------------------------------
void Breakout::update(float dt) {
	_context->grid->tick(dt);
	_context->elasticBorder->tick(dt);
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void Breakout::render() {
	_context->grid->render();
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
	_context->elasticBorder->render();
	_context->particles->render();
}
