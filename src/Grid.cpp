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
	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 10; ++x) {
			v2 p = v2(500 + x * 40, 150 + y * 70);
			ds::SID sid = _world->create(p, "brick");
			_world->attachBoxCollider(sid, OT_BRICK, 0);
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