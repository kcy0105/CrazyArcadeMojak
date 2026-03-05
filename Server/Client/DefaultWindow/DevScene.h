#pragma once
#include "Scene.h"

class MyPlayer;
class Player;
//class TilemapObject;
class Tilemap;

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
	uint64 GetMyPlayerId();


	Player* GetSyncObject(uint64 id);


private:
	MyPlayer* _myPlayer = nullptr;
	//TilemapObject* _tilemapObj = nullptr;
};

