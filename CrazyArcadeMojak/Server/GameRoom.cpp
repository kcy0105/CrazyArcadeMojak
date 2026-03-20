#include "pch.h"
#include "GameRoom.h"
#include "Player.h"
#include "GameSession.h"
#include "BreakableBlock.h"
#include "SolidBlock.h"
#include "WaterBomb.h"
#include "Item.h"
#include "BubbleItem.h"
#include "FluidItem.h"
#include "RollerItem.h"
#include "NeedleItem.h"

GameRoomRef GRoom = make_shared<GameRoom>();

void GameRoom::Init()
{
	LoadTilemap(L"C:\\Users\\user\\바탕 화면\\CrazyArcadeMojak\\CrazyArcadeMojak\\Resources\\Tilemap.txt");

	// TEMP
	SpawnMapObject(MAP_OBJECT_TYPE_ITEM, ITEM_TYPE_BUBBLE, { 10, 10 });
	SpawnMapObject(MAP_OBJECT_TYPE_ITEM, ITEM_TYPE_FLUID,	{ 11, 10 });
	SpawnMapObject(MAP_OBJECT_TYPE_ITEM, ITEM_TYPE_ROLLER, { 12, 10 });
	SpawnMapObject(MAP_OBJECT_TYPE_ITEM, ITEM_TYPE_NEEDLE, { 13, 10 });
}

void GameRoom::Update()
{
	for (auto& item : _objects)
	{
		item.second->Update();
	}

	RemoveDeadObjects();

	// TODO : vector 청소
}

void GameRoom::EnterRoom(GameSessionRef session)
{
	PlayerRef player = SpawnPlayer();

	// 서로의 존재를 연결
	session->gameRoom = GetRoomRef();
	session->player = player;
	player->session = session;

	/*====================
	   Initialize Player
	=====================*/
	player->SetPos({ 100, 100 });
	player->SetDir(DIR_DOWN);
	player->SetMainState(PLAYER_STATE_NORMAL, false);


	/*================
	   Send MyPlayer
	=================*/
	Protocol::S_MyPlayer myPlayerPkt;
	Protocol::PlayerInfo* myInfo = myPlayerPkt.mutable_info();
	{
		myInfo->set_objectid(player->GetObjectId());
		myInfo->set_posx(player->GetPos().x);
		myInfo->set_posy(player->GetPos().y);
		myInfo->set_dir(player->GetDir());
		myInfo->set_mainstate(player->GetMainState());

		session->SendPacket(myPlayerPkt);
	}


	/*=====================
	   Send Other Players
	======================*/
	{
		Protocol::S_OtherPlayers pkt;

		for (auto& p : _players)
		{
			PlayerRef player = p.lock();
			if (!player) continue;
			
			Protocol::PlayerInfo* info = pkt.add_infos();
			info->set_objectid(player->GetObjectId());
			info->set_posx((player->GetPos().x));
			info->set_posy((player->GetPos().y));
			info->set_mainstate(player->GetMainState());
			info->set_dir(player->GetDir());
		}

		session->SendPacket(pkt);
	}


	/*===============
	   Send Tilemap
	================*/
	{
		Protocol::S_Tilemap pkt;

		pkt.set_mapsizex(_mapObjects[0].size());
		pkt.set_mapsizey(_mapObjects.size());
		for (int y = 0; y < pkt.mapsizey(); y++)
		{
			for (int x = 0; x < pkt.mapsizex(); x++)
			{
				auto obj = GetMapObjectAt({ x, y });
				Protocol::TileInfo* info = pkt.add_infos();
				if (obj)
				{
					info->set_objectid(obj->GetObjectId());
					info->set_mapobjecttype(obj->GetMapObjectType());

					switch (obj->GetMapObjectType())
					{
					case MAP_OBJECT_TYPE_ITEM:
					{
						auto item = dynamic_pointer_cast<Item>(obj);
						if (item)
							info->set_detailedtype(item->GetItemType());
					}
						break;
					}
				}
				else
				{
					info->set_objectid(0);
					info->set_mapobjecttype(MAP_OBJECT_TYPE_NONE);
				}
			}
		}
		
		session->SendPacket(pkt);
	}


	/*=====================
	   Broadcast MyPlayer 
	======================*/
	{
		Protocol::S_OtherPlayers pkt;
		Protocol::PlayerInfo* info = pkt.add_infos();
		info->CopyFrom(*myInfo);
		Broadcast(pkt);
	}
}

