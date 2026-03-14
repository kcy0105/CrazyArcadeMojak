#include "pch.h"
#include "ClientPacketHandler.h"
#include "PacketHandlerRegistry.h"
#include "PacketEnum.h"
#include "PacketMacro.h"

REGISTER_PACKET(S_EnterGame, Protocol::S_EnterGame, ClientPacketHandler::Handle_S_EnterGame);
REGISTER_PACKET(S_MyPlayer, Protocol::S_MyPlayer, ClientPacketHandler::Handle_S_MyPlayer);
REGISTER_PACKET(S_AddObject, Protocol::S_AddObject, ClientPacketHandler::Handle_S_AddObject);
REGISTER_PACKET(S_RemoveObject, Protocol::S_RemoveObject, ClientPacketHandler::Handle_S_RemoveObject);
REGISTER_PACKET(S_Move, Protocol::S_Move, ClientPacketHandler::Handle_S_Move);
REGISTER_PACKET(S_Tilemap, Protocol::S_Tilemap, ClientPacketHandler::Handle_S_Tilemap);
REGISTER_PACKET(S_WaterBomb, Protocol::S_WaterBomb, ClientPacketHandler::Handle_S_WaterBomb);
