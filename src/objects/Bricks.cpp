#include "Bricks.h"
#include <SpriteBatchBuffer.h>

Bricks::Bricks() {
	ID id = _bricks.add();
	Brick& brick = _bricks.get(id);
	brick.position = ds::vec2(500, 384);
	brick.extent = ds::vec2(30, 15);
	brick.aabBox = { brick.position,ds::vec2(60,40) };
}

void Bricks::render(SpriteBatchBuffer * sprites) {
	for (int i = 0; i < _bricks.numObjects; ++i) {
		sprites->add(_bricks.objects[i].position, ds::vec4(200, 60, 60, 30));
	}
}

ID Bricks::checkIntersection(const ds::vec2 & pos, const ds::vec2 & extent) const {
	for (int i = 0; i < _bricks.numObjects; ++i) {
		const Brick& brick = _bricks.objects[i];
		if (checkIntersection(brick, pos, extent)) {
			return brick.id;
		}
	}
	return NO_RID;
}

ID Bricks::checkIntersection(phAABBox box, const ds::vec2& previous, float *u0, float *u1) const {
	for (int i = 0; i < _bricks.numObjects; ++i) {
		const Brick& brick = _bricks.objects[i];
		if (sweepTest(brick.aabBox,brick.aabBox.pos, box, previous, u0, u1)) {
			return brick.id;
		}
	}
	return NO_RID;
}

bool Bricks::checkIntersection(const Brick& brick, const ds::vec2& pos, const ds::vec2& extent) const {
	float top = brick.position.y + brick.extent.y;
	float left = brick.position.x - brick.extent.x;
	float right = brick.position.x + brick.extent.x;
	float bottom = brick.position.y - brick.extent.y;

	float otherTop = pos.y + extent.y;
	float otherLeft = pos.x - extent.x;
	float otherRight = pos.x + extent.x;
	float otherBottom = pos.y - extent.y;

	if (right < otherLeft || left > otherRight) return false;
	if (bottom > otherTop || top < otherBottom) return false;
	return true;
}
