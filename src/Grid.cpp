#include "Grid.h"
#include "Constants.h"
#include <utils\Log.h>

Grid::Grid(GameContext* context) : _context(context), _world(context->world) {
	_scalePath.add(0.0f, v2(1, 1));
	_scalePath.add(0.4f, v2(0.7f, 0.7f));
	_scalePath.add(0.8f, v2(1.2f, 1.2f));
	_scalePath.add(1.0f, v2(1, 1));
}

Grid::~Grid() {
}

void Grid::buildLevel(int level) {
	for (int y = 0; y < 6; ++y) {
		for (int x = 0; x < 10; ++x) {
			v2 p = v2(200 + x * 70, 450 + y * 40);
			ds::SID sid = _world->create(p, "brick");
			_world->attachBoxCollider(sid, OT_BRICK, 0);
			_world->moveTo(sid, v2(p.x, p.y + 600.0f), p, 1.5f + y * 0.1f, 0, tweening::easeOutBounce);
			_world->scaleTo(sid, v2(1.0f, 0.1f), v2(1, 1), 1.5f, 0, tweening::easeOutBounce);
			Brick* data = (Brick*)_world->attach_data(sid, sizeof(Brick));
			data->energy = 2;
			data->type = 1;
		}
	}
}

int Grid::handleHit(ds::SID sid) {
	if (_world->contains(sid) && _world->getType(sid) == OT_BRICK) {		
		Brick* data = (Brick*)_world->get_data(sid);
		if (data != 0) {
			--data->energy;
			LOG << "sid: " << sid << " energy: " << data->energy;
			if (data->energy <= 0) {
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
}