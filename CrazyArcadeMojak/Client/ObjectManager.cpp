#include "pch.h"
#include "ObjectManager.h"
#include "Object.h"
#include "BreakableBlock.h"
#include "SolidBlock.h"
#include "WaterBomb.h"
#include "Player.h"
#include "MyPlayer.h"
#include "Item.h"
#include "BubbleItem.h"
#include "FluidItem.h"
#include "RollerItem.h"
#include "NeedleItem.h"

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

MapObject* ObjectManager::SpawnMapObject(MAP_OBJECT_TYPE mapObjectype, int32 detailedType, Vec2Int tilePos)
{
	MapObject* obj = nullptr;
	switch (mapObjectype)
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
	case MAP_OBJECT_TYPE_ITEM:
		Item* item = nullptr;

		switch (detailedType)
		{
		case ITEM_TYPE_BUBBLE:
			item = Object::CreateObject<BubbleItem>();
			break;
		case ITEM_TYPE_FLUID:
			item = Object::CreateObject<FluidItem>();
			break;
		case ITEM_TYPE_ROLLER:
			item = Object::CreateObject<RollerItem>();
			break;
		case ITEM_TYPE_NEEDLE:
			item = Object::CreateObject<NeedleItem>();
			break;
		}

		obj = item;

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
