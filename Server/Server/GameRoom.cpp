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
	PlayerRef player = Object::CreatePlayer();

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
	int32 tilePosX = pkt.tileposx();
	int32 tilePosY = pkt.tileposy();

	// 일단 검증
	if (_mapObjects[tilePosY][tilePosX] != nullptr)
	{
		return;
	}

	_mapObjects[tilePosY][tilePosX] = make_shared<WaterBomb>();

	// TODO
	//{
	//	SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Move(player->GetObjectId(), player->GetState(), player->GetDir(), player->GetPos().x, player->GetPos().y, needsync);
	//	Broadcast(sendBuffer);
	//}
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
	float half = player.GetColSize() * 0.5f;

	RECT playerRect =
	{
		(LONG)(nextPos.x - half),
		(LONG)(nextPos.y - half),
		(LONG)(nextPos.x + half),
		(LONG)(nextPos.y + half)
	};

	int32 minTileX = playerRect.left	/ TILE_SIZE;
	int32 maxTileX = playerRect.right	/ TILE_SIZE;
	int32 minTileY = playerRect.top		/ TILE_SIZE;
	int32 maxTileY = playerRect.bottom	/ TILE_SIZE;

	for (int32 y = minTileY; y <= maxTileY; y++)
	{
		for (int32 x = minTileX; x <= maxTileX; x++)
		{
			if (_mapObjects[y][x] == nullptr)
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
				int32 w = intersect.right - intersect.left;
				int32 h = intersect.bottom - intersect.top;

				if (w < h)
				{
					if (playerRect.left < tileRect.left)
						nextPos.x -= w;
					else
						nextPos.x += w;
				}
				else
				{
					if (playerRect.top < tileRect.top)
						nextPos.y -= h;
					else
						nextPos.y += h;
				}

				playerRect =
				{
					(LONG)(nextPos.x - half),
					(LONG)(nextPos.y - half),
					(LONG)(nextPos.x + half),
					(LONG)(nextPos.y + half)
				};
			}
		}
	}

	player.SetPos(nextPos);
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

			_mapObjects[y][x] = MapObject::Factory((MAP_OBJECT_TYPE)value);
		}
	}

	::fclose(file);
}