void GameRoom::LeaveRoom(GameSessionRef session)
{
	if (session == nullptr)
		return;
	if (session->player.lock() == nullptr)
		return;

	session->player.lock()->Destroy();
	uint64 id = session->player.lock()->GetObjectId();

	{
		Protocol::S_PlayerLeave pkt;
		pkt.set_objectid(id);
		Broadcast(pkt);
	}
}

void GameRoom::Handle_C_Move(Protocol::C_Move& pkt)
{
	uint64 id = pkt.objectid();
	
	auto obj = _objects[id];
	if (!obj) return;
	auto player = dynamic_pointer_cast<Player>(obj);
	if (!player) return;

	if (player->GetMainState() == PLAYER_STATE_DEAD) return;

	player->SetMoveState((MOVE_STATE)pkt.movestate());
	player->SetDir((DIR)pkt.dir());

	Pos serverPos = player->GetPos();
	Pos clientPos = { pkt.posx(), pkt.posy() };

	bool needsync = false;

	if ((serverPos - clientPos).Length() < MAX_POSITION_ERROR)
	{
		player->SetPos({ pkt.posx(), pkt.posy() });
	}
	else
	{
		cout << "Need Sync" << endl;
		needsync = true;
	}

	{
		Protocol::S_Move pkt;
		pkt.set_objectid(player->GetObjectId());
		pkt.set_movestate(player->GetMoveState());
		pkt.set_dir(player->GetDir());
		pkt.set_posx(player->GetPos().x);
		pkt.set_posy(player->GetPos().y);
		pkt.set_needsync(needsync);

		Broadcast(pkt);
	}
}

void GameRoom::Handle_C_WaterBomb(Protocol::C_WaterBomb& pkt)
{
	uint64 ownerId = pkt.ownerid();
	int32 tilePosX = pkt.tileposx();
	int32 tilePosY = pkt.tileposy();

	auto ownerObj = _objects[ownerId];

	if (!ownerObj)
		return;

	auto owner = dynamic_pointer_cast<Player>(ownerObj);
	if (!owner)
		return;

	// 설치 가능 타이밍인가?
	if (!owner->CanSpawnBomb()) return;

	// 설치할 수 있는 위치인가?
	if (GetMapObjectAt({tilePosX, tilePosY})) return;


	auto bomb = static_pointer_cast<WaterBomb>(SpawnMapObject(MAP_OBJECT_TYPE_WATER_BOMB, 0, { tilePosX, tilePosY }));
	bomb->SetOwner(owner);

	owner->AddBombCount();

	for (auto& p : _players)
	{
		auto player = p.lock();
		if (!player) continue;

		RECT r1 = player->GetRect();
		RECT r2 = bomb->GetRect();
		RECT r;

		if (::IntersectRect(&r, &r1, &r2))
		{
			bomb->AddPassablePlayer(player);
			player->AddOverlapBomb(bomb);
		}
	}

	{
		Protocol::S_WaterBomb pkt;
		pkt.set_objectid(bomb->GetObjectId());
		pkt.set_ownerid(ownerId);
		pkt.set_tileposx(tilePosX);
		pkt.set_tileposy(tilePosY);

		Broadcast(pkt);
	}
}

void GameRoom::Handle_C_Skill(Protocol::C_Skill& pkt)
{
	ObjectRef obj = _objects[pkt.playerid()];
	if (!obj) return;

	auto player = dynamic_pointer_cast<Player>(obj);
	if (!player) return;

	if (player->GetHasNeedle())
	{
		if (player->GetMainState() == PLAYER_STATE_TRAPPED)
		{
			player->SetHasNeedle(false);

			player->SetMainState(PLAYER_STATE_ESCAPE, true);
		}
	}
}

void GameRoom::RegisterObject(ObjectRef obj)
{
	_objects[obj->GetObjectId()] = obj;
	if (!obj->room)
		obj->room = shared_from_this();
}

void GameRoom::UnregisterObject(ObjectRef obj)
{
	if (!obj)
		return;

	uint64 id = obj->GetObjectId();
	_objects.erase(id);

	obj->room = nullptr;
}

void GameRoom::RemoveDeadObjects()
{
	const auto objects = _objects;
	for (auto& item : objects)
	{
		uint64 id = item.first;

		if (item.second->IsDead())
		{
			UnregisterObject(item.second);
		}
	}
}

