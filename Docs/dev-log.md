# 2026-03-11
## 1. 클라, 서버 블럭 충돌 처리 방식 통일
### 이유
플레이어가 블럭에 가로막히는 것을 구현하기 위해, 기존에 클라 측은 엔진단에 구현해놓은 OnColliderStayOverlap()을 이용하여 플레이어가 블럭의 collider에 들어간만큼 다시 빼주도록 하였고, 서버 측은 이동할 좌표를 검사하여 갈 수 있는 곳까지만 가게끔 처리하였음.

대부분의 경우 오차 없이 잘 작동하였으나, 귀퉁이에 걸쳐서 충돌할 경우 클라는 충돌을 했다고 하고, 서버는 안했다고 하여 클라에서 보기엔 막히다가 키보드를 뗄 경우 순간이동하는 현상이 발생하였음.

클라와 서버의 충돌 처리 방식이 다른 것이 원인이라고 생각되어, 클라의 충돌 처리 방식을 서버의 충돌 처리 방식으로 변경함.
### 상세
Player(Client) OnUpdateMove 中 (서버와 동일)
```
float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

Pos pos = GetPos();

switch (_dir)
{
case DIR_LEFT:
	pos.x -= _moveSpeed * deltaTime;
	break;
case DIR_RIGHT:
	pos.x += _moveSpeed * deltaTime;
	break;
case DIR_UP:
	pos.y -= _moveSpeed * deltaTime;
	break;
case DIR_DOWN:
	pos.y += _moveSpeed * deltaTime;
	break;
}

TryMove(pos);
```

### 문제
어느 정도 개선됐지만 귀퉁이에 충돌할 때 클라와 서버가 종종 플레이어를 다른 방향으로 밀어내는 문제가 발생함.

### 해결
기존에 IntersectRect로 두 collider의 겹침 영역을 계산하고, 겹침 영역 중 짧은 변을 기준으로 플레이어를 밀어냈는데, 귀퉁이에 걸치게 되면 이러한 겹침 영역이 정사각형이 되면서, 짧은 변이라는 기준을 클라와 서버가 종종 다르게 판별하던 것이 원인이었음.

이동한 방향의 축에 따라 밀어내는 방식으로 수정하여 해결함.

TryMove(nextPos) 中
```
if (::IntersectRect(&intersect, &playerRect, &tileRect))
{
	if (GetPos().x == nextPos.x) // y 축 이동
	{
		if (nextPos.y > GetPos().y)
			pos.y = tileRect.top - half;
		else
			pos.y = tileRect.bottom + half;
	}
	else // x 축 이동
	{
		if (nextPos.x > GetPos().x)
			pos.x = tileRect.left - half;
		else
			pos.x = tileRect.right + half;
	}
	found = true;
	break;
}
```
## 2. 클라, 서버 물폭탄 벗어남 판단 방식 통일
### 이유
플레이어가 물폭탄을 설치하고 벗어났을 때 다시 물폭탄에 가로막히게 하기 위해, 플레이어가 물폭탄에서 벗어났을 때를 판단해야 함. 이를 위해 기존에 클라 측은 엔진단에서 구현해놓은 OnColliderEndOverlap()을 사용하였고, 서버 측은 플레이어가 움직일 때 물풍선에서 벗어났는지 검사함.

이 역시 통일이 필요하다 생각하여, 서버 측의 방식으로 클라 측의 방식을 수정함.
### 상세
모든 물폭탄을 검사하는 건 낭비라 생각되어, Player에 마지막으로 설치한 물폭탄을 들고 있도록 하고 해당 물폭탄만 검사하도록 함.

MyPlayer(Client) OnUpdateMove 中 (서버와 동일)
```
if (_lastBomb->GetPassable())
{
	RECT r1 = GetRect();
	RECT r2 = _lastBomb->GetRect();
	RECT r = {};

	if (::IntersectRect(&r, &r1, &r2) == false)
	{
		_lastBomb->SetPassable(false);
	}
}
```

