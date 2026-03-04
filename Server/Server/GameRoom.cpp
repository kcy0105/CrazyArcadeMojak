#include "pch.h"
#include "GameRoom.h"
#include "Player.h"
#include "GameSession.h"

GameRoomRef GRoom = make_shared<GameRoom>();

void GameRoom::Init()
{
	_tilemap.LoadFile(L"C:\\Users\\user\\OneDrive - postech.ac.kr\\바탕 화면\\CrazyArcadeMojak\\Server\\Client\\Resources\\Tilemap.txt");
	_tilemap.SetTileSize(40);
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
	player->SetColSize(30);
	player->SetState(PLAYER_STATE_IDLE);
	player->SetMoveSpeed(200);

	// 입장한 클라에게 정보를 보내주기
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_MyPlayer(player->GetObjectId(), player->GetPos().x, player->GetPos().y, player->GetDir(), player->GetColSize(), player->GetState(), player->GetMoveSpeed());
		session->Send(sendBuffer);
	}
	// 모든 오브젝트 정보 전송
	{
		Protocol::S_AddObject pkt;

		vector<uint64> objectids;
		vector<int32> objecttypes;
		vector<int32> posxs;
		vector<int32> posys;
		vector<int32> states;
		vector<int32> dirs;
		vector<int32> movespeeds;

		for (auto& item : _players)
		{
			objectids.push_back(item.second->GetObjectId());
			objecttypes.push_back(item.second->GetObjectType());
			posxs.push_back(item.second->GetPos().x);
			posys.push_back(item.second->GetPos().y);
			states.push_back(item.second->GetState());
			dirs.push_back(item.second->GetDir());
			movespeeds.push_back(item.second->GetMoveSpeed());
		}

		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddObject(objectids, objecttypes, posxs, posys, states, dirs, movespeeds);
		session->Send(sendBuffer);
	}
	// 타일맵 정보 전송
	{
		int32 mapsizex = _tilemap.GetMapSize().x;
		int32 mapsizey = _tilemap.GetMapSize().y;
		int32 tilesize = _tilemap.GetTileSize();
		vector<vector<int>> values = vector<vector<int>>(mapsizey, vector<int>(mapsizex));
		for (int y = 0; y < mapsizey; y++)
		{
			for (int x = 0; x < mapsizex; x++)
			{
				values[y][x] = _tilemap.GetTiles()[y][x].value;
			}
		}
		
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Tilemap(mapsizex, mapsizey, tilesize, values);
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

	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Move(id, player->GetState(), player->GetDir(), player->GetPos().x, player->GetPos().y);
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

			SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddObject({ id }, { objectType }, { (int32)player->GetPos().x }, {(int32)player->GetPos().y}, {player->GetState()}, {player->GetDir()}, {(int32)player->GetMoveSpeed()});
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

bool GameRoom::CanGo(uint8 colSize, Vec2Int pos)
{
	int32 tileSize = _tilemap.GetTileSize();

	RECT* r1 = new RECT({ pos.x - colSize / 2, pos.y - colSize / 2, pos.x + colSize / 2, pos.y + colSize / 2 });
	RECT* r2 = new RECT({ (pos.x / tileSize) * tileSize, (pos.y / tileSize) * tileSize, (pos.x / tileSize) * tileSize + tileSize, (pos.y / tileSize) * tileSize + tileSize });

	RECT r;
	return ::IntersectRect(&r, r1, r2) == false;
}
