#pragma once

const int GRID_X = 10;
const int GRID_Y = 8;

const int MESSAGE_DISPLAYED = 200;
const int BRICKS_MOVED      = 201;

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
	OT_HUD,
	OT_EOL
};

