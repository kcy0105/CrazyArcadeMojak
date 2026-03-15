
# 2026-03-14  패킷 처리 자동화 및 전송 인터페이스 개선

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