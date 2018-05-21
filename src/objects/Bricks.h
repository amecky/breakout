#pragma once
#include <diesel.h>
#include "..\lib\DataArray.h"
#include <ds_intersection.h>

class SpriteBatchBuffer;

struct Brick {
	ID id;
	ds::vec2 position;
	ds::vec2 extent;
	phAABBox aabBox;
};

class Bricks {

public:
	Bricks();
	void render(SpriteBatchBuffer* sprites);
	ID checkIntersection(const ds::vec2& pos, const ds::vec2& extent) const;
	ID checkIntersection(phAABBox box, const ds::vec2& previous, float *u0, float *u1) const;
	const Brick& getBrick(ID id) const {
		return _bricks.get(id);
	}
private:
	bool checkIntersection(const Brick& brick, const ds::vec2& pos, const ds::vec2& extent) const;
	ds::DataArray<Brick> _bricks;
};