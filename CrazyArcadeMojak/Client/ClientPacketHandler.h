#pragma once

#include "Protocol.pb.h"

class ClientPacketHandler
{
public:

    static void Handle_S_EnterGame(SessionRef session, Protocol::S_EnterGame& pkt);

    static void Handle_S_MyPlayer(SessionRef session, Protocol::S_MyPlayer& pkt);

    static void Handle_S_OtherPlayers(SessionRef session, Protocol::S_OtherPlayers& pkt);

    static void Handle_S_PlayerLeave(SessionRef session, Protocol::S_PlayerLeave& pkt);

    static void Handle_S_Move(SessionRef session, Protocol::S_Move& pkt);

    static void Handle_S_Tilemap(SessionRef session, Protocol::S_Tilemap& pkt);

    static void Handle_S_WaterBomb(SessionRef session, Protocol::S_WaterBomb& pkt);

    static void Handle_S_Explode(SessionRef session, Protocol::S_Explode& pkt);

    static void Handle_S_Dead(SessionRef session, Protocol::S_Dead& pkt);

    static void Handle_S_RemoveItem(SessionRef session, Protocol::S_RemoveItem& pkt);

    static void Handle_S_PlayerNormalSpeed(SessionRef session, Protocol::S_PlayerNormalSpeed& pkt);

};
