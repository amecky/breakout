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
}

class Enemy {

public:
	Enemy() : _num(0) {}
	void render(const Transformation& movement, SpriteBatchBuffer* sprites);
	int num() const {
		return _num;
	}
	const Segment& getSegment(int index)  const {
		return _segments[index];
	}
	Segment& getSegment(int index) {
		return _segments[index];
	}
	void tick(float dt) {
		for (int i = 0; i < _num; ++i) {
			_segments[i].timer += dt;
		}
	}
	virtual void update(float dt) {}
	int add();
	int remove(int index);
protected:
	int _num;
	Segment _segments[32];
	Transformation _transformation;
private:

};

class Ship : public Enemy {

public:
	Ship();
	void update(float dt);
};

class Hexagon : public Enemy {

public:
	Hexagon() {}
	void init();
};


class Shape : public Enemy {

public:
	Shape() : _pieces(0) {}
	void init(int num);
	int numPieces() const {
		return _pieces;
	}
private:
	int _pieces;
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