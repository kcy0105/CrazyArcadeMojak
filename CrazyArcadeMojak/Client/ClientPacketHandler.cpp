#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "SceneManager.h"
#include "MapObject.h"
#include "ObjectManager.h"
#include "WaterBomb.h"
#include "Item.h"

void ClientPacketHandler::Handle_S_EnterGame(SessionRef session, Protocol::S_EnterGame& pkt)
{
	bool success = pkt.success();
	uint64 accountId = pkt.accountid();

	// TODO
}

void ClientPacketHandler::Handle_S_MyPlayer(SessionRef session, Protocol::S_MyPlayer& pkt)
{
	const Protocol::PlayerInfo& info = pkt.info();

	MyPlayer* myPlayer = Object::CreateObject<MyPlayer>();
	myPlayer->SetObjectId(info.objectid());
	myPlayer->SetPos({ info.posx(), info.posy() });
	myPlayer->SetDir((DIR)info.dir());
	myPlayer->SetMainState((PLAYER_STATE)info.mainstate());

	GET_SINGLE(ObjectManager)->SetMyPlayer(myPlayer);
}

void ClientPacketHandler::Handle_S_OtherPlayers(SessionRef session, Protocol::S_OtherPlayers& pkt)
{
	uint64 myPlayerId = GET_SINGLE(ObjectManager)->GetMyPlayerId();
	int32 size = pkt.infos_size();
	for (int32 i = 0; i < size; i++)
	{
		const Protocol::PlayerInfo& info = pkt.infos(i);
		if (myPlayerId == info.objectid())
			continue;

		Player* player = Object::CreateObject<Player>();
		player->SetObjectId(info.objectid());
		player->SetPos({ info.posx(), info.posy() });
		player->SetTargetPos({ info.posx(), info.posy() });
		player->SetDir((DIR)info.dir());
		player->SetMainState((PLAYER_STATE)info.mainstate());
	}
}

void ClientPacketHandler::Handle_S_PlayerLeave(SessionRef session, Protocol::S_PlayerLeave& pkt)
{
	auto object = GET_SINGLE(ObjectManager)->GetSyncObject(pkt.objectid());
	if (!object)
		return;

	auto player = dynamic_cast<Player*>(object);
	if (!player)
		return;

	Object::DestroyObject(player);
}

void ClientPacketHandler::Handle_S_Move(SessionRef session, Protocol::S_Move& pkt)
{
	Player* player = dynamic_cast<Player*>(GET_SINGLE(ObjectManager)->GetSyncObject(pkt.objectid()));
	if (player)
	{
		// 오차가 심한 경우 서버 위치로 세팅해야 할 수도.
		if (pkt.objectid() == GET_SINGLE(ObjectManager)->GetMyPlayerId())
		{
			if (pkt.needsync())
				player->SetPos({ pkt.posx(), pkt.posy() });

			return;
		}

		player->SetMoveState((MOVE_STATE)pkt.movestate());
		player->SetTargetPos({ pkt.posx(), pkt.posy() });
		player->SetDir((DIR)pkt.dir());
	}
}

void ClientPacketHandler::Handle_S_Tilemap(SessionRef session, Protocol::S_Tilemap& pkt)
{
	int32 mapSizeX = pkt.mapsizex();
	int32 mapSizeY = pkt.mapsizey();

	GET_SINGLE(ObjectManager)->InitMap(mapSizeX, mapSizeY);

	for (int y = 0; y < mapSizeY; y++)
	{
		for (int x = 0; x < mapSizeX; x++)
		{
			int i = y * mapSizeX + x;

			const Protocol::TileInfo& info = pkt.infos(i);

			if (info.mapobjecttype() != MAP_OBJECT_TYPE_NONE)
			{
				auto obj = GET_SINGLE(ObjectManager)->SpawnMapObject((MAP_OBJECT_TYPE)info.mapobjecttype(), info.detailedtype(), { x, y });
				obj->SetObjectId(info.objectid());
			}	
		}
	}
}

