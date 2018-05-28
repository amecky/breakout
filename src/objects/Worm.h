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
};

class Ship {

public:
	Ship() {}
	void render(const Transformation& movement, SpriteBatchBuffer* sprites);
private:

};

class Worm {
	
public:
	Worm();
	void move(const Transformation& movement, float dt, float minDist, float relaxation);
	void render(const Transformation& movement, SpriteBatchBuffer* sprites);
private:
	//ds::vec2 _pos;
	//ds::vec2 _previous;
	Segment _segments[NUM_SEGMENTS];

	//float _timer;
};