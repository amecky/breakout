#pragma once
#include <core\world\World.h>
#include <particles\ParticleManager.h>
#include "GameSettings.h"

class WarpingGrid;

class ElasticBorder;

struct GameContext {

	ds::World* world;
	ds::SID playerID;
	ds::ParticleManager* particles;
	GameSettings* settings;
	WarpingGrid* grid;
	ElasticBorder* elasticBorder;

};