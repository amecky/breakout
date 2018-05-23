#pragma once
#include <diesel.h>

class SpriteBatchBuffer;

const int NUM_SEGMENTS = 10;

struct Segment {
	ds::vec2 pos;
	float rotation;
};

class Worm {
	
public:
	Worm();
	void move(float dt, float minDist, float relaxation);
	void render(SpriteBatchBuffer* sprites);
private:
	ds::vec2 _pos;
	ds::vec2 _previous;
	Segment _segments[NUM_SEGMENTS];
	float _timer;
};