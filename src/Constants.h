#pragma once
#include <Vector.h>

//const int GRID_SIZE_X = 10;
//const int GRID_SIZE_Y = 8;

// grid starting point
const float GSX = 196.0f;
const float GSY = 186.0f;
// grid size
const int GMX = 20;
const int GMY = 12;
// grid dimension
const float GDX = 70.0f;
const float GDY = 40.0f;

const v2 LEFT_WALL_POS = v2(140, 66);
const v2 RIGHT_WALL_POS = v2(890, 66);
const v2 TOP_WALL_POS = v2(215, 665);

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
	OT_LEFT_WALL,
	OT_TOP_WALL,
	OT_NEW_BRICK,
	OT_RIGHT_WALL,
	OT_EOL
};

enum LayerTypes {
	LT_BACKGROUND,
	LT_BORDER,
	LT_OBJECTS,
	LT_EOL
};