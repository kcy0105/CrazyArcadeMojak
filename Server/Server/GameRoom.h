#pragma once

class Player;


class GameRoom : public enable_shared_from_this<GameRoom>
{
public:
	GameRoom() {}
	virtual ~GameRoom() {}

	void Init();
	void Update();

	void EnterRoom(GameSessionRef session);
	void LeaveRoom(GameSessionRef session);
	ObjectRef FindObject(uint64 id);
	GameRoomRef GetRoomRef() { return shared_from_this(); }

public:
	// PacketHandler
	void Handle_C_Move(Protocol::C_Move& pkt);
	void Handle_C_WaterBomb(Protocol::C_WaterBomb& pkt);

public:
	void AddObject(ObjectRef gameObject);
	void RemoveObject(uint64 id);
	void Broadcast(SendBufferRef& sendBuffer);

	void TryMove(Player& player, Pos nextPos);

private:
	void LoadTilemap(wstring path);

private:
	map<uint64, PlayerRef> _players;
	vector<vector<MapObjectRef>> _mapObjects;
	//Tilemap _tilemap;
};

extern GameRoomRef GRoom;