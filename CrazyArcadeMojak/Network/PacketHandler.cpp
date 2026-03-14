#include "pch.h"
#include "PacketHandler.h"



void PacketHandler::HandlePacket(SessionRef session, BYTE* buffer, int32 len)
{
    PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

    auto& handler = GPacketHandler[header->id];

    if (handler)
        handler(session, buffer, len);
}