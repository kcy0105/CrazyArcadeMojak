#pragma once

#include "Player.h"
#include "GameSession.h"

class GameRoom : public enable_shared_from_this<GameRoom>
{
public:
	GameRoom() {}
	virtual ~GameRoom() {}

	/*============
	   Game Loop
	=============*/
	void Init();
	void Update();


	/*================
	   Enter & Leave
	=================*/
	void EnterRoom(GameSessionRef session);
	void LeaveRoom(GameSessionRef session);
	

	/*==========
	    Utils
	===========*/
	GameRoomRef GetRoomRef() { return shared_from_this(); }


	/*================
	   PacketHandler
	=================*/
public:
	void Handle_C_Move(Protocol::C_Move& pkt);
	void Handle_C_WaterBomb(Protocol::C_WaterBomb& pkt);


	/*=====================
	   Object Manage Base
	======================*/
public:
	void RegisterObject(ObjectRef obj);
	void UnregisterObject(ObjectRef obj);
	void RemoveDeadObjects();

private:
	unordered_map<uint64, ObjectRef> _objects;


	/*======================
	   Object Manage Plus
	=======================*/
private:
	PlayerRef SpawnPlayer();
	MapObjectRef SpawnMapObject(MAP_OBJECT_TYPE type, Vec2Int tilePos);
private:
	unordered_map<uint64, PlayerRef> _players;
	unordered_map<uint64, WaterBombRef> _bombs;
	vector<vector<MapObjectRef>> _mapObjects;

private:
	MapObjectRef GetMapObjectAt(Vec2Int tilePos);


	/*==========
	   Network
	===========*/
public:
	template<typename T>
	void Broadcast(T& pkt)
	{
		for (auto& item : _players)
		{
			item.second->session->SendPacket(pkt);
		}
	}


	/*============
	   Game Play
	=============*/
public:
	void TryMove(Player& player, Pos nextPos);
	void Explode(WaterBomb& bomb);

	
	/*==========
	    Load
	===========*/
private:
	void LoadTilemap(wstring path);


};

extern GameRoomRef GRoom;