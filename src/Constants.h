#pragma once

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
	OT_EOL
};

enum LayerTypes {
	LT_BACKGROUND,
	LT_BORDER,
	LT_OBJECTS,
	LT_EOL
};