#include "pch.h"
#include "ClientPacketHandler.h"
#include "PacketHandlerRegistry.h"
#include "PacketEnum.h"
#include "PacketMacro.h"

REGISTER_PACKET(S_EnterGame, Protocol::S_EnterGame, ClientPacketHandler::Handle_S_EnterGame);
REGISTER_PACKET(S_MyPlayer, Protocol::S_MyPlayer, ClientPacketHandler::Handle_S_MyPlayer);
REGISTER_PACKET(S_OtherPlayers, Protocol::S_OtherPlayers, ClientPacketHandler::Handle_S_OtherPlayers);
REGISTER_PACKET(S_PlayerLeave, Protocol::S_PlayerLeave, ClientPacketHandler::Handle_S_PlayerLeave);
REGISTER_PACKET(S_Move, Protocol::S_Move, ClientPacketHandler::Handle_S_Move);
REGISTER_PACKET(S_Tilemap, Protocol::S_Tilemap, ClientPacketHandler::Handle_S_Tilemap);
REGISTER_PACKET(S_WaterBomb, Protocol::S_WaterBomb, ClientPacketHandler::Handle_S_WaterBomb);
REGISTER_PACKET(S_Explode, Protocol::S_Explode, ClientPacketHandler::Handle_S_Explode);
REGISTER_PACKET(S_Dead, Protocol::S_Dead, ClientPacketHandler::Handle_S_Dead);
