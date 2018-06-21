#pragma once
#include <diesel.h>

struct Transformation {
	ds::vec2 pos;
	ds::vec2 previous;
	float timer;
	ds::vec2 scale;
	float rotation;
};

class SpriteBatchBuffer;

const int NUM_SEGMENTS = 10;

struct Segment {
	ds::vec2 pos;
	float rotation;
	ds::vec2 scale;
	ds::Color color;
	float timer;
	int expressionId;
};

struct EnemyDesc {
	Segment segments[64];
	int num;
	Transformation transformation;	
};

namespace enemies {

	void render(const EnemyDesc& desc, SpriteBatchBuffer* sprites);

	void build_shape(EnemyDesc* desc, int num);

	void build_shape(EnemyDesc* desc, ds::vec2* points, int num);

	void connect_segments(EnemyDesc* desc, int num, bool connectFirst);

	void rebuild_segments(EnemyDesc* desc, int num, bool connectFirst);
}

class Worm {
	
public:
	Worm();
	void move(const Transformation& movement, float dt, float minDist, float relaxation);
	void render(const Transformation& movement, SpriteBatchBuffer* sprites);
private:
	Segment _segments[NUM_SEGMENTS];
};