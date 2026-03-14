#pragma once

#include "Protocol.pb.h"

class ClientPacketHandler
{
public:

    static void Handle_S_EnterGame(SessionRef session, Protocol::S_EnterGame& pkt);

    static void Handle_S_MyPlayer(SessionRef session, Protocol::S_MyPlayer& pkt);

    static void Handle_S_AddObject(SessionRef session, Protocol::S_AddObject& pkt);

    static void Handle_S_RemoveObject(SessionRef session, Protocol::S_RemoveObject& pkt);

    static void Handle_S_Move(SessionRef session, Protocol::S_Move& pkt);

    static void Handle_S_Tilemap(SessionRef session, Protocol::S_Tilemap& pkt);

    static void Handle_S_WaterBomb(SessionRef session, Protocol::S_WaterBomb& pkt);

};
