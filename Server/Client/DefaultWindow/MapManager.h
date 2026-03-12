#pragma once

class MapObject;

class MapManager
{
	DECLARE_SINGLE(MapManager)

public:
	void InitMap(int mapSizeX, int mapSizeY);

	MapObject* SpawnMapObject(MAP_OBJECT_TYPE type, Vec2Int tilePos);
	MapObject* GetMapObjectAt(Vec2Int tilePos) { return _mapObjects[tilePos.y][tilePos.x]; }

private:
	vector<vector<MapObject*>> _mapObjects = {};
};

