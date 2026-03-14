#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "GameRoom.h"
#include "PacketUtils.h"
#include "PacketHandler.h"

void GameSession::OnConnected()
{
	GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));

	Protocol::S_EnterGame pkt;
	// TEMP
	pkt.set_accountid(0);
	pkt.set_success(true);

	SendPacket(pkt);

	// 게임 입장
	GRoom->EnterRoom(GetSessionRef());
}

void GameSession::OnDisconnected()
{
	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));

	// 게임 나가기
	GRoom->LeaveRoom(GetSessionRef());
}

void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketHandler::HandlePacket(static_pointer_cast<GameSession>(shared_from_this()), buffer, len);
}

void GameSession::OnSend(int32 len)
{
	//cout << "OnSend Len = " << len << endl;
}