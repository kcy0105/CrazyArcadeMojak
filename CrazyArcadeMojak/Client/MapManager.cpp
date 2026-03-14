#include "pch.h"
#include "MapManager.h"
#include "Object.h"
#include "BreakableBlock.h"
#include "SolidBlock.h"
#include "WaterBomb.h"
#include "Player.h"

void MapManager::InitMap(int mapSizeX, int mapSizeY)
{
	_mapObjects = vector<vector<MapObject*>>(mapSizeY, vector<MapObject*>(mapSizeX));
}

MapObject* MapManager::SpawnMapObject(MAP_OBJECT_TYPE type, Vec2Int tilePos)
{
	MapObject* obj = nullptr;
	switch (type)
	{
	case MAP_OBJECT_TYPE_BREAKABLE_BLOCK:
		obj = Object::CreateObject<BreakableBlock>();
		break;
	case MAP_OBJECT_TYPE_SOLID_BLOCK:
		obj = Object::CreateObject<SolidBlock>();
		break;
	case MAP_OBJECT_TYPE_WATER_BOMB:
		obj = Object::CreateObject<WaterBomb>();
		break;
	}

	if (obj)
	{
		obj->SetPos(Utils::TileToWorld(tilePos));
		_mapObjects[tilePos.y][tilePos.x] = obj;
	}
		
	
	return obj;
}