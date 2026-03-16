#include "pch.h"
#include "GameRoom.h"
#include "Player.h"
#include "GameSession.h"
#include "BreakableBlock.h"
#include "SolidBlock.h"
#include "WaterBomb.h"

GameRoomRef GRoom = make_shared<GameRoom>();

void GameRoom::Init()
{
	LoadTilemap(L"C:\\Users\\user\\바탕 화면\\CrazyArcadeMojak\\CrazyArcadeMojak\\Resources\\Tilemap.txt");
}

void GameRoom::Update()
{
	for (auto& item : _objects)
	{
		item.second->Update();
	}

	RemoveDeadObjects();
}

void GameRoom::EnterRoom(GameSessionRef session)
{
	PlayerRef player = SpawnPlayer();

	// 서로의 존재를 연결
	session->gameRoom = GetRoomRef();
	session->player = player;
	player->session = session;

	// Initialize
	player->SetPos({ 100, 100 });
	player->SetDir(DIR_DOWN);
	player->SetState(PLAYER_STATE_IDLE);
	player->SetMoveSpeed(200);


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
		myInfo->set_state(player->GetState());
		myInfo->set_movespeed(player->GetMoveSpeed());

		session->SendPacket(myPlayerPkt);
	}


	/*=====================
	   Send Other Players
	======================*/
	{
		Protocol::S_OtherPlayers pkt;

		for (auto& item : _players)
		{
			Protocol::PlayerInfo* info = pkt.add_infos();
			info->set_objectid(item.second->GetObjectId());
			info->set_posx((item.second->GetPos().x));
			info->set_posy((item.second->GetPos().y));
			info->set_state(item.second->GetState());
			info->set_dir(item.second->GetDir());
			info->set_movespeed((item.second->GetMoveSpeed()));
		}

		session->SendPacket(pkt);
	}
	_players[player->GetObjectId()] = player;



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
					info->set_type(obj->GetMapObjectType());
				}
				else
				{
					info->set_objectid(0);
					info->set_type(MAP_OBJECT_TYPE_NONE);
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
 
	PlayerRef player = _players[id];
	player->SetState((PLAYER_STATE)pkt.state());
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
		needsync = true;
	}

	{
		Protocol::S_Move pkt;
		pkt.set_objectid(player->GetObjectId());
		pkt.set_state(player->GetState());
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

	// 일단 검증
	if (_mapObjects[tilePosY][tilePosX] != nullptr)
	{
		return;
	}

	PlayerRef owner = _players[ownerId];

	if (!owner)
	{
		return;
	}


	auto bomb = static_pointer_cast<WaterBomb>(SpawnMapObject(MAP_OBJECT_TYPE_WATER_BOMB, { tilePosX, tilePosY }));
	bomb->SetOwner(owner);


	for (auto& p : _players)
	{
		PlayerRef player = p.second;

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

	switch (obj->GetObjectType())
	{
	case OBJECT_TYPE_PLAYER:
		_players.erase(id);
		break;
	case OBJECT_TYPE_MAP_OBJECT:
	{
		MapObjectRef mapObj = static_pointer_cast<MapObject>(obj);

		switch (mapObj->GetMapObjectType())
		{
		case MAP_OBJECT_TYPE_WATER_BOMB:
			_bombs.erase(id);
			break;
		}

		Vec2Int tilePos = mapObj->GetTilePos();
		_mapObjects[tilePos.y][tilePos.x] = nullptr;
	}

		break;
	}

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
	Pos pos = player.GetPos();
	float half = PLAYER_SIZE / 2;

	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
			pos.x = nextPos.x;
		else
			pos.y = nextPos.y;

		RECT playerRect =
		{
			(LONG)(pos.x - half),
			(LONG)(pos.y - half),
			(LONG)(pos.x + half),
			(LONG)(pos.y + half)
		};

		int32 minTileX = playerRect.left / TILE_SIZE;
		int32 maxTileX = (playerRect.right - 1) / TILE_SIZE;
		int32 minTileY = playerRect.top / TILE_SIZE;
		int32 maxTileY = (playerRect.bottom - 1) / TILE_SIZE;

		for (int32 y = minTileY; y <= maxTileY; y++)
		{
			for (int32 x = minTileX; x <= maxTileX; x++)
			{
				MapObjectRef obj = _mapObjects[y][x];

				if (obj == nullptr)
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
					if (i == 0)
					{
						if (nextPos.x > player.GetPos().x)
							pos.x = tileRect.left - half;
						else
							pos.x = tileRect.right + half;
					}
					else
					{
						if (nextPos.y > player.GetPos().y)
							pos.y = tileRect.top - half;
						else
							pos.y = tileRect.bottom + half;
					}
				}
			}
		}
	}

	player.SetPos(pos);
}

