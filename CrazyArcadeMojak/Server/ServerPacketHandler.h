#pragma once

#include "Protocol.pb.h"

class ServerPacketHandler
{
public:

    static void Handle_C_Move(SessionRef session, Protocol::C_Move& pkt);

    static void Handle_C_WaterBomb(SessionRef session, Protocol::C_WaterBomb& pkt);

    static void Handle_C_Skill(SessionRef session, Protocol::C_Skill& pkt);

};
