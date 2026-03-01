#pragma once
#include "Scene.h"

class MyPlayer;
class Player;

class DevScene : public Scene
{
public:
	DevScene() {}
	virtual ~DevScene() override {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRelease() override;

	void SetMyPlayer(MyPlayer* myPlayer) { _myPlayer = myPlayer; }
	MyPlayer* GetMyPlayer() { return _myPlayer; }

	Player* GetNetworkObject(uint64 id);

private:
	MyPlayer* _myPlayer = nullptr;
};