void GameRoom::Explode(WaterBomb& bomb)
{
	bomb.SetExploded(true);

	Vec2Int bombPos = bomb.GetTilePos();

	Protocol::S_Explode pkt;
	pkt.set_objectid(bomb.GetObjectId());

	
	// up down left right
	vector<Vec2Int> dirs = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
	vector<uint8> counts(4, bomb.GetRange());

	for (int i=0; i<4; i++)
	{
		Vec2Int dir = dirs[i];
		for (int j = 1; j <= bomb.GetRange(); j++)
		{
			Vec2Int checkPos = bombPos + dir * j;

			bool end = false;

			/*======================
			    Check Map Objects
			=======================*/
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
					Protocol::DestroyedBlockInfo* info = pkt.add_destroyedblockinfos();
					info->set_blockid(mapObject->GetObjectId());
					info->set_itemid(0); // TODO : 아이템 스폰

					mapObject->Destroy();

					counts[i] = j;
					end = true;
				}
					break;
				case MAP_OBJECT_TYPE_WATER_BOMB:
				{
					auto otherBomb = static_pointer_cast<WaterBomb>(mapObject);
					if (otherBomb->GetExploded() == false)
					{
						otherBomb->Explode();
					}
				}
					break;
				}
			}

			if (end)
				break;

			
			/*====================
			     Check Players
			=====================*/

			for (auto& item : _players)
			{
				uint64 playerId = item.first;
				PlayerRef player = item.second;

				RECT r1 = { checkPos.x - TILE_SIZE / 2, checkPos.y - TILE_SIZE / 2, checkPos.x + TILE_SIZE / 2, checkPos.y + TILE_SIZE / 2 };
				RECT r2 = player->GetRect();
				RECT r = {};

				if (::IntersectRect(&r, &r1, &r2))
				{
					pkt.add_trappedplayerids(playerId);

					player->SetTrapped(true);
				}
			}

			// TODO : 아이템 제거
			
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

	_mapObjects = vector<vector<MapObjectRef>>(mapSizeY, vector<MapObjectRef>(mapSizeX));

	for (int32 y = 0; y < mapSizeY; y++)
	{
		for (int32 x = 0; x < mapSizeX; x++)
		{
			int32 value = -1;
			::fread(&value, sizeof(value), 1, file);

			SpawnMapObject((MAP_OBJECT_TYPE)value, { x, y });
		}
	}

	::fclose(file);
}

PlayerRef GameRoom::SpawnPlayer()
{
	PlayerRef player = make_shared<Player>();

	RegisterObject(player);

	return player;
}

MapObjectRef GameRoom::SpawnMapObject(MAP_OBJECT_TYPE type, Vec2Int tilePos)
{
	MapObjectRef obj = nullptr;

	switch (type)
	{
	case MAP_OBJECT_TYPE_BREAKABLE_BLOCK:
		obj = make_shared<BreakableBlock>();
		break;
	case MAP_OBJECT_TYPE_SOLID_BLOCK:
		obj = make_shared<SolidBlock>();
		break;
	case MAP_OBJECT_TYPE_WATER_BOMB:
		obj = make_shared<WaterBomb>();
		break;
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

	return _mapObjects[tilePos.y][tilePos.x];
}

