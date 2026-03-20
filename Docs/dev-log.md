# 2026.03.04
## 1. 서버에서 보낸 맵 정보로 클라가 오브젝트 배치
### 상세
서버에서 플레이어가 들어올 때 맵 정보(Tilemap)를 전달하여, 클라 측에서 이를 참고해 오브젝트를 배치하도록 함.
## 2. 서버 권위 방식의 이동 동기화 구현 시도
### 상세
키 입력 시 클라 측 플레이어도 일단 움직이도록 하고, 서버 측 플레이어에서도 동시에 움직이며 클라에 위치 정보를 계속 전송하여 클라 측에서 이를 참고해 보정하도록 했으나, 그 오차가 크게 체감됨. 특히 멈출 때나 방향을 이리저리 바꿀 때.
<br><br>

# 2026.03.05
## 1. 이동 동기화 시 클라 측의 위치 반영하도록 수정
### 이유
기존에 서버 권위를 위해, 키 입력 시 서버의 플레이어도 Update에 따라 움직이고, 그 위치를 클라이언트의 플레이어 위치에 자연스럽게 반영하려고 여러 시도를 해봤으나, 클라와 서버의 프레임이 다르기 때문인지 자연스러운 처리를 하는데 실패함(키 입력의 지연 반응 체감 큼). 결국 키 입력 시 클라의 플레이어 위치도 바로 바뀌도록 하고, 키를 누를 때와 뗄 때 패킷을 보내 서버에서 검증 후 클라의 위치를 반영하는 방식으로 수정하여 자연스러운 이동 동기화를 구현함.
### 상세
다음과 같이 서버에서 클라 측의 위치와 서버 측의 위치를 비교하여 오차 이내면 클라 측의 위치를 따르고, 그게 아니라면 서버 측의 위치를 따르게 하였음. 또한 오차를 벗어났을 때 클라 측에도 정보를 보내 클라 측 위치도 서버 측 위치로 수정하도록 함.

서버 측 이동 패킷 핸들러 함수 中
```
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
```

## 2. 클라 블럭 충돌 처리 구현 (진행중)
### 상세
엔진단에서 구현해놓은 OnColliderBeginOverlap()을 사용하여 블럭의 collider에 플레이어 collider가 들어온만큼 다시 밀어주도록 코드를 작성하였는데 잘 작동하지 않음. 게임 루프 상 순서 이슈로 보임.

## 느낀 점
게임 서버의 TPS가 클라이언트의 FPS만큼 높지 않는 이상, 이렇게 키 입력이 빈번한 게임에서는 클라에 권위를 어느 정도 두는 건 불가피하지 않을 까란 생각이 든다.
<br><br>

# 2026.03.06
## 1. 클라 블럭 충돌 처리 구현
### 상세
OnColliderBeginOverlap() 대신 OnColliderStayOverlap()을 추가 구현하여 해당 함수 안에서 플레이어를 밀어주도록 작성하였더니 잘 작동함.
## 2. 서버 블럭 충돌 처리 구현
### 상세
플레이어가 블럭을 고려하여 다음에 갈 곳의 위치를 판별하는 방식으로 작성함.
### 문제
다음에 갈 곳의 위치를 CanGo() 함수에 넘겨 충돌하지 않아 갈 수 있는 위치이면 해당 위치로 변경하도록 작성하였는데, 서버의 TICK이 크다 보니 블럭이랑 밀착하지 않아도 충분히 가까우면 CanGo()가 false를 반환하는 문제가 발생함.
### 해결
다음 위치를 CanGo()로 갈 수 있는 곳인지 판별하는 것이 아니라, TryMove()에 넘겨 플레이어와 블럭이 겹친다면 밀착시키도록 함.
## 3. 맵 오브젝트 클래스 설계
### 상세
맵에 배치되는 오브젝트의 최상위 클래스를 MapObject라 두고, 산하에 BreakableBlock, SolidBlock, WaterBomb 클래스를 둠.
## 4. 클라 측 물풍선 생성 구현 및 패킷 전송
### 상세
MapObject를 상속받는 WaterBomb 클래스를 만들어 Flipbook을 적용시키고, 키 입력 시 해당 위치에 아무 MapObject가 없으면 WaterBomb 오브젝트를 생성하도록 함. 플레이어의 id와 물폭탄 생성 위치를 서버로 보냄.
```
message C_WaterBomb
{
	uint64 ownerid = 1;
	int32 tileposx = 2;
	int32 tileposy = 3;
}
```
## 5. TileMap 리소스 역할 재정의
### 이유
TileMap은 int value를 가지는 이차원 벡터를 가지고 있는 리소스이고, 이를 클라의 Scene과 서버의 Room에서 들고 있고 맵 오브젝트들을 TileMap으로 관리했었는데, MapObject의 이차원 벡터를 두고 이를 관리하는 게 훨씬 용이할 듯하여, TileMap은 정보 저장 및 전달 용으로만 활용하기로 함.
### 상세
```
vector<vector<MapObjectRef>> _mapObjects;
//Tilemap _tilemap;
```

