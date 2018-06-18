#pragma once
#include <diesel.h>
#include <SpriteBatchBuffer.h>

struct MetaBall {
	ds::vec2 pos;
	ds::vec2 velocity;
	ds::vec2 scale;
};

struct MetaBallSpriteBatchConstantBuffer {
	ds::vec4 screenCenter;
	ds::matrix wvp;
	ds::vec4 data;
};


class Metaballs {

public:
	Metaballs(RID textureID);
	~Metaballs();
	void move(float dt);
	void render();
private:
	MetaBall _balls[64];
	SpriteBatchBuffer* _sprites;
	MetaBallSpriteBatchConstantBuffer _metaBallConstantBuffer;
};

