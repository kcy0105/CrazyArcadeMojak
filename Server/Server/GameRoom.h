#pragma once



class GameRoom : public enable_shared_from_this<GameRoom>
{
public:
	GameRoom();
	virtual ~GameRoom();

	void Init();
	void Update();

	void EnterRoom(GameSessionRef session);
	void LeaveRoom(GameSessionRef session);
	ObjectRef FindObject(uint64 id);
	GameRoomRef GetRoomRef() { return shared_from_this(); }

public:
	// PacketHandler
	//void Handle_C_Move(Protocol::C_Move& pkt);

public:
	void AddObject(ObjectRef gameObject);
	void RemoveObject(uint64 id);
	void Broadcast(SendBufferRef& sendBuffer);

private:
	map<uint64, PlayerRef> _players;
};

extern GameRoomRef GRoom;