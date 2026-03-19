#pragma once

class MapObject;
class SyncObject;
class Player;
class WaterBomb;
class MyPlayer;

class ObjectManager
{
	DECLARE_SINGLE(ObjectManager)

public:
	void RegisterSyncObject(SyncObject* obj);
	void UnregisterSyncObject(SyncObject* obj);
	SyncObject* GetSyncObject(uint64 id) { return _syncObjects[id]; }

private:
	unordered_map<uint64, SyncObject*> _syncObjects;


public:
	Player* SpawnPlayer(uint64 id, Pos pos);
	WaterBomb* SpawnBomb(uint64 id, Pos pos);

public:
	void InitMap(int mapSizeX, int mapSizeY);

	MapObject* SpawnMapObject(MAP_OBJECT_TYPE mapObjectype, int32 detailedType, Vec2Int tilePos);
	MapObject* GetMapObjectAt(Vec2Int tilePos) { return _mapObjects[tilePos.y][tilePos.x]; }
	void CleartMapObjectAt(Vec2Int tilePos) 
	{
		_mapObjects[tilePos.y][tilePos.x] = nullptr; 
	}

private:
	vector<vector<MapObject*>> _mapObjects = {};

public:
	void SetMyPlayer(MyPlayer* myPlayer) { _myPlayer = myPlayer; }
	MyPlayer* GetMyPlayer() { return _myPlayer; }
	uint64 GetMyPlayerId();

private:
	MyPlayer* _myPlayer = nullptr;
};

