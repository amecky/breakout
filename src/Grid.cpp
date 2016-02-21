#include "Grid.h"
#include "Constants.h"
#include <utils\Log.h>

bool convertFromGrid(int x, int y, v2* ret) {
	if (x < 0 || x >= 10) {
		return false;
	}
	if (y < 0 || y >= GMY) {
		return false;
	}
	ret->x = GSX + x * GDX;
	ret->y = GSY + y * GDY;
	return true;
}

bool convertToGrid(const v2& p, Vector2i* ret) {
	int x = (p.x - GSX) / GDX;
	int y = (p.y - GSY) / GDY;
	//LOG << "p: " << DBG_V2(p) << " x: " << x << " y: " << y;
	if (x < 0 || x >= 10) {
		return false;
	}
	if (y < 0 || y >= GMY) {
		return false;
	}
	ret->x = x;
	ret->y = y;
	return true;
}

// -------------------------------------------------------
// load level definitions
// -------------------------------------------------------
bool LevelDefinitions::loadData(const ds::JSONReader& reader) {
	_definitions.clear();
	int cats[32];
	int num = reader.get_categories(cats, 32);
	for (int i = 0; i < num; ++i) {
		LevelDefinition def;
		reader.get_int(cats[i], "brick_range", &def.brickRange);
		reader.get_int(cats[i], "new_bricks", &def.newBricks);
		reader.get_int(cats[i], "start_rows", &def.startRows);
		reader.get_int(cats[i], "total_rows", &def.totalRows);
		reader.get_float(cats[i], "move_delay", &def.moveDelay);
		_definitions.push_back(def);
	}
	return true;
}

// -------------------------------------------------------
// load brick definitions
// -------------------------------------------------------
bool BrickDefinitions::loadData(const ds::JSONReader& reader) {
	_definitions.clear();
	int cats[32];
	int num = reader.get_categories(cats, 32);
	for (int i = 0; i < num; ++i) {
		BrickDefinition def;
		reader.get_int(cats[i], "type", &def.type);
		sprintf_s(def.name, 20, "%s", reader.get_string(cats[i], "name"));
		reader.get_int(cats[i], "energy", &def.energy);
		_definitions.push_back(def);
	}
	return true;
}

// -------------------------------------------------------
// Grid
// -------------------------------------------------------
Grid::Grid(GameContext* context) : _context(context), _world(context->world) {
	_scalePath.add(0.0f, v2(1, 1));
	_scalePath.add(0.4f, v2(0.7f, 0.7f));
	_scalePath.add(0.8f, v2(1.2f, 1.2f));
	_scalePath.add(1.0f, v2(1, 1));
	_brickDefinitions.load();
	_levelDefinitions.load();
}

Grid::~Grid() {
}

void Grid::createNewLine(int count) {
	int tmp[10];
	for (int i = 0; i < 10; ++i) {
		tmp[i] = -1;
	}
	for (int x = 0; x < count; ++x) {
		tmp[x] = ds::math::random(0, 3);
	}
	for (int i = 9; i > 0; i--) {
		// Pick a random index from 0 to i
		int j = ds::math::random(0, 9);
		int t = tmp[i];
		tmp[i] = tmp[j];
		tmp[j] = t;
	}
	for (int x = 0; x < 10; ++x) {
		//LOG << x << " = " << tmp[x];
		if (tmp[x] != -1) {
			v2 p = v2(GSX + x * GDX, GSY + 13 * GDY);
			const BrickDefinition& def = _brickDefinitions.get(tmp[x]);
			ds::SID sid = _world->create(p, def.name, LT_OBJECTS);
			_world->setType(sid, OT_NEW_BRICK);
			_world->scaleTo(sid, v2(1.0f, 0.1f), v2(1, 1), 1.5f, 0, tweening::easeOutBounce);
			Brick* data = (Brick*)_world->attach_data(sid, sizeof(Brick));
			data->energy = def.energy;
			data->type = 1;
		}
	}
}

// -------------------------------------------------------
// build level
// -------------------------------------------------------
void Grid::buildLevel(int level) {
	clear();
	if (level >= 0 && level < _levelDefinitions.size()) {
		const LevelDefinition& def = _levelDefinitions.get(level);
		createNewLine(def.newBricks);
		for (int i = 0; i < def.startRows; ++i) {			
			moveDown();
			createNewLine(def.newBricks);
		}
		_rowsLeft = def.totalRows;
		_moveTimer = 0.0f;
		_moveDelay = def.moveDelay;
	}
}

// -------------------------------------------------------
// tick
// -------------------------------------------------------
bool Grid::tick(float dt) {
	bool ret = true;
	_moveTimer += dt;
	if (_moveTimer >= _moveDelay) {
		_moveTimer = 0.0f;
		ret = moveDown();
		if (ret) {
			if (_rowsLeft > 1) {
				createNewLine(4);
			}
			if (_rowsLeft > 0) {
				--_rowsLeft;
			}
		}
	}
	return ret;
}