## 3. 클라에서 이미 설치된 물폭탄에 서버 측 id 부여
### 이유
키 입력에 클라가 일단 반응하는 것을 원칙으로 삼았기 때문에, 클라 측에선 일단 id 없이 물폭탄 오브젝트를 생성하고 서버에 패킷을 전송함. 서버에서 id가 부여된 물폭탄을 생성하고, 다시 브로드캐스트하면 클라는 설치된 물폭탄을 찾아 id를 부여하도록 함.
### 상세
MyPlayer에서 pendingBombs라는 WaterBomb queue를 들고 있게 하고, 물폭탄을 일단 생성할 때 pendingBombs에 push했다가, 서버에서 패킷이 왔을 때 pop하여 id를 부여함.

ClientPacketHandler::Handle_S_WaterBomb() 中
```
if (ownerid == myPlayer->GetObjectId())
{
	auto bomb = myPlayer->pendingBombs.front();
	myPlayer->pendingBombs.pop();
	bomb->SetObjectId(objectid);
	return;
}
```

## 느낀 점
지금은 게임이 간단하여 일일이 클라(MyPlayer)와 서버의 동작을 일일이 맞추고 있지만, 나중에 게임 물리가 복잡해지면 클라와 서버 단의 물리적인 처리를 하는 엔진단이 공용으로 있어야 할 것 같다.
<br><br>
# 2026-03-12 
## 1. 클라 : Player에서 예측 이동 제거
### 이유
기존에 키 입력을 누르거나 뗄 때 이동 패킷을 전송하였고, 서버에서 이를 브로드캐스트하였음. Player는 Move state이면 클라 측의 캐릭터를 이동하고, 서버에서 패킷이 왔을 때 오차가 심하면 보정하였음. 문득 이와 같은 처리는 내 자신의 플레이어에 당장의 입력이 바로 반영되도록 하기 위한 것이었음을 상기하고, Player에는 이러한 처리가 필요 없음을 깨달음.
### 상세
충돌 처리를 포함한 예측 이동을 MyPlayer로 옮김. 대신에 클라 측에서 이동 패킷을 주기적으로 보내고, 서버는 이를 다시 브로드캐스트하여 다른 Player에서는 받은 위치로 보간하는 방식을 사용함.

MyPlayer (Client)
```
void MyPlayer::OnUpdateMove()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	Pos pos = GetPos();

	switch (_dir)
	{
	case DIR_LEFT:
		pos.x -= _moveSpeed * deltaTime;
		break;
	case DIR_RIGHT:
		pos.x += _moveSpeed * deltaTime;
		break;
	case DIR_UP:
		pos.y -= _moveSpeed * deltaTime;
		break;
	case DIR_DOWN:
		pos.y += _moveSpeed * deltaTime;
		break;
	}

	TryMove(pos);

	static float moveSyncTimer = 0.f;
	moveSyncTimer += deltaTime;

	const float MOVE_SYNC_INTERVAL = 0.05f; // 20Hz

	if (moveSyncTimer >= MOVE_SYNC_INTERVAL)
	{
		_moveDirtyFlag = true;
		moveSyncTimer = 0.f;
	}

	HandleMoveInput_Move();
	HandleBombInput();
}
```
Player (Client)
```
void Player::OnUpdateMove()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	Vec2 pos = GetPos();
	Vec2 diff = _serverPos - pos;
	float dist = diff.Length();

	if (diff.Length() < 1.f || diff.Length() > 200.f) // 도착하면 상태 변경. 너무 멀어도 바로 싱크.
	{
		SetPos(_serverPos);
		SetState(_serverState);
		return;
	}

	pos += diff * 20.f * deltaTime;

	SetPos(pos);
}
```
### 문제
Player에서 이동 패킷이 오면 state도 바로 따라서 변경하였더니, 아직 패킷의 위치로 도달하지 않았는데 state가 변경되어 멈추는 상황이 발생함.
### 해결
위 코드처럼 패킷의 위치로 도달했을 때 state를 바꾸도록 함. 이때 방향 역시 도달했을 때 바뀌는 것으로 해보았지만, 방향은 즉시 변경하는 것이 자연스러웠음.

