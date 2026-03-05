#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "GameSession.h"
#include "GameRoom.h"

void ServerPacketHandler::HandlePacket(GameSessionRef session, BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br.Peek(&header);

	switch (header.id)
	{
	case C_Move:
		Handle_C_Move(session, buffer, len);
		break;

	default:
		break;
	}
}

void ServerPacketHandler::Handle_C_Move(GameSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::C_Move pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//if (pkt.state() == PLAYER_STATE_MOVE)
	//	cout << "\nReceived : Move : " << pkt.dir() << endl;
	//else
	//	cout << "\nReceived : Stop" << endl;

	GameRoomRef room = session->gameRoom.lock();
	if (room)
		room->Handle_C_Move(pkt);

	
}

SendBufferRef ServerPacketHandler::Make_S_EnterGame()
{
	Protocol::S_EnterGame pkt;

	pkt.set_success(true);
	pkt.set_accountid(0); // DB

	return MakeSendBuffer(pkt, S_EnterGame);
}

SendBufferRef ServerPacketHandler::Make_S_MyPlayer(uint64 objectid, float posx, float posy, int32 dir, int32 state, float movespeed)
{
	Protocol::S_MyPlayer pkt;

	pkt.set_objectid(objectid);
	pkt.set_posx(posx);
	pkt.set_posy(posy);
	pkt.set_dir(dir);
	pkt.set_state(state);
	pkt.set_movespeed(movespeed);

	return MakeSendBuffer(pkt, S_MyPlayer);
}

SendBufferRef ServerPacketHandler::Make_S_AddObject(const vector<uint64>& objectids, const vector<int32>& objecttypes, const vector<float>& posxs, const vector<float>& posys, const vector<int32>& states, const vector<int32>& dirs, const vector<float>& movespeeds)
{
	Protocol::S_AddObject pkt;

	for (auto objectid : objectids)
		pkt.add_objectids(objectid);

	for (auto objecttype : objecttypes)
		pkt.add_objecttypes(objecttype);

	for (auto posx : posxs)
		pkt.add_posxs(posx);

	for (auto posy : posys)
		pkt.add_posys(posy);

	for (auto state : states)
		pkt.add_states(state);

	for (auto dir : dirs)
		pkt.add_dirs(dir);

	for (auto movespeed : movespeeds)
		pkt.add_movespeeds(movespeed);

	return MakeSendBuffer(pkt, S_AddObject);
}

SendBufferRef ServerPacketHandler::Make_S_RemoveObject(const vector<uint64>& objectids)
{
	Protocol::S_RemoveObject pkt;

	for (uint64 objectid : objectids)
		pkt.add_objectids(objectid);

	return MakeSendBuffer(pkt, S_RemoveObject);
}

SendBufferRef ServerPacketHandler::Make_S_Move(uint64 objectid, int32 state, int32 dir, float posx, float posy, bool needsync)
{
	Protocol::S_Move pkt;

	pkt.set_objectid(objectid);
	pkt.set_state(state);
	pkt.set_dir(dir);
	pkt.set_posx(posx);
	pkt.set_posy(posy);
	pkt.set_needsync(needsync);

	return MakeSendBuffer(pkt, S_Move);
}


SendBufferRef ServerPacketHandler::Make_S_Tilemap(int32 mapsizex, int32 mapsizey, int32 tilesize, const vector<vector<int32>>& values)
{
	Protocol::S_Tilemap pkt;

	pkt.set_mapsizex(mapsizex);
	pkt.set_mapsizey(mapsizey);
	pkt.set_tilesize(tilesize);

	for (int y = 0; y < mapsizey; y++)
	{
		for (int x = 0; x < mapsizex; x++)
		{
			pkt.add_values(values[y][x]);
		}
	}

	return MakeSendBuffer(pkt, S_Tilemap);
}