void ClientPacketHandler::Handle_S_WaterBomb(SessionRef session, Protocol::S_WaterBomb& pkt)
{
	uint64 objectid = pkt.objectid();
	uint64 ownerid = pkt.ownerid();
	int32 tileposx = pkt.tileposx();
	int32 tileposy = pkt.tileposy();

	MyPlayer* myPlayer = GET_SINGLE(ObjectManager)->GetMyPlayer();

	//if (ownerid == myPlayer->GetObjectId())
	//{
	//	auto bomb = myPlayer->pendingBombs.front();
	//	myPlayer->pendingBombs.pop();
	//	bomb->SetObjectId(objectid);
	//	return;
	//}


	auto bomb = static_cast<WaterBomb*>(GET_SINGLE(ObjectManager)->SpawnMapObject(MAP_OBJECT_TYPE_WATER_BOMB, 0, { tileposx, tileposy }));
	Player* player = static_cast<Player*>(GET_SINGLE(ObjectManager)->GetSyncObject(ownerid));
	bomb->SetOwner(player);
	bomb->SetObjectId(objectid);

	RECT r1 = myPlayer->GetRect();
	RECT r2 = bomb->GetRect();
	RECT r;

	if (::IntersectRect(&r, &r1, &r2))
	{
		bomb->SetPassable(true);
		myPlayer->AddOverlapBomb(bomb);
	}
}

void ClientPacketHandler::Handle_S_Explode(SessionRef session, Protocol::S_Explode& pkt)
{
	uint64 id = pkt.objectid();

	auto bomb = dynamic_cast<WaterBomb*>(GET_SINGLE(ObjectManager)->GetSyncObject(id));

	if (bomb)
	{
		bomb->Explode(pkt.up(), pkt.down(), pkt.left(), pkt.right());
	}


	/*===============================
	   Destroy Blocks & Spawn Items
	================================*/
	for (int i = 0; i < pkt.destroyedblockinfos_size(); i++)
	{
		Protocol::DestroyedBlockInfo info = pkt.destroyedblockinfos(i);

		Object::DestroyObject(GET_SINGLE(ObjectManager)->GetSyncObject(info.blockid()));
		// TODO : 아이템 생성
	}


	/*===============
	   Trap Players
	=================*/
	for (int i = 0; i < pkt.trappedplayerids_size(); i++)
	{
		uint64 id = pkt.trappedplayerids(i);
		auto player = dynamic_cast<Player*>(GET_SINGLE(ObjectManager)->GetSyncObject(id));
		if (player)
		{
			player->SetMainState(PLAYER_STATE_TRAPPED);
		}
	}

	// TODO : 아이템 부수기
}

void ClientPacketHandler::Handle_S_Dead(SessionRef session, Protocol::S_Dead& pkt)
{
	auto object = GET_SINGLE(ObjectManager)->GetSyncObject(pkt.objectid());
	
	if (!object)
		return;

	auto player = dynamic_cast<Player*>(object);
	if (!player)
		return;

	player->SetMainState(PLAYER_STATE_DEAD);
}

void ClientPacketHandler::Handle_S_RemoveItem(SessionRef session, Protocol::S_RemoveItem& pkt)
{
	auto object = GET_SINGLE(ObjectManager)->GetSyncObject(pkt.objectid());

	if (!object)
		return;

	auto item = dynamic_cast<Item*>(object);
	if (!item)
		return;

	Object::DestroyObject(item);
}

void ClientPacketHandler::Handle_S_PlayerNormalSpeed(SessionRef session, Protocol::S_PlayerNormalSpeed& pkt)
{
	if (GET_SINGLE(ObjectManager)->GetMyPlayerId() == pkt.objectid())
	{
		GET_SINGLE(ObjectManager)->GetMyPlayer()->SetNormalSpeed(pkt.normalspeed());
	}
}
