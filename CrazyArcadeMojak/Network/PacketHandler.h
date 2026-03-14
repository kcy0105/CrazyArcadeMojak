#pragma once

#include <functional>

using PacketHandlerFunc = std::function<void(SessionRef, BYTE*, int32)>;

extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

class PacketHandler
{
public:
	static void HandlePacket(SessionRef session, BYTE* buffer, int32 len);
};