// -------------------------------------------------------
// find limit
// -------------------------------------------------------
void Grid::findLimit() {
	int f = -1;
	for (int y = 0; y < GMY; ++y) {
		int cnt = 0;
		for (int x = 0; x < GMX; ++x) {
			if (_board[x][y] != ds::INVALID_SID) {
				++cnt;
			}
		}
		if (f == -1 && cnt != 0) {			
			f = y;
		}
	}
	_limit = f;
	if (_limit == -1) {
		_limit = GMY - 1;
	}
	LOG << "LIMIT: " << _limit;
}

// -------------------------------------------------------
// handle hit
// -------------------------------------------------------
int Grid::handleHit(ds::SID sid) {
	if (_world->contains(sid) && _world->getType(sid) == OT_BRICK) {		
		Brick* data = (Brick*)_world->get_data(sid);
		if (data != 0) {
			--data->energy;
			LOG << "sid: " << sid << " energy: " << data->energy;
			if (data->energy <= 0) {
				v2 p = _world->getPosition(sid);
				Vector2i gp;
				if (convertToGrid(p, &gp)) {
					_board[gp.x][gp.y] = ds::INVALID_SID;
				}
				return 1;
			}
			_world->scaleByPath(sid, &_scalePath, 0.2f);
		}
	}
	return 0;
}

// -------------------------------------------------------
// clear
// -------------------------------------------------------
void Grid::clear() {
	ds::SID ids[256];
	int num = _world->find_by_type(OT_BRICK, ids, 256);
	for (int i = 0; i < num; ++i) {
		_world->remove(ids[i]);
	}
	num = _world->find_by_type(OT_NEW_BRICK, ids, 256);
	for (int i = 0; i < num; ++i) {
		_world->remove(ids[i]);
	}
	_limit = GMY - 1;
	for (int x = 0; x < GMX; ++x) {
		for (int y = 0; y < GMY; ++y) {
			_board[x][y] = ds::INVALID_SID;
		}
	}
}

// -------------------------------------------------------
// move down
// -------------------------------------------------------
bool Grid::moveDown() {
	if (_limit == 0) {
		return false;
	}
	// FIXME: check if we can even move down
	ds::SID ids[256];
	int num = _world->find_by_type(OT_BRICK, ids, 256);
	Vector2i gp;
	for (int i = 0; i < num; ++i) {
		v2 p = _world->getPosition(ids[i]);
		if (convertToGrid(p, &gp)) {
			_board[gp.x][gp.y] = ds::INVALID_SID;
			_board[gp.x][gp.y - 1] = ids[i];
			if (gp.y - 1 < 0) {
				// game over
			}
			else {
				_world->scaleTo(ids[i], v2(0.7f, 0.7f), v2(1, 1), 0.4f, 0, tweening::easeInOutQuad);
				_world->moveTo(ids[i], p, v2(p.x, p.y - GDY), 0.4f, 0, tweening::easeInOutQuad);
			}
		}
	}

	findLimit();

	num = _world->find_by_type(OT_NEW_BRICK, ids, 256);
	for (int i = 0; i < num; ++i) {
		v2 p = _world->getPosition(ids[i]);
		gp.y = 0;
		gp.x = (p.x - GSX) / GDX;
		int r = -1;
		for (int j = GMY - 1; j >= 0; --j) {
			if (r == -1 && _board[gp.x][j] != ds::INVALID_SID) {
				r = j;
			}
		}		
		r += 1;
		if (r == -1) {
			r = _limit;
		}
		if (r < _limit) {
			r = _limit;
		}
		LOG << "p.x " << gp.x << " r " << r;
		_board[gp.x][r] = ids[i];
		_world->setType(ids[i], OT_BRICK);
		_world->attachBoxCollider(ids[i], OT_BRICK, 0);
		v2 np;
		if (convertFromGrid(gp.x, r, &np)) {
			_world->moveTo(ids[i], p, np, 0.4f, 0, tweening::easeInOutQuad);
		}
	}
	return true;
	//debug();
}

// -------------------------------------------------------
// debug
// -------------------------------------------------------
void Grid::debug() {
	char buffer[32];
	LOG << "============================================================";
	for (int y = GMY - 1; y >= 0; --y) {
		std::string t;
		for (int x = 0; x < GMX; ++x) {
			if (_board[x][y] != ds::INVALID_SID) {
				sprintf_s(buffer, 32, "%3d", _board[x][y]);
				t.append(buffer);
			}
			else {
				t += "---";
			}
			t += " ";
		}
		LOG << t;
	}
}