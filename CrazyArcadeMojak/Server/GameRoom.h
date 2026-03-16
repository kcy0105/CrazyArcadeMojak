#pragma once

#include "Player.h"
#include "GameSession.h"

class GameRoom : public enable_shared_from_this<GameRoom>
{
public:
	GameRoom() {}
	virtual ~GameRoom() {}

	void Init();
	void Update();

	void EnterRoom(GameSessionRef session);
	void LeaveRoom(GameSessionRef session);
	GameRoomRef GetRoomRef() { return shared_from_this(); }

public:
	// PacketHandler
	void Handle_C_Move(Protocol::C_Move& pkt);
	void Handle_C_WaterBomb(Protocol::C_WaterBomb& pkt);

public:
	void RegisterObject(ObjectRef obj);
	void UnregisterObject(ObjectRef obj);

	void RemoveDeadObjects();

	PlayerRef SpawnPlayer();
	MapObjectRef SpawnMapObject(MAP_OBJECT_TYPE type, Vec2Int tilePos);

	template<typename T>
	void Broadcast(T& pkt)
	{
		for (auto& item : _players)
		{
			item.second->session->SendPacket(pkt);
		}
	}

	void TryMove(Player& player, Pos nextPos);

	

private:
	void LoadTilemap(wstring path);

private:

	unordered_map<uint64, ObjectRef> _objects;

	unordered_map<uint64, PlayerRef> _players;
	unordered_map<uint64, WaterBombRef> _bombs;

	vector<vector<MapObjectRef>> _mapObjects;
};

extern GameRoomRef GRoom;