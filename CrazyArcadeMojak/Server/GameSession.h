#pragma once
#include "Session.h"
#include "PacketEnum.h"
#include "PacketUtils.h"

class GameSession : public PacketSession
{
public:
	~GameSession()
	{
		cout << "~GameSession" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

	GameSessionRef GetSessionRef() { return static_pointer_cast<GameSession>(shared_from_this()); }

	template<typename T>
	void SendPacket(T& pkt)
	{
		SendBufferRef sendBuffer = PacketUtils::MakeSendBuffer(pkt);
		Send(sendBuffer);
	}

public:
	weak_ptr<GameRoom> gameRoom;
	weak_ptr<Player> player;
};