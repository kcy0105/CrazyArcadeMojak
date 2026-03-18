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

public:
	vector<weak_ptr<Player>> GetPlayers() { return _players; }
private:
	vector<weak_ptr<Player>> _players = {};
	vector<weak_ptr<WaterBomb>> _bombs = {};
	vector<vector<weak_ptr<MapObject>>> _mapObjects = {};

private:
	MapObjectRef GetMapObjectAt(Vec2Int tilePos);

private:
	void CleanupExpired();


	/*==========
	   Network
	===========*/
public:
	template<typename T>
	void Broadcast(T& pkt)
	{
		for (auto& p : _players)
		{
			auto player = p.lock();
			if (player)
				player->session->SendPacket(pkt);
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