# 2026.03.11
## 1. 클라, 서버 블럭 충돌 처리 방식 통일
### 이유
플레이어가 블럭에 가로막히는 것을 구현하기 위해, 기존에 클라 측은 엔진단에 구현해놓은 OnColliderStayOverlap()을 이용하여 플레이어의 collider가 블럭의 collider에 들어간만큼 다시 빼주도록 하였고, 서버 측은 이동할 좌표를 검사하여 갈 수 있는 곳까지만 가게끔 처리하였음.

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

## 3. 서버에서 물폭탄 생성 및 브로드캐스트
### 상세
클라에서 물폭탄 생성 패킷이 오면 다시 한번 해당 위치에 맵 오브젝트가 없는지 검증하고 생성 후 브로드캐스트.
### 문제
키 입력에 클라가 일단 반응하는 것을 원칙으로 삼았기 때문에, 클라 측에선 일단 id 없이 물폭탄 오브젝트를 생성하고 서버에 패킷을 전송함. 패킷이 오면 id를 부여 받아야 하는데, id 없이 생성한 오브젝트를 어떻게 탐색할 지 고민.
### 해결
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
# 2026.03.12 
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
# 2026.03.14

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
# 2026.03.15
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


## 3. 클라 측 물폭탄 터짐 처리 : 이펙트, 블럭 부수기
### 상세
패킷 정보에 따라 이펙트 오브젝트를 생성하고, 블럭을 파괴함.

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
## 3. 물폭탄 폭발 로직 개선
### 이유
기존에 폭발 범위에 물폭탄이 있던 자리는 포함을 안 시켜서, 어떻게 넣을 지 고민하다 전체적으로 리팩토링함.
### 상세
폭발 범위를 계산하는 부분과, 폭발 범위 내의 오브젝트를 처리하는 부분을 분리함.

## 4. 플레이어 갇힘 구현
### 상세
애니메이션을 세팅하고, TRAPPED_IDLE, TRAPPED_MOVE 상태를 추가하여 클라, 서버 측의 갇힘 상태를 구현함.
### 문제
상태 처리가 복잡해져 코드가 지저분해지고 꼬임. 특히 이동 부분.
### 해결
메인 상태를 NORMAL, TRAPPED, DEAD로 두고, 이동 상태를 IDLE, MOVE로 두어 상태를 분리함. 상당 부분 고쳐야 하여 진행 중.
## 느낀 점
코딩을 할 때 MyPlayer -> 서버 -> 클라 순서로 생각을 하면 흐름이 잘 잡힌다.

# 2026.03.18
## 1. 플레이어 상태 분리
위 기술한대로 메인 상태를 NORMAL, TRAPPED, DEAD로 두고, 이동 상태를 IDLE, MOVE로 두어 상태를 분리함. 이동 패킷엔 이동 상태를 전달함.

