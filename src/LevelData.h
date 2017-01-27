#pragma once
#include <core\io\DataFile.h>

class LevelData : public ds::JSONAssetFile {

public:
	LevelData() : ds::JSONAssetFile("content\\levels.json") {}
	virtual ~LevelData();
	bool loadData(const ds::JSONReader& loader);
	bool reloadData(const ds::JSONReader& loader);
	int* getDataPtr(int index);
private:
	int* _data;
};

