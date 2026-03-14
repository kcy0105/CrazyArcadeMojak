#pragma once
#include "Scene.h"

class MyPlayer;
class MapObject;
class SyncObject;

class DevScene : public Scene
{
public:
	DevScene() {}
	virtual ~DevScene() override {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRelease() override;

public:
	void SetMyPlayer(MyPlayer* myPlayer) { _myPlayer = myPlayer; }
	MyPlayer* GetMyPlayer() { return _myPlayer; }
	uint64 GetMyPlayerId();

public:
	SyncObject* GetSyncObject(uint64 id);

private:
	MyPlayer* _myPlayer = nullptr;
};