다음과 같이 NORMAL, TRAPPED 내부에서 IDLE, MOVE 상태로 나뉘어 관리함.
```
void MyPlayer::OnUpdateNormal()
{
	HandleBombInput();

	switch (_moveState)
	{
	case MOVE_STATE_IDLE:
		HandleMoveInput_Idle();
		break;
	case MOVE_STATE_MOVE:
		HandleMoveInput_Move();
		Move();
		CheckOverlapBombs();
		break;
	}
}

void MyPlayer::OnUpdateTrapped()
{
	switch (_moveState)
	{
	case MOVE_STATE_IDLE:
		HandleMoveInput_Idle();
		break;
	case MOVE_STATE_MOVE:
		HandleMoveInput_Move();
		Move();
		break;
	}
}
```
## 2. 클라 Player에서 이동 로직 삭제
### 이유
클라 Player는 껍데기일 뿐인데, state에 따라서 이동하고 멈추는 로직이 존재하는 것이 이상하다고 판단. 또한 그 때문에 서버 위치로 도달 후에야 state를 바꿔주는 불편한 동작이 있었음.
## 상세
DEAD 상태를 제외하곤 그저 서버 위치를 따라가도록 함.
```
void Player::OnUpdateNormal()
{
	SyncFromServer();
}

void Player::OnUpdateTrapped()
{
	SyncFromServer();
}

void Player::OnUpdateDead()
{
}
```
## 3. 플레이어 죽음 구현
### 상세
1. 서버 측에서 플레이어가 물풍선에 갇히고 일정 시간이 지나면 DEAD 상태로 두고, 죽은 플레이어의 id를 포함한 패킷을 브로드캐스트함.
2. 서버 측에서 NORMAL 상태의 플레이어가 TRAPPED 상태의 플레이어와 충돌 시 TRAPPED 상태의 플레이어를 DEAD 상태로 두고 죽음 패킷을 브로드캐스트함.

## 느낀 점
상태를 계층적으로 분리하는 것은 처음 해봤는데, 코드가 꽤 깔끔해지는 것 같다.
모든 걸 클라에서 '일단' 처리할 필요는 없는 것 같다. 플레이어 죽임 같은 경우 일단 죽였는데 서버에서 패킷이 오니 막상 엉뚱한 놈을 죽였으면 처리가 부자연스러워진다.

# 2026.03.19
## 1. 물폭탄 '일단' 생성 방식 폐기
### 이유
기존에 클라 측에서 키를 입력하면 WaterBomb 오브젝트를 일단 생성하고, 서버로부터 패킷이 오면 그때 id를 부여하는 '일단 보여주기' 방식을 사용했음. 그러나 물방울 아이템을 구현하려다보니 현재 물방울 개수가 서버 뿐만 아니라 클라에도 들고 있어야 하는데, 이가 비효율적이라고 생각됨. '일단 보여주기' 방식을 사용하려면 클라, 서버 모두 같은 작업이 (번거롭게도) 필요한 것으로 보여, 이러한 방식을 최소화하고자 함.
### 상세
클라 측에서 키 입력 시 패킷을 보내고, 다시 브로드캐스트됐을 때 WaterBomb 오브젝트를 생성함.
## 2. MapObject 생성 시 타입을 이중으로 전달하도록 변경
### 이유
여러 종류의 아이템을 생성하기 위해. 또한 다른 타입의 MapObject도 향후 내부적으로 여러 타입을 가질 수 있음.
### 상세
Tilemap도 mapobjecttype, detailedtype의 이중 타입을 가지도록 함.
```
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
```
## 3. 아이템 3종 구현
### 상세
- 아이템 클래스 설계  
	MapObject -> Item -> BubbleItem, FluidItem, RollerItem, NeedleItem
- 물풍선(Bubble) 아이템 구현  
	서버 측에서 Player에 현재 설치된 물폭탄 수와 bubble 아이템 획득 수를 관리하여 스페이스바 입력 패킷이 오면 해당 정보들로 현재 놓을 수 있는지 검사한 후 물폭탄을 설치하도록 함.
- 액체(Fluid) 아이템 구현  
	서버 측에서 물폭탄이 터질 때 owner->fluidCount를 확인하여 범위를 계산하도록 함.
- 롤러(Roller) 아이템 구현  
	서버 측에서 rollerCount를 이용해 NORMAL 상태일 때 속도를 계산하도록 하고, 클라의 예측 이동을 위해 해당 속도를 전송하도록 함.
- 폭발에 닿은 아이템 파괴 구현

## 느낀 점
클라 측에서 입력 시 서버를 거치지 않고 일단 동작시키는 방법은 네트워크 지연 시 유저가 덜 불편하다는 장점이 있겠지만, 그렇지 않았을 때 서버 측만 로직을 구현하면 되는 반면, 그렇게 했을 때 클라 측도 같은 로직을 구현해야 하는 번거로움이 있는 것 같다. 일단 이동을 제외한 나머지는 일단 동작 방식을 사용하지 않으려고 한다.
<br><br>