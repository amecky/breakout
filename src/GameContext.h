#pragma once
#include <world\World.h>
#include <particles\ParticleManager.h>
#include <utils\Trails.h>

struct GameContext {

	ds::World* world;
	ds::SID playerID;
	ds::ParticleManager* particles;
	ds::Trails* trails;

};