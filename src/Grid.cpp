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
	ret->x = GSX + x * 70;
	ret->y = GSY + y * 40;
	return true;
}

bool convertToGrid(const v2& p, Vector2i* ret) {
	int x = (p.x - GSX) / 70;
	int y = (p.y - GSY) / 40;
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
// ---------------------------------------
// load cube definitions
// ---------------------------------------
bool BrickDefinitions::loadData(const ds::JSONReader& reader) {
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

Grid::Grid(GameContext* context) : _context(context), _world(context->world) {
	_scalePath.add(0.0f, v2(1, 1));
	_scalePath.add(0.4f, v2(0.7f, 0.7f));
	_scalePath.add(0.8f, v2(1.2f, 1.2f));
	_scalePath.add(1.0f, v2(1, 1));
	_brickDefinitions.load();
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
			v2 p = v2(200 + x * 70, 720);
			const BrickDefinition& def = _brickDefinitions.get(tmp[x]);
			ds::SID sid = _world->create(p, def.name);
			_world->setType(sid, OT_NEW_BRICK);
			_world->scaleTo(sid, v2(1.0f, 0.1f), v2(1, 1), 1.5f, 0, tweening::easeOutBounce);
			Brick* data = (Brick*)_world->attach_data(sid, sizeof(Brick));
			data->energy = def.energy;
			data->type = 1;
		}
	}
}

void Grid::buildLevel(int level) {
	for (int y = 0; y < 6; ++y) {
		for (int x = 0; x < 10; ++x) {
			v2 p = v2(200 + x * 70, 350 + y * 40);
			ds::SID sid = _world->create(p, "brick");
			_world->attachBoxCollider(sid, OT_BRICK, 0);
			//_world->moveTo(sid, v2(p.x, p.y + 600.0f), p, 1.5f + y * 0.1f, 0, tweening::easeOutBounce);
			//_world->scaleTo(sid, v2(1.0f, 0.1f), v2(1, 1), 1.5f, 0, tweening::easeOutBounce);
			Brick* data = (Brick*)_world->attach_data(sid, sizeof(Brick));
			data->energy = 2;
			data->type = 1;
		}
	}
}

void Grid::buildLevel(const Level& level) {
	v2 p;
	_limit = 0;
	for (int x = 0; x < GMX; ++x) {
		for (int y = 0; y < GMY; ++y) {
			_board[x][y] = ds::INVALID_SID;
		}
	}
	for (int y = 0; y < GMY; ++y) {
		for (int x = 0; x < GRID_SIZE_X; ++x) {
			int index = x + y * GRID_SIZE_X;
			_board[x][y] = ds::INVALID_SID;
			//if (level.grid[index] != -1) {
				if (convertFromGrid(x, y, &p)) {
					const BrickDefinition& def = _brickDefinitions.get(0);// level.grid[index] - 1);
					ds::SID sid = _world->create(p, def.name);
					_world->attachBoxCollider(sid, OT_BRICK, 0);
					//_world->moveTo(sid, v2(p.x, p.y + 700.0f), p, 1.5f + y * 0.1f, 0, tweening::easeOutBounce);
					//_world->scaleTo(sid, v2(1.0f, 0.1f), v2(1, 1), 1.5f, 0, tweening::easeOutBounce);
					Brick* data = (Brick*)_world->attach_data(sid, sizeof(Brick));
					data->energy = def.energy;
					data->type = 1;
					_board[x][y] = sid;
				}
			//}
		}
	}
	findLimit();
	debug();
}

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
			_limit = y;
		}
	}
	LOG << "LIMIT: " << _limit;
}

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
}



void Grid::moveDown() {
	
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
				_world->moveTo(ids[i], p, v2(p.x, p.y - 40.0f), 0.4f, 0, tweening::easeInOutQuad);
			}
		}
	}
	num = _world->find_by_type(OT_NEW_BRICK, ids, 256);
	for (int i = 0; i < num; ++i) {
		v2 p = _world->getPosition(ids[i]);
		gp.y = 0;
		gp.x = (p.x - GSX) / 70;
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
		LOG << "p.x " << gp.x << " r " << r;
		_board[gp.x][r] = ids[i];
		_world->setType(ids[i], OT_BRICK);
		_world->attachBoxCollider(ids[i], OT_BRICK, 0);
		v2 np;
		if (convertFromGrid(gp.x, r, &np)) {
			_world->moveTo(ids[i], p, np, 0.4f, 0, tweening::easeInOutQuad);
		}
	}
	findLimit();
	debug();
}

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