#pragma once
#include <core\data\DynamicSettings.h>

struct GameSettings : public ds::DynamicGameSettings {

	struct Grid {
		float ttl;
		ds::Color flash;
		ds::Color regular;
	} grid;

	GameSettings() : ds::DynamicGameSettings("content\\game_settings.json") {
		add("grid.ttl", &grid.ttl, 0.2f);
		add("grid.regular_color", &grid.regular, ds::Color(32, 32, 32, 255));
		add("grid.flash_color", &grid.flash, ds::Color(32, 32, 32, 255));
	}

};
