#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "SceneManager.h"

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
		//case S_Move:
		//	Handle_S_Move(session, buffer, len);
		//	break;
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

	//
	const Protocol::ObjectInfo& info = pkt.info();

	DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetScene());
	if (scene)
	{
		MyPlayer* myPlayer = Object::CreateObject<MyPlayer>();
		myPlayer->info = info;
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

	DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetScene());
	if (scene)
	{
		uint64 myPlayerId = scene->GetMyPlayer()->GetId();
		const int32 size = pkt.objects_size();
		for (int32 i = 0; i < size; i++)
		{
			const Protocol::ObjectInfo& info = pkt.objects(i);
			if (myPlayerId == info.objectid())
				continue;

			if (info.objecttype() == Protocol::OBJECT_TYPE_PLAYER)
			{
				Player* player = Object::CreateObject<Player>();
				player->SetPos({ (float)info.posx(), (float)info.posy() });

				player->SetDir(info.dir());
				player->SetState(info.state());
				player->info = info;
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

	DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetScene());
	if (scene)
	{
		const int32 size = pkt.ids_size();
		for (int32 i = 0; i < size; i++)
		{
			uint64 id = pkt.ids(i);

			Object* object = scene->GetNetworkObject(id);
			if (object)
				Object::DestroyObject(object);
		}
	}
}

//void ClientPacketHandler::Handle_S_Move(ServerSessionRef session, BYTE* buffer, int32 len)
//{
//	PacketHeader* header = (PacketHeader*)buffer;
//	//uint16 id = header->id;
//	uint16 size = header->size;
//
//	Protocol::S_Move pkt;
//	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));
//	//
//	const Protocol::ObjectInfo& info = pkt.info();
//
//	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
//	if (scene)
//	{
//		uint64 myPlayerId = GET_SINGLE(SceneManager)->GetMyPlayerId();
//		if (myPlayerId == info.objectid())
//			return;
//
//		GameObject* gameObject = scene->GetObject(info.objectid());
//		if (gameObject)
//		{
//			gameObject->SetDir(info.dir());
//			gameObject->SetState(info.state());
//			gameObject->SetCellPos(Vec2Int{ info.posx(), info.posy() });
//		}
//	}
//}

SendBufferRef ClientPacketHandler::Make_C_Move()
{
	Protocol::C_Move pkt;

	DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetScene());
	MyPlayer* myPlayer = scene->GetMyPlayer();

	*pkt.mutable_info() = myPlayer->info;

	return MakeSendBuffer(pkt, C_Move);
}
