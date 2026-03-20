#include "pch.h"
#include "ServerPacketHandler.h"
#include "PacketHandlerRegistry.h"
#include "PacketEnum.h"
#include "PacketMacro.h"

REGISTER_PACKET(C_Move, Protocol::C_Move, ServerPacketHandler::Handle_C_Move);
REGISTER_PACKET(C_WaterBomb, Protocol::C_WaterBomb, ServerPacketHandler::Handle_C_WaterBomb);
REGISTER_PACKET(C_Skill, Protocol::C_Skill, ServerPacketHandler::Handle_C_Skill);