## 2. 플레이어가 설치한 물폭탄에서 벗어나는 판단 처리 개선
### 이유
플레이어가 물폭탄을 설치했다가, 물폭탄에서 벗어났을 때 다시 물폭탄을 통과하지 못하게 하기 위해 물폭탄에서 벗어났을 때를 판단할 필요가 있었음. 다만 이를 위해 모든 물폭탄을 순회하는 건 낭비라 생각하여, 마지막으로 설치한 물폭탄을 Player가 들고 있도록 하고, 해당 물폭탄에 대해서만 검사를 하도록 했는데, 플레이어가 물폭탄을 설치할 시점에 다른 물폭탄과도 겹쳐있는 상태가 있을 수 있으므로, Player가 현재 겹쳐있는 물폭탄들을 들고 있도록 하고 이를 검사함.
### 상세
MyPlayer(Client)의 OnUpdateMove() 中
```
for (auto it = _overlapBombs.begin(); it != _overlapBombs.end();)
{
    WaterBomb* bomb = *it;

    RECT r1 = GetRect();
    RECT r2 = bomb->GetRect();
    RECT r = {};

    if (::IntersectRect(&r, &r1, &r2) == false)
    {
        bomb->SetPassable(false);
        it = _overlapBombs.erase(it);
    }
    else
    {
        ++it;
    }
}
```
서버 측에서도 마찬가지로 구현하였음. 클라 측에서는 MyPlayer만 고려한 반면 서버 측에서는 모든 플레이어를 고려해야 하므로 WaterBomb에 passablePlayers를 들고 있도록 하였고, BlocksPlayer(player)에서 passablePlayers를 검사하여 플레이어가 통과 가능한지 반환하도록 함.

Player(Server)의 Update() 中
```
for (auto it = _overlapBombs.begin(); it != _overlapBombs.end();)
{
    WaterBombRef bomb = it->lock();

    if (!bomb)
    {
        it = _overlapBombs.erase(it);
        continue;
    }

    RECT r1 = GetRect();
    RECT r2 = bomb->GetRect();
    RECT r = {};

    if (::IntersectRect(&r, &r1, &r2) == false)
    {
        bomb->RemovePassablePlayer(shared_from_this());
        it = _overlapBombs.erase(it);
    }
    else
    {
        ++it;
    }
}
```
WaterBomb(Server) 中
```
virtual bool BlocksPlayer(const Player* player) const override
{
    for (auto& w : _passablePlayers)
    {
        auto p = w.lock();
        if (p.get() == player)
            return false;
    }

    return true;
}
```
## 느낀 점
입력 시 일단 동작하도록 하는 방식을 사용해서 그런지, 클라의 MyPlayer와 서버의 Player 동작이 거의 흡사하다. 클라의 나머지 오브젝트는 껍데기에 불과하다.
<br><br>
# 2026-03-14

## 1. proto 파일 컴파일 후 생성된 파일을 서버와 클라로 복사하는 작업을 별도 프로젝트로 분리
### 이유
해당 작업이 기존에 Server 프로젝트에 포함되어 있어 빌드 시 불필요한 시간을 소요함
### 상세
프로젝트를 클라이언트(Client), 서버(Server), 네트워크 라이브러리(Network), 프로토콜(Protocol)로 분리하였음.

![alt text](images/image-1.png)
## 2. 패킷 핸들러 함수 자동 생성 구현
### 이유
패킷이 추가될 때마다 반복적으로 하던 작업의 최소화 위함
### 상세
다음의 파일을 자동생성 및 복사하도록 함.
- PacketEnum.h (공통)
- ClientPacketHandler.h
- ClientPacketHandler.gen.cpp
- ServerPacketHandler.h
- ServerPacketHandler.gen.cpp

~PacketHandler.gen.cpp 에서 static 객체 생성을 이용하여 프로그램 실행 시 핸들러 함수를 핸들러 함수 전역 배열에 등록하도록 함. 앞으로 패킷 추가 시 ~PacketHandler.cpp 에서 대응하는 핸들러 함수만 작성하면 됨.


