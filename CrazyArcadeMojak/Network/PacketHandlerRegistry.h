#pragma once
#include "pch.h"
#include "PacketHandler.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

class PacketHandlerRegistry
{
public:
    PacketHandlerRegistry(PacketHandlerFunc func, uint16 id)
    {
        GPacketHandler[id] = func;
    }
};