#include "pch.h"
#include "GameRoom.h"
#include "Player.h"
#include "GameSession.h"

GameRoomRef GRoom = make_shared<GameRoom>();

GameRoom::GameRoom()
{
	
}

GameRoom::~GameRoom()
{

}

void GameRoom::Init()
{
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

	// TEMP
	player->info.set_posx(5);
	player->info.set_posy(5);

	// 입장한 클라에게 정보를 보내주기
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_MyPlayer(player->info);
		session->Send(sendBuffer);
	}
	// 모든 오브젝트 정보 전송
	{
		Protocol::S_AddObject pkt;

		for (auto& item : _players)
		{
			Protocol::ObjectInfo* info = pkt.add_objects();
			*info = item.second->info;
		}

		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddObject(pkt);
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

	uint64 id = session->player.lock()->info.objectid();
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

//void GameRoom::Handle_C_Move(Protocol::C_Move& pkt)
//{
//	uint64 id = pkt.info().objectid();
//	GameObjectRef gameObject = FindObject(id);
//	if (gameObject == nullptr)
//		return;
//
//	// TODO : Validation
//	gameObject->info.set_state(pkt.info().state());
//	gameObject->info.set_dir(pkt.info().dir());
//	gameObject->info.set_posx(pkt.info().posx());
//	gameObject->info.set_posy(pkt.info().posy());
//
//	{
//		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Move(pkt.info());
//		Broadcast(sendBuffer);
//	}
//}

void GameRoom::AddObject(ObjectRef object)
{
	uint64 id = object->info.objectid();

	auto objectType = object->info.objecttype();

	switch (objectType)
	{
		case Protocol::OBJECT_TYPE_PLAYER:
			_players[id] = static_pointer_cast<Player>(object);
			break;
		default:
			return;
	}

	object->room = GetRoomRef();

	// 신규 오브젝트 정보 전송
	{
		Protocol::S_AddObject pkt;

		Protocol::ObjectInfo* info = pkt.add_objects();
		*info = object->info;
	
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_AddObject(pkt);
		Broadcast(sendBuffer);
	}
}

void GameRoom::RemoveObject(uint64 id)
{
	ObjectRef gameObject = FindObject(id);
	if (gameObject == nullptr)
		return;

	switch (gameObject->info.objecttype())
	{
	case Protocol::OBJECT_TYPE_PLAYER:
		_players.erase(id);
		break;
	default:
		return;
	}

	gameObject->room = nullptr;

	// 오브젝트 삭제 전송
	{
		Protocol::S_RemoveObject pkt;
		pkt.add_ids(id);

		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_RemoveObject(pkt);
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