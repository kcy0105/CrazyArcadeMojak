#pragma once
#include "pch.h"

class PacketUtils
{
public:
    template<typename T>
    static SendBufferRef MakeSendBuffer(T& pkt)
    {
        const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
        const uint16 packetSize = dataSize + sizeof(PacketHeader);

        SendBufferRef sendBuffer = make_shared<SendBuffer>(packetSize);

        PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
        header->size = packetSize;
        header->id = PacketIdType<T>::value;

        assert(pkt.SerializeToArray(&header[1], dataSize));

        sendBuffer->Close(packetSize);

        return sendBuffer;
    }
};