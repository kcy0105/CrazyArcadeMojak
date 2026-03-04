#include "pch.h"
#include <iostream>
#include <thread>
#include <vector>
using namespace std;
#include <atomic>
#include <mutex>
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "Listener.h"
#include "Service.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ServerPacketHandler.h"
#include "GameRoom.h"

int main()
{
	SocketUtils::Init();
	GRoom->Init();

	ServerServiceRef service = make_shared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		make_shared<IocpCore>(),
		[](){ return make_shared<GameSession>(); }, // TODO : SessionManager 등
		100);

	assert(service->Start());

	float accumulator = 0.f;
	uint64 last = GetTickCount64();

	while (true)
	{
		service->GetIocpCore()->Dispatch(0);

		uint64 now = GetTickCount64();

		float delta = (now - last) / 1000.f;
		last = now;

		accumulator += delta;


		while (accumulator >= TICK)
		{
			GRoom->Update();
			
		}
	}

	GThreadManager->Join();
	
	// 윈속 종료
	SocketUtils::Clear();	
}