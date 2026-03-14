#pragma once
#include "PacketEnum.h"
#include "PacketUtils.h"
#include "ServerSession.h"

using ServerSessionRef = shared_ptr<class ServerSession>;

class NetworkManager
{
	DECLARE_SINGLE(NetworkManager)

public:
	void Init();
	void Update();

	ServerSessionRef CreateSession();
    template<typename T>
    void SendPacket(T& pkt)
    {
        if (_session == nullptr)
            return;

        SendBufferRef sendBuffer = PacketUtils::MakeSendBuffer(pkt);

        _session->Send(sendBuffer);
    }

private:
	ClientServiceRef _service;
	ServerSessionRef _session;
};