ClientPacketHandler.gen.cpp
```
REGISTER_PACKET(S_EnterGame, Protocol::S_EnterGame, ClientPacketHandler::Handle_S_EnterGame);
REGISTER_PACKET(S_MyPlayer, Protocol::S_MyPlayer, ClientPacketHandler::Handle_S_MyPlayer);
```

매크로(Network)
```
#define REGISTER_PACKET(ID, TYPE, FUNC) \
static PacketHandlerRegistry _reg_##ID( \
    MakeHandler<TYPE>(FUNC), ID)

template<typename PacketType, typename ProcessFunc>
PacketHandlerFunc MakeHandler(ProcessFunc func)
{
    return [func](SessionRef session, BYTE* buffer, int32 len)
        {
            PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

            PacketType pkt;
            pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));

            func(session, pkt);
        };
}
```

PacketHandlerRegistry.h(Network)
```
PacketHandlerFunc GPacketHandler[UINT16_MAX];

class PacketHandlerRegistry
{
public:
    PacketHandlerRegistry(PacketHandlerFunc func, uint16 id)
    {
        GPacketHandler[id] = func;
    }
};
```


bat 파일로 [proto 컴파일 -> 코드 생성 -> proto 컴파일 파일 및 자동 생성 코드 복사] 를 한번에 처리할 수 있도록 함.

## 3. 패킷 전송 인터페이스 개선
### 이유
기존에 Make_{PacketName}(uint64 objectid, ...)과 같이 함수 인자로 패킷을 조립하여 SendBuffer를 반환하는 함수를 패킷 별로 만들고, 반환한 버퍼를 세션에서 Send(buffer)를 호출하여 전송하는 방식을 사용했는데, 번거로운 과정이라 느껴져 해당 함수를 사용하지 않고 패킷을 조립하여 바로 SendPacket(pkt)을 할 수 있도록 수정함
### 상세
기존
```
SendBufferRef sendBuffer = ServerPacketHandler::Make_S_MyPlayer(player->GetObjectId(), player->GetPos().x, player->GetPos().y, player->GetDir(), player->GetState(), player->GetMoveSpeed());
session->Send(sendBuffer);
```
바뀜
```
Protocol::S_MyPlayer pkt;
pkt.set_objectid(player->GetObjectId());
pkt.set_posx(player->GetPos().x);
pkt.set_posy(player->GetPos().y);
pkt.set_dir(player->GetDir());
pkt.set_state(player->GetState());
pkt.set_movespeed(player->GetMoveSpeed());
session->SendPacket(pkt);
```
### 문제
SendPacket에서 패킷을 인자로 받으면 패킷 아이디를 추출할 필요가 있었는데, 이를 위해 (패킷 이름, 패킷 아이디)의 unordered_map을 자동 생성으로 세팅하였음. 다만 패킷 아이디를 추출하는 과정에서 패킷에서 string 및 map lookup으로 인한 부하가 예상됨.

### 해결
다음과 같이 템플릿을 활용하여 컴파일 타임에 패킷에 따른 id가 결정되도록 함.

```
enum PacketId
{
    C_Move = 14,
    S_Move = 15,
};

template<typename T>
struct PacketIdType
{
};

template<>
struct PacketIdType<Protocol::C_Move>
{
    static const uint16 value = C_Move;
};

template<>
struct PacketIdType<Protocol::S_Move>
{
    static const uint16 value = S_Move;
};
```
<br><br>
# 2026-03-15
## 1. 클라이언트 측 ObjectManager 구현
### 이유
기존에 서버 측에서 object id가 넘어오면, Scene에서 모든 오브젝트를 순회하며 id에  해당하는 오브젝트를 찾았는데, 오브젝트와 패킷이 많아질 수록 비용이 커질 거라 예상되어 탐색 복잡도를 줄이기 위해 (id, object)의 unordered_map을 가지는 ObjectManager를 추가하였음.
### 상세
서버와 연동되는 오브젝트들의 상위 클래스를 SyncObject라 두었고, ObjectManager에서 (id, SyncObject)의 unordered_map을 가지고 있음. 싱글톤으로 사용함.
```
class ObjectManager
{
	DECLARE_SINGLE(ObjectManager)

public:
	void RegisterSyncObject(SyncObject* obj);
	void UnregisterSyncObject(SyncObject* obj);
	SyncObject* GetSyncObject(uint64 id) { return _syncObjects[id]; }

private:
	unordered_map<uint64, SyncObject*> _syncObjects;
}
```
SyncObject에 id를 설정할 때, 해제될 때 자동으로 등록 및 등록 해제되도록 함.

