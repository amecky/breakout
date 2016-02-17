#pragma once
#include "GameContext.h"
#include "Constants.h"

// ---------------------------------------
// Brick definition
// ---------------------------------------
struct BrickDefinition {

	char name[20];
	int type;
	int energy;
};

// ---------------------------------------
// Cube definitions
// ---------------------------------------
class BrickDefinitions : public ds::DataFile {

public:
	BrickDefinitions() {}
	~BrickDefinitions() {}
	bool saveData(ds::JSONWriter& writer) {
		return true;
	}
	bool loadData(const ds::JSONReader& reader);
	const char* getFileName() const {
		return "resources\\brick_definitions.json";
	}
	size_t size() const {
		return _definitions.size();
	}
	const BrickDefinition& get(size_t index) const {
		return _definitions[index];
	}
private:
	ds::Array<BrickDefinition> _definitions;
};

struct Brick {
	int energy;
	int type;

};

class Grid {

public:
	Grid(GameContext* context);
	~Grid();
	void buildLevel(int level);
	void buildLevel(const Level& level);
	int handleHit(ds::SID sid);
	void clear();
	void moveDown();
	void createNewLine(int count);
	void debug();
private:
	void findLimit();
	GameContext* _context;
	ds::World* _world;
	ds::Vector2fPath _scalePath;
	BrickDefinitions _brickDefinitions;
	ds::SID _board[20][16];
	int _limit;
};

