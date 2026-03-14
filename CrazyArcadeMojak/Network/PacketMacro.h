#define REGISTER_PACKET(ID, TYPE, FUNC) \
static PacketHandlerRegistry _reg_##ID( \
    MakeHandler<TYPE>(FUNC), ID)

template<typename PacketType, typename ProcessFunc>
PacketHandlerFunc MakeHandler(ProcessFunc func)
{
    return [func](SessionRef session, BYTE* buffer, int32 len)
        {
            PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

            PacketType pkt;
            pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));

            func(session, pkt);
        };
}