```
void SyncObject::OnRelease()
{
	GET_SINGLE(ObjectManager)->UnregisterSyncObject(this);
}

void SyncObject::SetObjectId(uint64 objectId)
{
	_objectId = objectId;
	GET_SINGLE(ObjectManager)->RegisterSyncObject(this);
}
```
## 2. 서버 측 Object 관리 구조 개선
### 이유
오브젝트가 많아지며 한번에 관리를 해야겠다고 느낌. 클라이언트에서의 Object 관리 구조 모방.
### 상세
다음의 함수로 오브젝트가 컨테이너에 추가, 삭제되도록 함.
```
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
```
이때 모든 오브젝트는 _objects에 포함되며, _players, _bombs와 같이 따로 그룹화를 위해 정의한 컨테이너는 Spawn 함수를 따로 파서 추가되도록 하였음.

클라이언트에서와 마찬가지로 _objects를 순회하며 Update를 실행하고, dead 플래그가 세워진 오브젝트를 지움. RemoveDeadObjects() 안에서 UnregisterObject(obj)가 수행됨.
```
void GameRoom::Update()
{
	for (auto& item : _objects)
	{
		item.second->Update();
	}

	RemoveDeadObjects();
}
```
## 느낀 점
추가적인 컨테이너로 객체를 관리할 시, 객체를 삭제할 때 해당 컨테이너들도 비워주는 것을 잊지 말자.
<br><br>
# 2026.03.16
## 1. 물폭탄 터짐 패킷 설계
### 상세
서버 측에서 물폭탄 터짐을 처리하고, 다음의 패킷을 클라로 보내주도록 설계함.
```
message S_Explode
{
	// 물폭탄 id
	uint64 objectid = 1;
	
	// 폭발 범위
	int32 up = 2;
	int32 down = 3;
	int32 left = 4;
	int32 right = 5;

	// (부서진 블럭 id, 스폰된 아이템 id)
	repeated DestroyedBlockInfo destroyedblockinfos = 6;
	// 맞은 플레이어 id
	repeated uint64 trappedplayerids = 7;
	// 부서진 아이템 id
	repeated uint64 destroyeditemids = 8;
}
```
## 2. 서버 측 물폭탄 로직 구현
### 상세
1. SOLID_BLOCK에 닿았을 땐 폭발이 그 전에 끊김
2. BREAKABLE_BLOCK에 닿았을 땐 블럭을 부수고 폭발이 거기까지 끊김
3. WATER_BOMB에 닿았을 땐 해당 물폭탄도 폭발시킴
4. 범위 내에 플레이어가 겹치면 trapped 처리

GameRoom(Server)
```
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
```
## 3. 클라 측 물폭탄 터짐 처리 : 이펙트, 블럭 부수기
### 상세
ClientPacketHandler(Client)
```
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
			// TODO : TRAP
		}
	}

	// TODO : 아이템 부수기
}
```
bomb->Explode(~) 내에서 범위에 따른 이펙트 오브젝트가 생성되도록 함.
<br><br>
# 2026.03.17
## 1. 서버 측 오브젝트 보조 관리 개선
### 이유
기존에 서버에서 다음과 같이 오브젝트를 관리했음.
```
unordered_map<uint64, shared_ptr<Object>> _objects;
unordered_map<uint64, shared_ptr<Player>> _players;
unrodered_map<uint64, shared_ptr<WaterBomb>> _bombs;
vector<vector<shared_ptr<MapObject>>> _mapObjects;
```
모든 오브젝트는 _objects에 들어가 GameRoom에서 순회하며 Update를 하고 삭제를 관리하였고, 타입에 따라 _players, _bombs, _mapObjects에 각각 등록, 해제되도록 함.

