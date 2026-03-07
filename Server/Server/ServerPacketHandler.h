#pragma once

enum
{
	S_EnterGame = 1,

	S_MyPlayer = 4,
	S_AddObject = 5,
	S_RemoveObject = 6,

	C_Move = 10,
	S_Move = 11,

	S_Tilemap = 15,

	C_WaterBomb = 20,
};

class ServerPacketHandler
{
public:
	static void HandlePacket(GameSessionRef session, BYTE* buffer, int32 len);

	// 받기
	static void Handle_C_Move(GameSessionRef session, BYTE* buffer, int32 len);
	static void Handle_C_WaterBomb(GameSessionRef session, BYTE* buffer, int32 len);


	// 보내기
	static SendBufferRef Make_S_EnterGame();
	static SendBufferRef Make_S_MyPlayer(uint64 objectid, float posx, float posy, int32 dir, int32 state, float movespeed);
	static SendBufferRef Make_S_AddObject(const vector<uint64>& objectids, const vector<int32>& objecttypes, const vector<float>& posxs, const vector<float>& posys, const vector<int32>& states, const vector<int32>& dirs, const vector<float>& movespeeds);
	static SendBufferRef Make_S_RemoveObject(const vector<uint64>& objectids);
	static SendBufferRef Make_S_Move(uint64 objectid, int32 state, int32 dir, float posx, float posy, bool needsync);
	static SendBufferRef Make_S_Tilemap(int32 mapsizex, int32 mapsizey, const vector<vector<int32>>& values);


	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = make_shared<SendBuffer>(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		assert(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};