void GameRoom::TryMove(Player& player, Pos nextPos)
{
	Pos pos = nextPos;
	float half = PLAYER_SIZE / 2;

	RECT playerRect =
	{
		(LONG)(nextPos.x - half),
		(LONG)(nextPos.y - half),
		(LONG)(nextPos.x + half),
		(LONG)(nextPos.y + half)
	};

	int32 minTileX = playerRect.left / TILE_SIZE;
	int32 maxTileX = (playerRect.right - 1) / TILE_SIZE;
	int32 minTileY = playerRect.top / TILE_SIZE;
	int32 maxTileY = (playerRect.bottom - 1) / TILE_SIZE;

	bool found = false;

	for (int32 y = minTileY; y <= maxTileY; y++)
	{
		for (int32 x = minTileX; x <= maxTileX; x++)
		{
			MapObjectRef obj = GetMapObjectAt({ x, y });

			if (!obj)
				continue;

			if (!obj->BlocksPlayer(&player))
				continue;

			RECT tileRect =
			{
				x * TILE_SIZE,
				y * TILE_SIZE,
				x * TILE_SIZE + TILE_SIZE,
				y * TILE_SIZE + TILE_SIZE
			};

			RECT intersect;
			if (::IntersectRect(&intersect, &playerRect, &tileRect))
			{
				if (player.GetPos().x == nextPos.x)
				{
					if (nextPos.y > player.GetPos().y)
						pos.y = tileRect.top - half;
					else
						pos.y = tileRect.bottom + half;
				}
				else
				{
					if (nextPos.x > player.GetPos().x)
						pos.x = tileRect.left - half;
					else
						pos.x = tileRect.right + half;
				}
				found = true;
				break;
			}
		}
		if (found) break;
	}

	player.SetPos(pos);
}

void GameRoom::Explode(WaterBomb& bomb)
{
	bomb.SetExploded(true);

	bomb.GetOwner()->SubBombCount();

	Vec2Int bombPos = bomb.GetTilePos();

	Protocol::S_Explode pkt;
	pkt.set_objectid(bomb.GetObjectId());

	
	/*===================
	   Calculate Range
	====================*/
	vector<Vec2Int> dirs = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} }; // up down left right
	vector<uint8> counts(4, bomb.GetRange());

	for (int i=0; i<4; i++)
	{
		Vec2Int dir = dirs[i];
		for (int j = 1; j <= bomb.GetRange(); j++)
		{
			Vec2Int checkPos = bombPos + dir * j;

			bool end = false;

			MapObjectRef mapObject = GetMapObjectAt(checkPos);
			if (mapObject)
			{
				switch (mapObject->GetMapObjectType())
				{
				case MAP_OBJECT_TYPE_SOLID_BLOCK:
				{
					counts[i] = j - 1;
					end = true;
				}
					break;
				case MAP_OBJECT_TYPE_BREAKABLE_BLOCK:
				{
					counts[i] = j;
					end = true;
				}
					break;
				}
			}

			if (end)
				break;
		}
	}

	/*==========================
	   Handle Objects In Range
	===========================*/
	vector<Vec2Int> explodePoses = {bombPos};
	for (int i = 0; i < 4; i++)
	{
		Vec2Int dir = dirs[i];
		for (int j = 1; j <= counts[i]; j++)
		{
			explodePoses.push_back(bombPos + dir * j);
		}
	}


	for (Vec2Int explodePos : explodePoses)
	{
		/*===============
		   Trap Players
		================*/
		for (auto& p : _players)
		{
			PlayerRef player = p.lock();

			if (!player) continue;

			Vec2 explodeWorldPos = Utils::TileToWorld(explodePos);

			RECT explodeRect = { explodeWorldPos.x - TILE_SIZE / 2, explodeWorldPos.y - TILE_SIZE / 2, explodeWorldPos.x + TILE_SIZE / 2, explodeWorldPos.y + TILE_SIZE / 2 };

			if (::PtInRect(&explodeRect, {(LONG)player->GetPos().x, (LONG)player->GetPos().y}))
			{
				player->SetMainState(PLAYER_STATE_TRAPPED, true);
			}
		}


		auto mapObject = GetMapObjectAt(explodePos);
		if (!mapObject) continue;

		/*================
		   Destroy Items
		=================*/
		if (mapObject->GetMapObjectType() == MAP_OBJECT_TYPE_ITEM)
		{
			pkt.add_destroyeditemids(mapObject->GetObjectId());
			mapObject->Destroy();
		}


		/*===============================
		   Destroy Blocks & Spawn Items
		=================================*/
		if (mapObject->GetMapObjectType() == MAP_OBJECT_TYPE_BREAKABLE_BLOCK)
		{
			Protocol::DestroyedBlockInfo* info = pkt.add_destroyedblockinfos();
			info->set_blockid(mapObject->GetObjectId());
			info->set_itemid(0); // TODO : 아이템 스폰
			mapObject->Destroy();
		}


		/*================
		   Explode Bombs
		=================*/
		auto otherBomb = static_pointer_cast<WaterBomb>(mapObject);
		if (otherBomb->GetExploded() == false)
		{
			otherBomb->Explode();
		}
	}

	pkt.set_up(counts[0]);
	pkt.set_down(counts[1]);
	pkt.set_left(counts[2]);
	pkt.set_right(counts[3]);

	Broadcast(pkt);
}