다만 다음의 이유로 개선이 필요하다고 생각됨.  
1. 오브젝트를 지울 때 보조 컨테이너(_players, _bombs, _mapObjects)에서도 지워줘야 하는 번거로움
2. 1번을 하지 않았을 때 객체 삭제가 되지 않음
3. id lookup은 _objects에서 제공하는데, _players, _bombs가 map일 필요가 있을까? vector로 한다면 성능 개선 기대됨.
### 상세
다음과 같이 보조 컨테이너에서 shared_ptr 대신 weak_ptr을 사용하였고, map 대신 vector를 사용함.
```
unordered_map<uint64, shared_ptr<Object>> _objects;
vector<weak_ptr<Player>> _players;
vector<weak_ptr<WaterBomb>> _bombs;
vector<vector<weak_ptr<MapObject>>> _mapObjects;
```
이제 오브젝트를 지울 땐 _objects에서만 지워도 객체가 삭제됨. 보조 컨테이너에 expired된 weak_ptr이 남아있지만, lock()으로 유효함을 체크하면 될 일이고, 그럼에도 컨테이너에 expired된 weak_ptr이 계속 쌓이는 것을 방지하기 위해 매 틱마다 이를 정리하도록 하였음.
```
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
```
## 2. 연속 애니메이션 재생 기능 추가
### 이유
물풍선에 갇힐 때, 물풍선이 생기는 애니메이션이 재생된 후 물풍선이 떠다니는 애니메이션이 반복 재생되도록 하기 위해 FlipbookRenderer 컴포넌트에 기능을 추가함.
### 상세
기존에 SetFlipbook에 Flipbook 하나만 넣어줬는데, vector<Flipbook>을 넣는 함수를 오버로드함. vector에 담긴 순서대로 Flipbook이 재생됨.
## 3. 물폭탄 터짐 로직 개선
### 이유
기존에 폭발 범위에 물폭탄이 있던 자리는 포함을 안 시켜서, 어떻게 넣을 지 고민하다 전체적으로 리팩토링함.
### 상세
폭발 범위를 계산하는 부분과, 폭발 범위 내의 오브젝트를 처리하는 부분을 분리함.
```
void GameRoom::Explode(WaterBomb& bomb)
{
	bomb.SetExploded(true);

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

			RECT r1 = { explodeWorldPos.x - TILE_SIZE / 2, explodeWorldPos.y - TILE_SIZE / 2, explodeWorldPos.x + TILE_SIZE / 2, explodeWorldPos.y + TILE_SIZE / 2 };
			RECT r2 = player->GetRect();
			RECT r = {};

			if (::IntersectRect(&r, &r1, &r2))
			{
				pkt.add_trappedplayerids(player->GetObjectId());

				player->SetState(PLAYER_STATE_TRAPPED_IDLE);
			}
		}


		auto mapObject = GetMapObjectAt(explodePos);
		if (!mapObject) continue;

		/*================
		   Destroy Items
		=================*/
		// TODO


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
```
## 4. 플레이어 갇힘 구현
### 상세
애니메이션을 세팅하고, TRAPPED_IDLE, TRAPPED_MOVE 상태를 추가하여 클라, 서버 측의 갇힘 상태를 구현함.
### 문제
상태 처리가 복잡해져 코드가 꼬임. 특히 애니메이션이나 이동.
### 해결
메인 상태를 NORMAL, TRAPPED, DEAD로 두고, 이동 상태를 IDLE, MOVE로 두어 상태를 분리함. 상당 부분 고쳐야 하여 진행 중.
## 
## 느낀 점
코딩을 할 때 MyPlayer -> 서버 -> 클라 순서로 생각을 하면 흐름이 잘 잡힌다.