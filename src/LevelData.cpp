#include "LevelData.h"
#include "Constants.h"

LevelData::~LevelData() {
	delete[] _data;
}

bool LevelData::loadData(const ds::JSONReader& loader) {
	int levels[64];
	int num = loader.get_categories(levels, 64);
	_data = new int[GRID_X * GRID_Y * num];
	char line[16];
	for (int i = 0; i < num; ++i) {
		int idx = 0;
		loader.get_int(levels[i], "index", &idx);
		for (int j = 0; j < 8; ++j) {
			sprintf_s(line, "r%d", (7 - j));
			const char* str = loader.get_string(levels[i], line);
			int dataIdx = GRID_X * GRID_Y * idx + j * GRID_X;
			for (int k = 0; k < 10; ++k) {
				if (str[k] == '-') {
					_data[dataIdx + k] = 0;
				}
				else {
					_data[dataIdx + k] = str[k] - '0';
				}
			}
		}
	}
	return true;
}

bool LevelData::reloadData(const ds::JSONReader& loader) {
	delete[] _data;
	return loadData(loader);
}

int* LevelData::getDataPtr(int index) {
	return _data + GRID_X * GRID_Y * index;
}