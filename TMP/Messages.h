#pragma once
#include <core\base\GameObject.h>
#include "GameContext.h"

struct Message {
	ID id;
	ds::Texture texture;
	v2 pos;
	float timer;
	float ttl;
	ds::Color color;
};

class Messages : public ds::GameObject {

public:
	Messages(GameContext* ctx);
	~Messages();
	void tick(float dt);
	void onActivation();
	void onDeactivation();
	void add(const v2& pos, const ds::Texture& t);
	void reset();
	void showLevel(int lvl);
private:
	GameContext* _context;
	ds::Array<Message> _messages;
	ds::Texture _numbers[10];
	ds::Texture _level;
	
};

