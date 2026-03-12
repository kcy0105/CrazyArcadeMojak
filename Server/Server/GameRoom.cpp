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
	LoadTilemap(L"C:\\Users\\user\\바탕 화면\\CrazyArcadeMojak\\Server\\Client\\Resources\\Tilemap.txt");
}

void GameRoom::Update()
{
	for (auto& item : _players)
	{
		item.second->Update();
	}
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

	// 입장한 클라에게 정보를 보내주기
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_MyPlayer(player->GetObjectId(), player->GetPos().x, player->GetPos().y, player->GetDir(), player->GetState(), player->GetMoveSpeed());
		session->Send(sendBuffer);
	}
	// 모든 오브젝트 정보 전송
	{
		Protocol::S_AddObject pkt;

		vector<uint64> objectids;
		vector<int32> objecttypes;
		vector<float> posxs;
		vector<float> posys;
		vector<int32> states;
		vector<int32> dirs;
		vector<float> movespeeds;

		for (auto& item : _players)
		{
			objectids.push_back(item.second->GetObjectId());
			objecttypes.push_back(item.second->GetObjectType());
			posxs.push_back((item.second->GetPos().x));
			posys.push_back((item.second->GetPos().y));
			states.push_back(item.second->GetState());
			dirs.push_back(item.second->GetDir());
			movespeeds.push_back((item.second->GetMoveSpeed()));
		}

		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddObject(objectids, objecttypes, posxs, posys, states, dirs, movespeeds);
		session->Send(sendBuffer);
	}
	// 타일맵 정보 전송
	{
		int32 mapsizex = _mapObjects[0].size();
		int32 mapsizey = _mapObjects.size();
		vector<vector<int>> values = vector<vector<int>>(mapsizey, vector<int>(mapsizex));
		for (int y = 0; y < mapsizey; y++)
		{
			for (int x = 0; x < mapsizex; x++)
			{
				if (_mapObjects[y][x])
					values[y][x] = _mapObjects[y][x]->GetMapObjectType();
			}
		}
		
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Tilemap(mapsizex, mapsizey, values);
		session->Send(sendBuffer);
	}

	AddObject(player);
}

void GameRoom::LeaveRoom(GameSessionRef session)
{
	if (session == nullptr)
		return;
	if (session->player.lock() == nullptr)
		return;

	uint64 id = session->player.lock()->GetObjectId();
	RemoveObject(id);
}

ObjectRef GameRoom::FindObject(uint64 id)
{
	{
		auto findIt = _players.find(id);
		if (findIt != _players.end())
			return findIt->second;
	}

	return nullptr;
}

void GameRoom::Handle_C_Move(Protocol::C_Move& pkt)
{
	uint64 id = pkt.objectid();
	ObjectRef object = FindObject(id);
	if (object == nullptr)
		return;
 
	PlayerRef player = static_pointer_cast<Player>(object);
	player->SetState((PLAYER_STATE)pkt.state());
	player->SetDir((DIR)pkt.dir());


	// TODO: 데이터 검증. 지금은 너무 빈약하다.
	Pos serverPos = player->GetPos();
	Pos clientPos = { pkt.posx(), pkt.posy() };

	bool needsync = false;

	/*cout << "Server Pos : { " << serverPos.x << ", " << serverPos.y << " }, Client Pos : { " << clientPos.x << ", " << clientPos.y << " }" << endl;
	cout << "Distance : " << (serverPos - clientPos).Length() << endl;*/

	if ((serverPos - clientPos).Length() < MAX_POSITION_ERROR)
	{
		player->SetPos({ pkt.posx(), pkt.posy() });
	}
	else
	{
		//cout << "오차 심함" << endl;
		needsync = true;
	}

	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Move(player->GetObjectId(), player->GetState(), player->GetDir(), player->GetPos().x, player->GetPos().y, needsync);
		Broadcast(sendBuffer);
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

	PlayerRef owner = dynamic_pointer_cast<Player>(FindObject(ownerId));

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
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_WaterBomb(bomb->GetObjectId(), ownerId, tilePosX, tilePosY);
		Broadcast(sendBuffer);
	}
}

void GameRoom::AddObject(ObjectRef object)
{
	uint64 id = object->GetObjectId();

	OBJECT_TYPE objectType = object->GetObjectType();

	switch (objectType)
	{
		case OBJECT_TYPE_PLAYER:
		{
			PlayerRef player = static_pointer_cast<Player>(object);
			_players[id] = player;

			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddObject({ id }, { objectType }, { player->GetPos().x }, {player->GetPos().y}, {player->GetState()}, {player->GetDir()}, {player->GetMoveSpeed()});
			Broadcast(sendBuffer);
		}
			break;
		default:
			return;
	}

	object->room = GetRoomRef();
}

void GameRoom::RemoveObject(uint64 id)
{
	ObjectRef object = FindObject(id);
	if (object == nullptr)
		return;

	switch (object->GetObjectType())
	{
	case OBJECT_TYPE_PLAYER:
		_players.erase(id);
		break;
	default:
		return;
	}

	object->room = nullptr;

	// 오브젝트 삭제 전송
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_RemoveObject({id});
		Broadcast(sendBuffer);
	}
}

void GameRoom::Broadcast(SendBufferRef& sendBuffer)
{
	for (auto& item : _players)
	{
		item.second->session->Send(sendBuffer);
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
	player->SetObjectId(Object::s_idGenerator++);
	player->SetObjectType(OBJECT_TYPE_PLAYER);

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
		obj->SetMapObjectType(type);
		obj->SetPos(Utils::TileToWorld(tilePos));
		_mapObjects[tilePos.y][tilePos.x] = obj;

		obj->SetObjectId(Object::s_idGenerator++);
	}


	return obj;
}

