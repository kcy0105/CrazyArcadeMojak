#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "SceneManager.h"
#include "MapObject.h"

void ClientPacketHandler::HandlePacket(ServerSessionRef session, BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	switch (header.id)
	{
		case S_EnterGame:
			Handle_S_EnterGame(session, buffer, len);
			break;
		case S_MyPlayer:
			Handle_S_MyPlayer(session, buffer, len);
			break;
		case S_AddObject:
			Handle_S_AddObject(session, buffer, len);
			break;
		case S_RemoveObject:
			Handle_S_RemoveObject(session, buffer, len);
			break;
		case S_Move:
			Handle_S_Move(session, buffer, len);
			break;
			break;
		case S_Tilemap:
			Handle_S_Tilemap(session, buffer, len);
			break;
	}
}


void ClientPacketHandler::Handle_S_EnterGame(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_EnterGame pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	bool success = pkt.success();
	uint64 accountId = pkt.accountid();

	// TODO

}

void ClientPacketHandler::Handle_S_MyPlayer(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_MyPlayer pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		MyPlayer* myPlayer = Object::CreateObject<MyPlayer>();
		myPlayer->SetObjectId(pkt.objectid());
		myPlayer->SetPos({ pkt.posx(), pkt.posy() });
		myPlayer->SetDir((DIR)pkt.dir());
		myPlayer->SetState((PLAYER_STATE)pkt.state());
		myPlayer->SetMoveSpeed((float)pkt.movespeed());
		scene->SetMyPlayer(myPlayer);
	}
}

void ClientPacketHandler::Handle_S_AddObject(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_AddObject pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();

	if (scene)
	{
		uint64 myPlayerId = scene->GetMyPlayerId();
		int32 objectSize = pkt.objectids_size();
		for (int32 i = 0; i < objectSize; i++)
		{
			if (myPlayerId == pkt.objectids(i))
				continue;

			if (pkt.objecttypes(i) == OBJECT_TYPE_PLAYER)
			{
				Player* player = Object::CreateObject<Player>();
				player->SetObjectId(pkt.objectids(i));
				player->SetPos({ pkt.posxs(i), pkt.posys(i) });
				player->SetDir((DIR)pkt.dirs(i));
				player->SetState((PLAYER_STATE)pkt.states(i));
				player->SetMoveSpeed((float)pkt.movespeeds(i));
			}
		}
	}
	
}

void ClientPacketHandler::Handle_S_RemoveObject(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_RemoveObject pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		int32 objectSize = pkt.objectids_size();
		for (int32 i = 0; i < objectSize; i++)
		{
			uint64 id = pkt.objectids(i);


			Object* object = scene->GetSyncObject(id);
			if (object)
				Object::DestroyObject(object);
		}
	}
}

void ClientPacketHandler::Handle_S_Move(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_Move pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		Player* player = scene->GetSyncObject(pkt.objectid());
		if (player)
		{
			// 오차가 심한 경우 서버 위치로 세팅해야 할 수도.
			if (pkt.objectid() == scene->GetMyPlayerId())
			{
				if (pkt.needsync())
					player->SetPos({ pkt.posx(), pkt.posy() });

				return;
			}

			player->SetState((PLAYER_STATE)pkt.state());
			player->SetDir((DIR)pkt.dir());
			player->SetPos({ pkt.posx(), pkt.posy() });
		}
	}
	
}

void ClientPacketHandler::Handle_S_Tilemap(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_Tilemap pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	int32 mapSizeX = pkt.mapsizex();
	int32 mapSizeY = pkt.mapsizey();

	for (int y = 0; y < mapSizeY; y++)
	{
		for (int x = 0; x < mapSizeX; x++)
		{
			int i = y * mapSizeX + x;
			int32 value = pkt.values(i);
			if (value > 0)
			{
				auto obj = MapObject::Factory((MAP_OBJECT_TYPE)value);
				obj->SetPos(Utils::TileToWorld({ x, y }));
			}

		}
	}
}

SendBufferRef ClientPacketHandler::Make_C_Move(uint64 objectid, int32 state, int32 dir, float posx, float posy)
{
	Protocol::C_Move pkt;

	pkt.set_objectid(objectid);
	pkt.set_state(state);
	pkt.set_dir(dir);
	pkt.set_posx(posx);
	pkt.set_posy(posy);

	return MakeSendBuffer(pkt, C_Move);
}

SendBufferRef ClientPacketHandler::Make_C_WaterBomb(float tileposx, float tileposy)
{
	Protocol::C_WaterBomb pkt;

	pkt.set_tileposx(tileposx);
	pkt.set_tileposy(tileposy);

	return MakeSendBuffer(pkt, C_WaterBomb);
}