void GameRoom::LoadTilemap(wstring path)
{
	FILE* file = nullptr;

	::_wfopen_s(&file, path.c_str(), L"rb");
	assert(file);

	int32 mapSizeX;
	int32 mapSizeY;

	::fread(&mapSizeX, sizeof(mapSizeX), 1, file);
	::fread(&mapSizeY, sizeof(mapSizeY), 1, file);

	_mapObjects = vector<vector<weak_ptr<MapObject>>>(mapSizeY, vector<weak_ptr<MapObject>>(mapSizeX));

	for (int32 y = 0; y < mapSizeY; y++)
	{
		for (int32 x = 0; x < mapSizeX; x++)
		{
			int32 value = -1;
			::fread(&value, sizeof(value), 1, file);


			SpawnMapObject((MAP_OBJECT_TYPE)value, 0, { x, y });
		}
	}

	::fclose(file);
}

PlayerRef GameRoom::SpawnPlayer()
{
	PlayerRef player = make_shared<Player>();

	RegisterObject(player);
	_players.push_back(player);

	return player;
}

MapObjectRef GameRoom::SpawnMapObject(MAP_OBJECT_TYPE mapObjectType, int32 detailedType, Vec2Int tilePos)
{
	MapObjectRef obj = nullptr;

	switch (mapObjectType)
	{
	case MAP_OBJECT_TYPE_BREAKABLE_BLOCK:
	{
		auto breakableBlock = make_shared<BreakableBlock>();
		_breakableBlocks.push_back(breakableBlock);
		obj = breakableBlock;
	}
	break;
	case MAP_OBJECT_TYPE_SOLID_BLOCK:
		obj = make_shared<SolidBlock>();
		break;
	case MAP_OBJECT_TYPE_WATER_BOMB:
	{
		auto bomb = make_shared<WaterBomb>();
		_bombs.push_back(bomb);
		obj = bomb;
	}
	break;
	case MAP_OBJECT_TYPE_ITEM:
	{
		ItemRef item = nullptr;

		switch (detailedType)
		{
		case ITEM_TYPE_BUBBLE:
			item = make_shared<BubbleItem>();
			break;
		case ITEM_TYPE_FLUID:
			item = make_shared<FluidItem>();
			break;
		case ITEM_TYPE_ROLLER:
			item = make_shared<RollerItem>();
			break;
		case ITEM_TYPE_NEEDLE:
			item = make_shared<NeedleItem>();
			break;
		}

		_items.push_back(item);
		obj = item;
	}
	}

	if (obj)
	{
		RegisterObject(obj);

		obj->SetPos(Utils::TileToWorld(tilePos));
		_mapObjects[tilePos.y][tilePos.x] = obj;
	}

	return obj;
}



MapObjectRef GameRoom::GetMapObjectAt(Vec2Int tilePos)
{
	int32 mapSizeX = _mapObjects[0].size();
	int32 mapSizeY = _mapObjects.size();

	if (tilePos.x < 0 || tilePos.x >= mapSizeX || tilePos.y < 0 || tilePos.y >= mapSizeY)
		return nullptr;

	return _mapObjects[tilePos.y][tilePos.x].lock();
}

void GameRoom::CleanupExpired()
{
	_players.erase(
		remove_if(_players.begin(), _players.end(),
			[](auto& w) { return w.expired(); }),
		_players.end());

	_bombs.erase(
		remove_if(_bombs.begin(), _bombs.end(),
			[](auto& w) { return w.expired(); }),
		_bombs.end());
}

