#include "pch.h"
#include "ObjectManager.h"
#include "Object.h"
#include "BreakableBlock.h"
#include "SolidBlock.h"
#include "WaterBomb.h"
#include "Player.h"
#include "MyPlayer.h"

void ObjectManager::RegisterSyncObject(SyncObject* obj)
{
	uint64 id = obj->GetObjectId();
	_syncObjects[id] = obj;

	//Player* player = dynamic_cast<Player*>(obj);
	//if (player)
	//{
	//	_players[id] = player;
	//}

	//WaterBomb* bomb = dynamic_cast<WaterBomb*>(obj);
	//if (bomb)
	//{
	//	_bombs[id] = bomb;
	//}
}

void ObjectManager::UnregisterSyncObject(SyncObject* obj)
{
	uint64 id = obj->GetObjectId();
	_syncObjects[id] = nullptr;

	//Player* player = dynamic_cast<Player*>(obj);
	//if (player)
	//{
	//	_players[id] = nullptr;
	//}

	//WaterBomb* bomb = dynamic_cast<WaterBomb*>(obj);
	//if (bomb)
	//{
	//	_bombs[id] = nullptr;
	//}
}

Player* ObjectManager::SpawnPlayer(uint64 id, Pos pos)
{
	return nullptr;
}

WaterBomb* ObjectManager::SpawnBomb(uint64 id, Pos pos)
{
	return nullptr;
}

void ObjectManager::InitMap(int mapSizeX, int mapSizeY)
{
	_mapObjects = vector<vector<MapObject*>>(mapSizeY, vector<MapObject*>(mapSizeX));
}

MapObject* ObjectManager::SpawnMapObject(MAP_OBJECT_TYPE type, Vec2Int tilePos)
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

uint64 ObjectManager::GetMyPlayerId()
{
	return _myPlayer->GetObjectId();
}
