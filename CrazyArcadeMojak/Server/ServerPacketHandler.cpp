#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "GameSession.h"
#include "GameRoom.h"

void ServerPacketHandler::Handle_C_Move(SessionRef session, Protocol::C_Move& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);
	GameRoomRef room = gameSession->gameRoom.lock();
	if (room)
		room->Handle_C_Move(pkt);
}

void ServerPacketHandler::Handle_C_WaterBomb(SessionRef session, Protocol::C_WaterBomb& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);
	GameRoomRef room = gameSession->gameRoom.lock();
	if (room)
		room->Handle_C_WaterBomb(pkt);
}

void ServerPacketHandler::Handle_C_Skill(SessionRef session, Protocol::C_Skill& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);
	GameRoomRef room = gameSession->gameRoom.lock();
	if (room)
		room->Handle_C_Skill(pkt);
}