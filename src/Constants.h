#pragma once

const int GRID_SIZE_X = 10;
const int GRID_SIZE_Y = 8;

const float GSX = 196.0f;
const float GSY = 186.0f;
const int GMX = 20;
const int GMY = 12;

struct Level {
	char name[32];
	int index;
	int grid[GRID_SIZE_X * GRID_SIZE_Y];
};

enum ParticleSystemTypes {
	PST_EXPLOSION,
	PST_BALL_TRAIL,
	PST_BRICK_EXPLOSION,
	PST_EOL
};

enum ObjectTypes {
	OT_UNKNOWN,
	OT_PLAYER,
	OT_BALL,
	OT_BRICK,
	OT_SIDE_WALL,
	OT_TOP_WALL,
	OT_NEW_BRICK,
	OT_EOL
};

enum LayerTypes {
	LT_BACKGROUND,
	LT_BORDER,
	LT_OBJECTS,
	LT_EOL
};